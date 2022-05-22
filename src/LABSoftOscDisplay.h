#ifndef LABSOFTOSCDISPLAY_H
#define LABSOFTOSCDISPLAY_H

#include "LABSoftGlobals.h"

#include <gtkmm/glarea.h>
#include <gtkmm/builder.h>

class LABSoftOscDisplay
{
private:
  Glib::RefPtr<Gtk::Builder> builder;
  Gtk::GLArea* glarea_disp_graph;
  
  // changeable vars
  int num_chans {};
  // Structure for a 3D point
  typedef struct {
      GLfloat x;
      GLfloat y;
      GLfloat z;
  } POINT;

  // Structure for a trace
  typedef struct {
      int np, mod;
      POINT *pts;
  } TRACE;

  // Traces (grid plus channels)
  TRACE traces[MAX_TRACES];

  // Buffer for FIFO text, and shader compiler messages
  char txtbuff[20000];
  // Buffer for floating-point FIFO values
  float fifo_vals[MAX_VALS];

  // Scale & offset values for each trace. First is grid,
  // the rest depend on number of traces.
  GLfloat trace_scoffs[MAX_TRACES][2] = {{1,0}};

  // Variables to hold IDs for program & vertex buffer
  GLuint program, vbo, vao;
  
  // IDs for 3D coords, colour array, scale & offset array
  GLint a_coord3d, u_colours, u_scoffs;

  int nvertices, 
      frame_count, 
      win_width, 
      win_height, 
      num_vals;
      
  int use_fifo, 
      osc_fifo_fd, 
      fifo_in, 
      discard, 
      chan_vals;
      
  int args, verbose, vert_buff_alloc, paused;
  float trace_ymax;
  
  const char* osc_fifo_name;

public:
  LABSoftOscDisplay (Glib::RefPtr<Gtk::Builder> builder);
  

  void idle_handler ();
  void timer_handler (int value);
  
  // --- Signal Handlers ---
  void on_realize ();
  bool on_render (const Glib::RefPtr<Gdk::GLContext>&);
  void on_resize (int width, int height);
  void on_unrealize ();
   
  // --- FIFO ---
  int fifo_read (float *vals, int maxvals);
  int is_fifo (const char *fname);
  
  // --- GL ---
  GLint get_attrib (GLuint prog, const char *name);
  GLint get_uniform (GLuint prog, const char *name);
  GLuint create_shader(const char *source, GLenum typ);
  GLuint create_program (const char *verts, const char *frags);
  
  // --- FOR DRAW ---
  void set_point (POINT *pp, float x, float y, float z);
  int move_draw_line (POINT *p, float x1, float y1, float x2, float y2, int z);
  int draw_grid (POINT *p, int nx, int ny, int z);
  int create_grid(TRACE *tp, int nx, int ny, int z);
  int create_polyline (TRACE *tp, float xmin, float ymin, float xmax, float ymax, float *vals, int np, int zval);
  void update_polyline(TRACE *tp, float *vals, int np);
  int create_trace (TRACE *tp, int idx, float *vals, int np, int zval);
  int create_test_trace (TRACE *tp, int idx, int np, int zval);
  void free_traces ();
  void init_scale_offset (float ymax);
  void reshape (int width, int height);
  int add_vertex_data ();
  
  // --- GRAPH ---
  int  graph_init ();
  void graph_display ();
  void graph_free();
  void graph_do ();
};

#endif

// EOF
