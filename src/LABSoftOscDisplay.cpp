#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <math.h>
#include <iostream>
#include <fcntl.h>
#include <epoxy/gl.h>

#include "LABSoftOscDisplay.h"

LABSoftOscDisplay::LABSoftOscDisplay (Glib::RefPtr<Gtk::Builder> builder) 
: osc_fifo_name {OSC_FIFO_NAME},
  num_vals      {NUM_VALS},
  trace_ymax    {TRACE_YMAX},
  num_chans     {NUM_CHANS},
  verbose       {VERBOSE}
{
  std::cout << "DEBUG: LABSoftOscDisplay constructor begin.\n";
  
  builder->get_widget("glarea_disp_graph", glarea_disp_graph);
    
  chan_vals = num_vals / num_chans;
  
  if (!is_fifo (osc_fifo_name) || (osc_fifo_fd = open (osc_fifo_name, O_RDONLY)) == -1 ||
          fcntl (osc_fifo_fd, F_SETFL, O_NONBLOCK) == -1)
    {
      printf ("Can't open %s\n", osc_fifo_name);
    }
  else
    {
      printf("Reading FIFO %s\n", osc_fifo_name);
      use_fifo = 1;
    }
  
  // Connect GLArea signals
  glarea_disp_graph->signal_realize().connect   (sigc::mem_fun(*this, &LABSoftOscDisplay::on_realize));
  glarea_disp_graph->signal_render().connect    (sigc::mem_fun(*this, &LABSoftOscDisplay::on_render));
  glarea_disp_graph->signal_resize().connect    (sigc::mem_fun(*this, &LABSoftOscDisplay::on_resize));
  glarea_disp_graph->signal_unrealize().connect (sigc::mem_fun(*this, &LABSoftOscDisplay::on_unrealize));
  
  std::cout << "DEBUG: LABSoftOscDisplay constructor end.\n";
}

// Signal Handlers
void
LABSoftOscDisplay::on_realize ()
{  
  glarea_disp_graph->make_current ();
  
  // If there were errors during the initialization or
  // when trying to make the context current, this
  // function will return a `GError` for you to catch
  try
    {
      glarea_disp_graph->throw_if_error ();
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "An error occured making the context current:" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }

  program = create_program (vert_shader, frag_shader);
  
  if (program == 0)
    {
      std::cout << "Error in program creation.\n";
    //return 0;
    }
  
  a_coord3d = get_attrib  (program, "coord3d");
  u_colours = get_uniform (program, "u_colours");
  u_scoffs  = get_uniform (program, "u_scoffs");
    
  if (a_coord3d == -1 || u_colours == -1 || u_scoffs == -1)
    {
      std::cout << "Error in get_attrib/get_uniform.\n";
      // return 0;
    }
    
  // Draw grid and trace
  create_grid (&traces[0], GRID_DIVS, GRID_CHAN);
  std::cout << "123123\n";
  
  for (int i = 0; i < num_chans; i++)
    create_test_trace (&traces[i+1], i, chan_vals, TRACE1_CHAN + i);
    
    
  // Line width and multisample anti-alias
  glLineWidth (LINE_WIDTH);
  // glEnable (GLUT_MULTISAMPLE);
   
  // Enable blending
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // create vao
  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);

  // Create the vertex buffer object
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);

  // Copy data into the buffer object
  nvertices = add_vertex_data ();
  glEnableVertexAttribArray (a_coord3d);
  glVertexAttribPointer (a_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

  init_scale_offset (trace_ymax);
}

