//
// "$Id$"
//
//	Simple example of an interactive spreadsheet using Fl_Table.
//	Uses Mr. Satan's technique of instancing an Fl_Input around.
//
// Copyright 1998-2010 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//
#include <stdio.h>
#include <stdlib.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>

const int MAX_COLS = 10;
const int MAX_ROWS = 10;

class LABChecker_GUI_Digital_Input_Table : public Fl_Table 
{
  private:
    Fl_Input* input;					// single instance of Fl_Int_Input widget
    int values[MAX_ROWS][MAX_COLS];			// array of data for cells
    int row_edit, col_edit;				// row/col being modified

    unsigned m_number_of_rows = 2;
    unsigned m_number_of_cols = 4;

protected:
  void draw_cell(TableContext context,int=0,int=0,int=0,int=0,int=0,int=0);
  void event_callback2();				// table's event callback (instance)


  static void event_callback(Fl_Widget*,void *v) {	// table's event callback (static)
    ((LABChecker_GUI_Digital_Input_Table*)v)->event_callback2();
  }
  static void input_cb(Fl_Widget*,void* v) {		// input widget's callback
    ((LABChecker_GUI_Digital_Input_Table*)v)->set_value_hide();
  }

public:
  LABChecker_GUI_Digital_Input_Table (int X, int Y, int W, int H, const char* L = 0) 
    : Fl_Table (X, Y, W, H, L) 
  {
    when (FL_WHEN_NOT_CHANGED | when ());

    callback (&event_callback, static_cast<void*>(this));    


    // Create input widget that we'll use whenever user clicks on a cell
    input = new Fl_Input (W/2, H/2, 0, 0);
    input->hide         ();
    input->callback     (input_cb, static_cast<void*>(this));
    input->when         (FL_WHEN_ENTER_KEY_ALWAYS);		// callback triggered when user hits Enter
    input->maximum_size (1);
    input->color        (FL_YELLOW);

    for (int c = 0; c < MAX_COLS; c++)
      for (int r = 0; r < MAX_ROWS; r++)
	values[r][c] = c + (r*MAX_COLS);		// initialize cells
    end();
    row_edit = col_edit = 0;
    set_selection(0,0,0,0);

    init_table ();
  }
  ~LABChecker_GUI_Digital_Input_Table() { }

  void init_table ()
  {
    row_header        (1);
    row_header_width  (70);
    row_resize        (0);
    rows              (2);
    row_height_all    (25);

    col_header        (1);
    col_header_height (25);
    col_resize        (0);
    cols              (4);
    col_width_all     (70);

    tooltip ("Use keyboard to navigate cells:\nArrow keys or Tab/Shift-Tab");
  }

  // Apply value from input widget to values[row][col] array and hide (done editing)
  void set_value_hide() {
    values[row_edit][col_edit] = atoi(input->value());
    input->hide();
    window()->cursor(FL_CURSOR_DEFAULT);		// XXX: if we don't do this, cursor can disappear!
  }
  // Start editing a new cell: move the Fl_Int_Input widget to specified row/column
  //    Preload the widget with the cell's current value,
  //    and make the widget 'appear' at the cell's location.
  //
  void start_editing(int R, int C) {
    row_edit = R;					// Now editing this row/col
    col_edit = C;
    set_selection(R,C,R,C);				// Clear any previous multicell selection
    int X,Y,W,H;
    find_cell(CONTEXT_CELL, R,C, X,Y,W,H);		// Find X/Y/W/H of cell
    input->resize(X,Y,W,H);				// Move Fl_Input widget there
    char s[30]; sprintf(s, "%d", values[R][C]);		// Load input widget with cell's current value
    input->value(s);
    input->position(0,strlen(s));			// Select entire input field
    input->show();					// Show the input widget, now that we've positioned it
    input->take_focus();
  }
  // Tell the input widget it's done editing, and to 'hide'
  void done_editing() {
    if (input->visible()) {				// input widget visible, ie. edit in progress?
      set_value_hide();					// Transfer its current contents to cell and hide
    }
  }
  // Return the sum of all rows in this column
  int sum_rows(int C) {
    int sum = 0;
    for (int r=0; r<rows()-1; ++r)			// -1: don't include cell data in 'totals' column
      sum += values[r][C];
    return(sum);
  }
  // Return the sum of all cols in this row
  int sum_cols(int R) {
    int sum = 0;
    for (int c=0; c<cols()-1; ++c)			// -1: don't include cell data in 'totals' column
      sum += values[R][c];
    return(sum);
  }
  // Return the sum of all cells in table
  int sum_all() {
    int sum = 0;
    for (int c=0; c<cols()-1; ++c)			// -1: don't include cell data in 'totals' column
      for (int r=0; r<rows()-1; ++r)			// -1: ""
	sum += values[r][c];
    return(sum);
  }
};

