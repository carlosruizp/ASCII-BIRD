/* 
 * File:   scwindow.h
 * Author: Carlos Ruiz Pastor
 *
 * Created on October 13, 2013, 11:08 PM
 */

#ifndef SCWINDOW_H
#define	SCWINDOW_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*

  This is the structure used to store the information about a window:
  this is the structure returned by the function that creates a new
  window, and must be passed as a parameter to all functions that
  write in the window.
*/
typedef struct {
  int  c1;     /* column on the screen where the window begins */
  int  r1;     /* row on the screen where the window begins */
  int  nr;     /* number of rows in the window  */
  int  nc;     /* number of column in the window  */

  int  crs_r;  /* last position of the cursor in this window */
  int  crs_c;  /* last position of the cursor in this window */

  int  bg_color; /* the background color for this window */
  int  fg_color; /* the foreground color for this window */
  int  ch_effect; /* the character effect for this window */
} sc_window;



void _prepare_font(sc_window *sc);


void _move_to(sc_window *sc, int r, int c);

/*
  Creates a window on the screen. 
  Parameters:
     r1, c2: row and column where the window begins;
     nr, nc: number of rows and columns of the window.

  Returns:
    a pointer to a newly allocated sc_window structure with the 
    information for the window.

  Note that this function will do absolutely nothing on the screen
  itself. In particular, it will NOT clear the window. This function
  will simply create the structure, initialize its fields, and return
  it.
*/
sc_window *win_new(int r1, int c1, int nr, int nc, int bgcol, int fgcol, int ch_effect);

/*
  Paints the window 
  Parameters:
 sc:the window we want to paint
 bg_color: background color
 fg color: foreground color

*/

int paint_window (sc_window *sc, int bg_color, int fg_color);


/*
  Clears a window, leaving it all in the current background color

  Parameters:
      sc: the window structure on which the function operates.

  Returns:
      nothing.
*/
void win_cls(sc_window *sc);

/*
  Releases the structure allocated for a window. After calling this
  function it is no longer possible to use the window.

  This function doesn't erase the window contents from the screen.

  Parameters:
      sc: the window structure on which the function operates.

  Returns:
      nothing.
*/
void win_delete(sc_window *sc);

/*
  Sets the background color for the window

  Parameters:
      sc:  the window structure on which the function operates.
      col: the new background color;

  Returns:
      1: regular execution
      0: incorrect color parameter
*/
void win_bgcol(sc_window *sc, int col);


/*
  Sets the foreground color for the window

  Parameters:
      sc:  the window structure on which the function operates.
      col: the new foreground color;

  Returns:
      1: regular execution
      0: incorrect color parameter
*/
void win_fgcol(sc_window *sc, int col);


/*
  This function writes a string in a single line starting at a given
  position of the window, and without changing line. Each new line
  character contained in the string will be considered as a string
  termination, and if the string is too long to fit in the screen, it
  will be truncated. 

  So, if on the screen of the example we execute the functions:

  win_write_line_at(sc, 1, 7, "en un lugar de la mancha");
  win_write_line_at(sc, 3, 5, "oh!\nI didn't know");
 

  we get:

                       19             33
     0                  v             v    39
    +----------------------------------------+
  0 |                                        |
    |                                        |
    |                          en un lu      | < 2
    |                                        | 
    |                        oh!             |
    |                                        |
    |                                        | < 6
    |                                        | 
    |                                        |
   9|                                        |
    +----------------------------------------+

    Parameters:
      sc:  the window structure on which the function operates.
      r:   the row of the window at which the writing begins
      c:   the column of the window at which the writing begins
      str: the screen that we must write

    Returns:
      the number of characters actually written on the screen.

*/
int win_write_line_at(sc_window *sc, int r, int c, char *str);

/*
  This function writes a string on multiple, left-aligned lines
  starting at a given position of the window. Each new line character
  contained in the string will cause a new line to be started aligned
  with the first position of the string. If a line is too long to fit
  in the screen, it will be truncated. If there are more lines than
  fit the window, the non-fitting lines will be ignored.


  So, if on the screen of the example we execute the functions:

  win_write_at(sc, 1, 7, "en un lugar de la mancha");
  win_write_at(sc, 3, 5, "oh!\nI didn't know that.\nNot quite\nNot really");
 

  we get:

                       19             33
     0                  v             v    39
    +----------------------------------------+
  0 |                                        |
    |                                        |
    |                          en un lu      | < 2
    |                                        | 
    |                        oh!             |
    |                        I didn't k      |
    |                        Not quite       | < 6
    |                                        | 
    |                                        |
   9|                                        |
    +----------------------------------------+

    Parameters:
      sc:  the window structure on which the function operates.
      r:   the row of the window at which the writing begins
      c:   the column of the window at which the writing begins
      str: the screen that we must write

    Returns:
      The number of characters actually written on the screen.

*/
int win_write_at(sc_window *sc, int r, int c, char *str);

/*It cuts the string and continues it in the line below*/

int write_in(sc_window *sc, int r, int c, char *str);





int win_write_at_color(sc_window *sc, int r, int c, char *str, int color);






#ifdef	__cplusplus
}
#endif

#endif	/* SCWINDOW_H */

