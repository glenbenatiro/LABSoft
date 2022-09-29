#include <cmath>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Input_Choice.H>
#include <FL/Fl_Menu_Button.H>

#define LABSOFT_OSCILLOSCOPE_DISPLAY_LABEL_SIZE 5

class out : public Fl_Input_Choice
{
  public:
    float m_val = 0.0;

    out (int X, int Y, int W, int H, const char *l) : Fl_Input_Choice (X, Y, W, H, l)
    {

    }

    int handle (int e) {
      int key = Fl::event_key ();

      switch (e)
        {
          case FL_KEYDOWN:
            if (key == FL_Enter) {
              m_val = atof (this->input()->value());

              printf ("fl_keydown\n");
            }

            else if (key == 32) {
              printf ("spacebar?\n");
            }

            break;
          }         

      return (Fl_Input_Choice::handle(e));
    }
};


class LABSoft_Oscilloscope_Display : public Fl_Widget
{
  private:
    int m_grid_color = FL_WHITE,
        m_number_of_columns = 10,
        m_number_of_rows = 10,
        m_default_color = FL_BLACK,
        m_background_color = FL_BLACK;

  public:
    LABSoft_Oscilloscope_Display (int X, int Y, int W, int H) : Fl_Widget (X, Y, W, H, 0)
    {

    }

    void rows_columns (int rows, int columns)
    {
      m_number_of_rows = rows;
      m_number_of_columns = columns;
    }

    void draw ()
    {
      draw_box (FL_FLAT_BOX, m_background_color);
      draw_grid ();
      
      // if (m_is_display_enabled)
      //   draw_channel_signals ();
    }

    void draw_grid ()
    {
      // set color
      fl_color (m_grid_color);

      // draw grid outer box
      fl_line_style (FL_SOLID, 0, NULL);
      fl_line (x (), y(), x () + w (), y ());                 // up
      fl_line (x () + w (), y (), x () + w (), y () + h ());  // right
      fl_line (x () + w (), y () + h (), x (), y () + h ());  // down
      fl_line (x (), y () + h (), x (), y ());                // left

      // draw rows
      for (int a = 0; a < (m_number_of_rows - 1); a++)
        {
          if (a == ((m_number_of_rows / 2) - 1))
            fl_line_style (FL_DASH, 0, NULL);
          else 
            fl_line_style (FL_DOT, 0, NULL);

          int Y = round ((a + 1) * ((float)h () / (float)m_number_of_rows)) + y ();
          fl_line (x (), Y, x () + w (), Y);
        }

      // draw columns 
      for (int a = 0; a < (m_number_of_columns - 1); a++)
        {
          if (a == ((m_number_of_columns / 2) - 1))
            fl_line_style (FL_DASH, 0, NULL);
          else 
            fl_line_style (FL_DOT, 0, NULL);

          int X = round ((a + 1) * ((float)w () / (float)m_number_of_columns)) + x ();
          fl_line (X, y (), X, y () + h ());
        }
      
      // reset color
      fl_line_style (0);
      fl_color (m_default_color);
    }


};




class LABSoft_Oscilloscope_Display_Group : public Fl_Group
{
  public:
    LABSoft_Oscilloscope_Display *m_disp;
    
    // for core oscilloscope display operation
    int m_number_of_rows = 10,
        m_number_of_columns = 10,
       
        m_x_offset = 0,
        m_number_of_channels = 2;
    
    float  m_time_per_div = 1.0;

    // for drawing UI
    int m_upper_padding = 50,
        m_lower_padding = 50,
        m_left_padding = 80,
        m_right_padding = 65,
        m_label_width = 0,
        m_label_height = 0,
        m_ylabel1_padding_from_graph = 30,
        m_ylabel2_padding_from_graph = 60,
        m_label_size = 10,
        m_chan1_color = 0x00000003,
        m_chan2_color = 0x00000006,
        m_xlabel_inner_padding = 10,
        m_label_color = 0xFFFFFF00;


    std::vector<Fl_Box*> x_labels, y1_label, y2_label;
    

  