// Handle drawing all cells in table
void LABChecker_GUI_Digital_Input_Table::draw_cell(TableContext context, int R,int C, int X,int Y,int W,int H) 
{
  static char s[30]; 



  switch (context)
  {
    case CONTEXT_STARTPAGE:			// table about to redraw
      break;

    case CONTEXT_COL_HEADER:			// table wants us to draw a column heading (C is column)
      fl_font(FL_HELVETICA | FL_BOLD, 14);	// set font for heading to bold
      fl_push_clip(X,Y,W,H);			// clip region for text
      {
	fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, col_header_color());
	fl_color(FL_BLACK);
	if (C == cols()-1) {			// Last column? show 'TOTAL'
	  fl_draw("TOTAL", X,Y,W,H, FL_ALIGN_CENTER);
	} else {				// Not last column? show column letter
	  sprintf(s, "%c", 'A' + C);
	  fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
	}
      }
      fl_pop_clip();
      return;

    case CONTEXT_ROW_HEADER:			// table wants us to draw a row heading (R is row)
      fl_font(FL_HELVETICA | FL_BOLD, 14);	// set font for row heading to bold
      fl_push_clip(X,Y,W,H);
      {
	fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, row_header_color());
	fl_color(FL_BLACK);
	if (R == rows()-1) {			// Last row? Show 'Total'
	  fl_draw("TOTAL", X,Y,W,H, FL_ALIGN_CENTER);
	} else {				// Not last row? show row#
	  sprintf(s, "%d", R+1);
	  fl_draw(s, X,Y,W,H, FL_ALIGN_CENTER);
	}
      }
      fl_pop_clip();
      return;

    case CONTEXT_CELL: {	
      		// table wants us to draw a cell
      if (R == row_edit && C == col_edit && input->visible()) {
	return;					// dont draw for cell with input widget over it
      }
      // Background
      if ( C < cols()-1 && R < rows()-1 ) {
	fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, is_selected(R,C) ? FL_YELLOW : FL_WHITE);
      } else {
	fl_draw_box(FL_THIN_UP_BOX, X,Y,W,H, is_selected(R,C) ? 0xddffdd00 : 0xbbddbb00);	// money green
      }
      // Text
      fl_push_clip(X+3, Y+3, W-6, H-6);
      {
	fl_color(FL_BLACK); 
	if (C == cols()-1 || R == rows()-1) {	// Last row or col? Show total
	  fl_font(FL_HELVETICA | FL_BOLD, 14);	// ..in bold font
	  if (C == cols()-1 && R == rows()-1) {	// Last row+col? Total all cells
	    sprintf(s, "%d", sum_all());
	  } else if (C == cols()-1) {		// Row subtotal
	    sprintf(s, "%d", sum_cols(R));
	  } else if (R == rows()-1) {		// Col subtotal
	    sprintf(s, "%d", sum_rows(C));
	  }
	  fl_draw(s, X+3,Y+3,W-6,H-6, FL_ALIGN_RIGHT);
	} else {				// Not last row or col? Show cell contents
	  fl_font(FL_HELVETICA, 14);		// ..in regular font
	  sprintf(s, "%d", values[R][C]);
	  fl_draw(s, X+3,Y+3,W-6,H-6, FL_ALIGN_RIGHT);
	}
      }
      fl_pop_clip();
      return;
    }


    case CONTEXT_RC_RESIZE:			// table resizing rows or columns
      if ( input->visible() ) {
        find_cell(CONTEXT_TABLE, row_edit, col_edit, X, Y, W, H);
        input->resize(X,Y,W,H);
        init_sizes();
      }
      return;

    default:
      return;
  }
}

// Callback whenever someone clicks on different parts of the table
void LABChecker_GUI_Digital_Input_Table::event_callback2() {
  int R = callback_row();
  int C = callback_col();
  TableContext context = callback_context(); 

  switch ( context ) {
    case CONTEXT_CELL: {				// A table event occurred on a cell
      switch (Fl::event()) { 				// see what FLTK event caused it
	case FL_PUSH:					// mouse click?
	  done_editing();				// finish editing previous
	  if (R != rows()-1 && C != cols()-1 )		// only edit cells not in total's columns
	    start_editing(R,C);				// start new edit
	  return;

	case FL_KEYBOARD:				// key press in table?
	  if ( Fl::event_key() == FL_Escape ) exit(0);	// ESC closes app
	  done_editing();				// finish any previous editing
	  if (C==cols()-1 || R==rows()-1) return;	// no editing of totals column
	  switch ( Fl::e_text[0] ) {
	    case '0': case '1': case '2': case '3':	// any of these should start editing new cell
	    case '4': case '5': case '6': case '7':
	    case '8': case '9': case '+': case '-':
	      start_editing(R,C);			// start new edit
	      input->handle(Fl::event());		// pass typed char to input
	      break;
	    case '\r': case '\n':			// let enter key edit the cell
	      start_editing(R,C);			// start new edit
	      break;
	  }
	  return;
      }
      return;
    }

    case CONTEXT_TABLE:					// A table event occurred on dead zone in table
    case CONTEXT_ROW_HEADER:				// A table event occurred on row/column header
    case CONTEXT_COL_HEADER:
      done_editing();					// done editing, hide
      return;
    
    default:
      return;
  }
}

int main() 
{
  Fl_Double_Window *win = 
    new Fl_Double_Window (862, 322, "LABChecker_GUI_Digital_Input_Table");
  
  LABChecker_GUI_Digital_Input_Table *table = 
    new LABChecker_GUI_Digital_Input_Table (10, 10, win->w()-20, win->h()-20);

  #if FLTK_ABI_VERSION >= 10303
    table->tab_cell_nav(1);		// enable tab navigation of table cells (instead of fltk widgets)
  #endif
  win->end        ();
  win->resizable  (table);
  win->show       ();

  return (Fl::run ());
}