bool 
LABSoftOscDisplay::on_render (const Glib::RefPtr<Gdk::GLContext>&)
{
  // make glarea_disp_graph context current
  glarea_disp_graph->make_current ();
    
  // If there were errors during the initialization or
  // when trying to make the context current, this
  // function will return a `GError` for you to catch
  try
    {
      glarea_disp_graph->throw_if_error ();
    }
  catch (const Gdk::GLError& gle)
    {
      std::cerr << "An error occured making the context current during realize:" << std::endl;
      std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
  
  glUseProgram (program);
  
  glUniform4fv (u_colours, MAX_TRACES, (GLfloat *)trace_colours);
  glUniform2fv (u_scoffs, MAX_TRACES, (GLfloat *)trace_scoffs);
  
  glClearColor (CLEAR_COLOUR);
  glClear (GL_COLOR_BUFFER_BIT);
  
  glDrawArrays (GL_LINE_STRIP, 0, nvertices);
  
  //#if GLUT_MODE == GLUT_DOUBLE
    //glutSwapBuffers ();
  //#else
    //glFlush ();
  
  glFlush ();
  
  frame_count++;
  
  return FALSE;
}

void 
LABSoftOscDisplay::on_resize (int width,
                                                int height)
{
  width = (win_width = width) - X_MARGIN * 2;
  height = (win_height = height) - Y_MARGIN * 2;
  
  glViewport (X_MARGIN, Y_MARGIN, (GLsizei)width, (GLsizei)height);
}

void
LABSoftOscDisplay::on_unrealize ()
{
  glarea_disp_graph->make_current ();
  
  glDeleteProgram (program);
}


// --- FIFO ---
// Read in comma or space-delimited floating-point values
int 
LABSoftOscDisplay::fifo_read (float *vals, 
                              int    maxvals)
{
  int i, n, nvals = 0, done = 0;
  char *s;

    while (!done && (n = read(osc_fifo_fd, &txtbuff[fifo_in], sizeof(txtbuff)-fifo_in-1)) > 0)
    {
        txtbuff[fifo_in + n] = 0;
        if ((s=strchr(&txtbuff[fifo_in], '\n')) != 0)
        {
            s = txtbuff;
            while (!done && (i = strcspn(s, " ,\t\r\n")) > 0 && nvals < maxvals)
            {
                if (!discard)
                    vals[nvals++] = atof(s);
                s += i;
                if (*s == '\n')
                {
                    if ((i=strlen(s+1)) > 0)
                    {
                        strcpy(txtbuff, s+1);
                        fifo_in = i;
                    }
                    else
                        fifo_in = 0;
                    done = 1;
                }
                else while (*s==',' || *s==' ' || *s=='\t' || *s=='\r')
                    s++;
            }
            discard = 0;
        }
        else if ((fifo_in += n) >= sizeof(txtbuff)-2)
        {
            discard = 1;
            fifo_in = nvals = 0;
        }
        if (verbose && nvals)
        {
            for (i=0; i<nvals; i++)
                printf("%1.3f ", vals[i]);
            printf("\n");
        }
    }
    
  return (nvals);
}

// Check if fifo exists
int 
LABSoftOscDisplay::is_fifo (const char *fname)
{
  struct stat stat_p;
  stat (fname, &stat_p);
  
  return (S_ISFIFO (stat_p.st_mode));
}

// Handler for idle events
bool 
LABSoftOscDisplay::idle_handler ()
{
  std::cout << ".";
  
  glarea_disp_graph->make_current ();
  
  int n, i;

   if (use_fifo && (n = fifo_read(fifo_vals, MAX_VALS)) > 0 && !paused)
    {
      for (i=0; i<num_chans; i++)
        update_polyline(&traces[TRACE1_CHAN+i], fifo_vals+i, n/num_chans);
        
      add_vertex_data();
    }
    
  // glutPostRedisplay();
  glarea_disp_graph->queue_render ();
  
  return TRUE;
}

// Handler for timer events
void 
LABSoftOscDisplay::timer_handler (int value)
{
    char temps[50] = "";

    if (value)
    {
        sprintf(temps, "%d FPS, %u x %u", frame_count, win_width, win_height);
        //glutSetWindowTitle(temps);
    }
    frame_count = 0;
    //glutTimerFunc(TIMER_MSEC, timer_handler, 1);
}

// --- GL ---
// Get attribute
GLint 
LABSoftOscDisplay::get_attrib (GLuint 		 prog, 
                               const char *name)
{
  GLint attrib = glGetAttribLocation (prog, name);
  
  if (attrib == -1)
    printf("Can't bind attribute '%s'\n", name);
    
  return (attrib);
}

// Get uniform
GLint 
LABSoftOscDisplay::get_uniform (GLuint      prog,
                                const char *name)
{
  GLint uniform = glGetUniformLocation (program, name);
  
  if (uniform == -1)
    fprintf(stderr, "Could not bind uniform '%s'\n", name);
    
  return (uniform);
}

// Create shader, given source code string
GLuint 
LABSoftOscDisplay::create_shader (const char *source, 
                                  GLenum      typ)
{
  GLuint res = glCreateShader (typ);
  GLint ok = GL_FALSE;
  char *s = 0;

  glShaderSource (res, 1, &source, NULL);
  glCompileShader(res);
  glGetShaderiv (res, GL_COMPILE_STATUS, &ok);
  printf ("%s shader ", typ==GL_FRAGMENT_SHADER ? "Frag" : "Vert");
  
  if (ok == GL_FALSE)
    {
      printf ("error:\n%s", txtbuff);
      
      glGetShaderInfoLog (res, sizeof(txtbuff), 0, txtbuff);
      
      while ((s=strtok(s?0:txtbuff, "\n")) != 0)
        printf("  %s\n", s);
        
      glDeleteShader (res);
      
      res = 0;
    }
  else
    {
      printf ("compiled OK\n");
    }
    
  return (res);
}

// Create GLSL program, given vertex and fragment shader strings
GLuint
LABSoftOscDisplay::create_program (const char *verts,
                                   const char *frags)
{
  GLuint prog = glCreateProgram ();
  GLuint vshader = 0, fshader = 0;

  if (verts && (vshader = create_shader (verts, GL_VERTEX_SHADER)) != 0)
    glAttachShader(prog, vshader);
  if (frags && (fshader = create_shader (frags, GL_FRAGMENT_SHADER)) != 0)
    glAttachShader (prog, fshader);
  if (vshader && fshader)
    glLinkProgram (prog);
    
  return (vshader && fshader ? prog : 0);
}



// --- FOR DRAW ---
// Set x, y and z values for single point
void 
LABSoftOscDisplay::set_point (POINT *pp, 
                              float  x, 
                              float  y, 
                              float  z)
{
  pp->x = x;
  pp->y = y;
  pp->z = z;
}

// Move, then draw line between 2 points
int 
LABSoftOscDisplay::move_draw_line (POINT *p, 
                                   float  x1, 
                                   float  y1, 
                                   float  x2, 
                                   float  y2, 
                                   int    z)
{
  set_point (p++, x1, y1, z);
  set_point (p++, x1, y1, ZEN (z));
  set_point (p++, x2, y2, ZEN (z));
  set_point (p++, x2, y2, z);
  
  return(4);
}

// Create grid data, with scaling
int 
LABSoftOscDisplay::draw_grid (POINT *p, 
                              int    nx, 
                              int    ny, 
                              int    z)
{
  float x, y;
  int i, n = 0;

  for (i = 0; i <= nx; i++)
    {
      x = NORM_XMIN + (NORM_XMAX - NORM_XMIN) * i / nx;
      n += move_draw_line (&p[n], x, NORM_YMIN, x, NORM_YMAX, z);
    }
    
  for (i = 0; i <= ny; i++)
    {
      y = NORM_YMIN + (NORM_YMAX - NORM_YMIN) * i / ny;
      n += move_draw_line (&p[n], NORM_XMIN, y, NORM_XMAX, y, z);
    }
    
  return (n);
}

// Create grid data, with scaling
int 
LABSoftOscDisplay::create_grid (TRACE *tp, 
                                int    nx, 
                                int    ny, 
                                int    z)
{
  int n = 0;

  if (!tp->pts)
    tp->pts = (POINT *) malloc ((nx + 1 + ny + 1) * 4 * sizeof(POINT));
    
  if (tp->pts)
    n = draw_grid (tp->pts, nx, ny, z);
    
  tp->mod = 1;
  
  return (tp->np = n);
}

// Create polyline data, given trace values
int 
LABSoftOscDisplay::create_polyline (TRACE *tp, 
                                    float  xmin, 
                                    float  ymin, 
                                    float  xmax, 
                                    float  ymax, 
                                    float *vals, 
                                    int    np, 
                                    int    zval)
{
  int n, start = 1;
  float x, y;
  POINT *pts;

  tp->np = np + 2;
  if ((pts = tp->pts = (POINT *)calloc(tp->np, sizeof(POINT))) != 0)
    {
      for (n=0; n<np; n++)
        {
          x = xmin + (xmax-xmin) * n / (np - 1);
          y = vals ? *vals++ : 0;
          
          if (start)
            set_point(pts++, x, y, zval);
            
          set_point(pts++, x, y, ZEN(zval));
          start = 0;
        }
        
      set_point(pts++, x, y, 0);
    }
    
  tp->mod = 1;
    
  return (pts ? tp->np : 0);
}

// Update y data in an existing polyline
void 
LABSoftOscDisplay::update_polyline (TRACE *tp, 
                                    float *vals, 
                                    int    np)
{
  int n, start = 1;
  POINT *pts = tp->pts;
  float val;

  np = np > tp->np-2 ? tp->np-2 : np;
  
  for (n=0; n<np; n++)
    {
      val = vals[n*num_chans];
      
      if (start)
        pts++->y = val;
        
      pts++->y = val;
      start = 0;
    }
    
  pts->y = val;
  tp->mod = 1;
}

// Create a trace
int 
LABSoftOscDisplay::create_trace (TRACE *tp, 
                                 int    idx, 
                                 float *vals, 
                                 int    np, 
                                 int    zval)
{
  float ymin = NORM_YMIN + (NORM_YMAX-NORM_YMIN) * idx / num_chans;
  float ymax = ymin + (NORM_YMAX-NORM_YMIN) / num_chans;

  return (create_polyline(tp, NORM_XMIN, ymin, NORM_XMAX, ymax, vals, np, zval));
}

// Create the traces, and initialise them with test data
int 
LABSoftOscDisplay::create_test_trace (TRACE *tp, 
                                      int    idx, 
                                      int    np, 
                                      int    zval)
{
  int i, n = 0;
  float *vals = (float *)malloc(np * sizeof(float));

  if (vals)
    {
       for (i = 0; i < np; i++)
          vals[i] = (sin(i / 10.0 - M_PI / 2) + 1) *
            trace_ymax / (2.0 + i/100.0);
            
      n = create_trace (tp, idx, vals, np, zval);
      free(vals);
    }
    
  return (n);
}

// Free memory for all traces
void
LABSoftOscDisplay::free_traces ()
{
  int i;

  for (i = 0; i < MAX_TRACES; i++)
    {
      if (traces[i].pts)
        free(traces[i].pts);
    }
}

// Set up the scale and offset values for the channels
void 
LABSoftOscDisplay::init_scale_offset (float ymax)
{
  for (int n = 0; n < num_chans; n++)
    {
      trace_scoffs[n+TRACE1_CHAN][0] = (NORM_YMAX-NORM_YMIN) / (ymax*num_chans);
      trace_scoffs[n+TRACE1_CHAN][1] = NORM_YMIN + n * (NORM_YMAX-NORM_YMIN) / num_chans;
    }
}

// Reshape the plot area when window resizes
void 
LABSoftOscDisplay::reshape (int width, 
                            int height)
{
  width = (win_width = width) - X_MARGIN * 2;
  height = (win_height = height) - Y_MARGIN * 2;
  glViewport (X_MARGIN, Y_MARGIN, (GLsizei)width, (GLsizei)height);
}

// Add trace vertex data to program
// If vertex buffer doesn't exist, create it
int 
LABSoftOscDisplay::add_vertex_data ()
{
  int i, npts;

  if (!vert_buff_alloc)
    {
      for(i=npts=0; traces[i].np>0; i++)
        npts += traces[i].np;
        
      glBufferData(GL_ARRAY_BUFFER, npts*sizeof(POINT), 0, GL_STATIC_DRAW);
      vert_buff_alloc = 1;
    }
    
  for(i=npts=0; traces[i].np>0; i++)
    {
      if (traces[i].mod)
        glBufferSubData(GL_ARRAY_BUFFER, npts*sizeof(POINT),
          traces[i].np*sizeof(POINT), traces[i].pts);
          
      npts += traces[i].np;
    }
    
  return (npts);
}

// Initialise graph
int
LABSoftOscDisplay::graph_init()
{
  program = create_program (vert_shader, frag_shader);
  
  if (program == 0)
    return 0;
    
  a_coord3d = get_attrib  (program, "coord3d");
  u_colours = get_uniform (program, "u_colours");
  u_scoffs  = get_uniform (program, "u_scoffs");

  if (a_coord3d == -1 || u_colours == -1 || u_scoffs == -1)
    return(0);

  // Draw grid and trace
  create_grid (&traces[0], GRID_DIVS, GRID_CHAN);
  for (int i=0; i<num_chans; i++)
    create_test_trace(&traces[i+1], i, chan_vals, TRACE1_CHAN+i);

  // Line width, and multisample anti-alias
  glLineWidth (LINE_WIDTH);
  //glEnable (GLUT_MULTISAMPLE);

  // Enable blending
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Create the vertex buffer object
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);

  // Copy data into the buffer object
  nvertices = add_vertex_data ();
  glEnableVertexAttribArray (a_coord3d);
  glVertexAttribPointer (a_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

  init_scale_offset(trace_ymax);
  
  return (1);
}

// Run the shaders to update the display
void 
LABSoftOscDisplay::graph_display ()
{
  glarea_disp_graph->make_current ();
  
  glUseProgram (program);

  glUniform4fv (u_colours, MAX_TRACES, (GLfloat *)trace_colours);
  glUniform2fv (u_scoffs, MAX_TRACES,  (GLfloat *)trace_scoffs);

  glClearColor (CLEAR_COLOUR);
  glClear      (GL_COLOR_BUFFER_BIT);

  glDrawArrays(GL_LINE_STRIP, 0, nvertices);
  
  glFlush ();
  
  frame_count++;
  
  // return FALSE;
}

// Clean up on exit
void 
LABSoftOscDisplay::graph_free ()
{
  glDeleteProgram (program);
}

// EOF 