    LABSoft_Oscilloscope_Display_Group (int X, int Y, int W, int H) : Fl_Group (X, Y, W, H, 0)
    {
          m_disp = new LABSoft_Oscilloscope_Display (X + m_left_padding , 
        Y + m_upper_padding , W - m_left_padding - m_right_padding, H - m_upper_padding - m_lower_padding);

      m_disp->rows_columns (m_number_of_rows, m_number_of_columns);

      // dyamically add labels
      // x label
      for (int a = 0; a < m_number_of_columns + 1; a++)
      {
        int x_coord =  (a) * ((float)(W - m_left_padding - m_right_padding) / (float)(m_number_of_columns));
        Fl_Box *lbl = new Fl_Box (X + m_left_padding + x_coord, H + Y - m_lower_padding + m_xlabel_inner_padding, m_label_width, m_label_height, "5ms");
        lbl->labelcolor (m_label_color);
        lbl->labelsize (m_label_size);
        x_labels.push_back (lbl);
      }

      // y label
      for (int a = 0; a <  m_number_of_channels; a++) {
        std::vector<Fl_Box*> *vec = (a == 0) ? &y1_label : &y2_label;
        int padding = (a == 0) ? m_ylabel1_padding_from_graph : m_ylabel2_padding_from_graph;

        for (int b = 0; b < m_number_of_rows + 1; b++) {
          int y_coord =  (b) * ((float)(H - m_upper_padding - m_lower_padding) / (float)(m_number_of_rows));
          Fl_Box *lbl = new Fl_Box (X + m_left_padding - padding, Y + m_upper_padding + y_coord, m_label_width, m_label_height, "5ms");
          lbl->labelcolor ((a == 0) ? m_chan1_color : m_chan2_color);
          lbl->labelsize (m_label_size);
          vec->push_back (lbl);
        }
      }      
      
      end ();

      update_x_axis_labels ();
    }

    void update_x_axis_labels ()
    {
      char s[50];

      // update x labels
      for (int a = 0; a < m_number_of_columns + 1; a++) {      
        sprintf(s, "%.1f s", ((a + m_x_offset - ((float)m_number_of_columns / 2.0)) * m_time_per_div));
        x_labels[a]->copy_label (s);
        x_labels[a]->labelcolor (m_label_color);
        x_labels[a]->redraw_label ();
      }
    }

  

    void draw ()
    {
      draw_box (FL_FLAT_BOX, FL_BLACK);
      draw_children ();

      // dynamically draw labels

    }

    void rows_columns (int rows, int columns)
    {
      m_number_of_rows = rows;
      m_number_of_columns = columns;
    }
};




float label_to_scaled_value (const char *label)
{
  float scaler = 1.0;
  float value = atof (strtok (strdup (label),  " "));
  char unit = strtok (NULL, "/")[0];

  switch (unit)
    {
      case 'G':
          scaler = 1000000000.0;
          break;
      case 'M':
          scaler = 1000000.0;
          break;
      case 'k':
          scaler = 1000.0;
          break;
      case 'm':
          scaler = 0.001;
          break;
      case 'u':
          scaler = 0.000001;
          break;
      case 'n':
          scaler = 0.000000001;
          break;
      default:
          scaler = 1.0;
          break;
    }
  
  return value * scaler;
}

void timeout (void *data)
{
  ((Fl_Double_Window*)data)->flush();
  Fl::repeat_timeout(1/30, timeout, data);
}

LABSoft_Oscilloscope_Display_Group *disp_group;

 void cb_time_per_div (Fl_Input_Choice *w, void *data)
   {
     disp_group->m_time_per_div = label_to_scaled_value (w->input()->value());
     printf ("callback\n");
     disp_group->update_x_axis_labels ();
     //Fl::flush ();
     //disp_group->parent()->flush();

     Fl_Double_Window *win = (Fl_Double_Window*)data;
     win->flush();
   }


int main ()
{
  Fl_Double_Window win (0, 0, 1366, 768, "fltk widget");
  win.begin ();

  disp_group = new LABSoft_Oscilloscope_Display_Group (24, 60, 640, 480);

   out  *time_per_div = new out (900, 400, 120, 60, "Time per Div");
      time_per_div->callback ((Fl_Callback*)cb_time_per_div, &win);
      time_per_div->add ("1 s");
      time_per_div->add ("2 s");
      time_per_div->add ("3 s");
      time_per_div->value("1s");
      time_per_div->activate();
    


  win.end ();
  win.resizable(disp_group->m_disp);

  win.show ();

  // Fl::add_timeout (1/30, timeout, &win);

  return Fl::run ();
}

