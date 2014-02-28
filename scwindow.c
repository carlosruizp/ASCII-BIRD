#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <memory.h>
#include "scwindow.h"


/*
  -----------------------------------------------
    I N T E R N A L     F U N C T I O N S
  -----------------------------------------------
 */

/*
  Prints on the screen the style for writing on this window,
  foreground color, background color, and font style.
 */
void _prepare_font(sc_window *sc) {
    printf("%c[%d;%d;%dm", 27, sc->bg_color, sc->fg_color, sc->ch_effect);
    return;
}

/*
  Moves the cursor into a window position, translating it into
  absolute screen positions. Note the "+1" used to translate from the
  0-based library coordinates to the 1-based escape codes.
 * It adds 1 because the screen starts at position 1, 1 and we would like
 * to start at position 0, 0.
 */
void _move_to(sc_window *sc, int r, int c) {
    if (r < 0) r = 0;
    if (c < 0) c = 0;
    if (r >= sc->nr) r = sc->nr - 1;
    if (c >= sc->nc) c = sc->nc - 1;
    r += (sc->r1 + 1);
    c += (sc->c1 + 1);
    printf("%c[%d;%dH", 27, r, c);
    return;
}

/*
  Returns true if a window is visible, that is, if the structure is
  non-null, has valid values, and has at least one row and one column.
 */
static int _is_visible(sc_window *sc) {
    if (!sc) return 0;
    if (sc->r1 < 0 || sc->c1 < 0 || sc->nr <= 0 || sc->nc <= 0) return 0;
    return 1;
}

/*
  -----------------------------------------------
    P U B L I C     F U N C T I O N S
  -----------------------------------------------
 */

/*
  Creates a window on the screen. 
  Parameters:
     r1, c1: row and column where the window begins;
     nr, nc: number of rows and columns of the window.

  Returns:
    a pointer to a newly allocated sc_window structure with the 
    information for the window.

  Note that this function will do absolutely nothing on the screen
  itself. In particular, it will NOT clear the window. This function
  will simply create the structure, initialize its fields, and return
  it.
 */
sc_window *win_new(int r1, int c1, int nr, int nc, int bgcol, int fgcol, int ch_effect) {
    sc_window *src = (sc_window *) malloc(sizeof (sc_window));
    src->r1 = r1;
    src->c1 = c1;
    src->nr = nr;
    src->nc = nc;

    src->bg_color = bgcol; /* change this */
    src->fg_color = fgcol;
    src->ch_effect = ch_effect;

    return src;
}

/*
  Paints the window 
  Parameters:
 sc:the window we want to paint
 bg_color: background color
 fg color: foreground color

 */

int paint_window(sc_window *sc, int bg_color, int fg_color) {

    if (!sc)
        return 0;

    sc->bg_color = bg_color;
    sc->fg_color = fg_color;
    return 1;
}

/*
  Clears a window, leaving it all in the current background color

  Parameters:
      sc: the window structure on which the function operates.

  Returns:
      nothing.
 */
void win_cls(sc_window *sc) {
    int i = 0;
    char *buf = NULL;
    

    if (!_is_visible(sc)) return; 
    buf = (char *) malloc((sc->nc+1) * sizeof (char));
    if (!buf)
        return;
    
    for (i=0; i<sc->nc; i++)
        buf[i]=' ';
    buf[i]=0;
    
    _prepare_font(sc);
    for (i = 0; i < sc->nr; i++) {
        _move_to(sc, i, 0);
        printf("%s", buf);
        fflush(stdout);
    }

    
    free(buf);
    
    return;
}

/*
  Releases the structure allocated for a window. After calling this
  function it is no longer possible to use the window.

  This function doesn't erase the window contents from the screen.

  Parameters:
      sc: the window structure on which the function operates.

  Returns:
      nothing.
 */
void win_delete(sc_window *sc) {
    if (sc) 
        free(sc);
    return;
}

/*
  Sets the background color for the window

  Parameters:
      sc:  the window structure on which the function operates.
      col: the new background color;

  Returns:
      1: regular execution
      0: incorrect color parameter
 */
void win_bgcol(sc_window *sc, int col) {
    if (!_is_visible(sc)) return;
    if (col >= 40 && col <= 47) sc->bg_color = col;
    return;
}

/*
  Sets the foreground color for the window

  Parameters:
      sc:  the window structure on which the function operates.
      col: the new foreground color;

  Returns:
      1: regular execution
      0: incorrect color parameter
 */
void win_fgcol(sc_window *sc, int col) {
    if (!_is_visible(sc)) return;
    if (col >= 30 && col <= 37) sc->bg_color = col;
    return;
}

/*
  This function writes a string in a single line starting at a given
  position of the window, and without changing line. Each new line
  character contained in the string will be considered as a string
  termination, and if the string is too long to fit in the screen, it
  will be truncated. 

  So, if on the screen of the example we execute the functions:

  win_write_line_at(sc, 3, 26, "en un lugar de la mancha");
  win_write_line_at(sc, 5, 24, "oh!\nI didn't know");
 

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
int win_write_line_at(sc_window *sc, int r, int c, char *str) {
    char *nl_p = NULL;
    char save = 0, ret = 0;
    int av_space = 0;

    if (!_is_visible(sc)) return 0;
    if (r >= sc->nr || c >= sc->nc) {
        /*printf("Error 1");*/
        return 0;
    }
    nl_p = strchr(str, 'Q');
    if (nl_p) *nl_p = 0;

    av_space = sc->nc - c ; /**/
    save = -1;
    if (strlen(str) > av_space) {
        save = str[av_space];
        str[av_space] = 0;
    }

    _move_to(sc, r, c);
    printf("%s", str);
    fflush(stdout);
    if (save > 0) {
        str[av_space] = save;
        ret = av_space;
    } else
        ret = strlen(str);
    if (nl_p) *nl_p = 'Q';

    /*if (!ret) printf("Error 2");*/

    return ret;

}

/*
  This function writes a string on multiple, left-aligned lines
  starting at a given position of the window. Each new line character
  contained in the string will cause a new line (to be started aligned
  with the first position of the string). If a line is too long to fit
  in the screen, it will be truncated. If there are more lines than
  fit the window, the non-fitting lines will be ignored.


  So, if on the screen of the example we execute the functions:

  win_write_at(sc, 3, 26, "en un lugar de la mancha");
  win_write_at(sc, 5, 24, "oh!\nI didn't know that.\nNot quite\nNot really");
 

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

/*int win_write_at(sc_window *sc, int r, int c, char *str) {
    char *pt = NULL, *pt2=NULL;
    int num_char, cont = 0, flag=0;
    if (!_is_visible(sc)) return 0;
    if (r >= sc->nr || c >= sc->nc) return 0;

    pt = str;
    while (r < sc->nr) {
        num_char = (win_write_line_at(sc, r, c, pt));
        cont += num_char;
        r++;
        pt = strchr(pt, '\n');
        
        if((sc->nc-c)>num_char)flag=1; 
        
        if (cont < strlen(str) && flag==0) {
            if(pt2){
                pt=++pt2;
                pt2=NULL;
                cont++;
                c=0;}
            else if(pt){
                pt2=pt;       
                pt = &str[cont];                
                c = 0;}
        } 
        else if (pt) {
            pt++;
            flag=0;
            cont++;
            c=0;
        }
        else break;
    }
    return cont;
}*/

int win_write_at(sc_window *sc, int r, int c, char *str) {
  
  char *pt = NULL;  
  if (!_is_visible(sc)) return 0;
  if (r >= sc->nr || c >= sc->nc) return 0;

  _prepare_font(sc);

  pt = str;
  while(r < sc->nr) {
    win_write_line_at(sc, r, c, pt);
    r++;
    pt = strchr(pt, 'Q');
    if (pt) {
        pt++;
        c=0;/*Not aligned*/}
    else break;
  }
  return 1;
}

/*This function writes a string on multiple, left-aligned lines
  starting at a given position of the window.  If a line is too long to fit
  in the screen, it will be truncated and it will continue in the line below.*/
int write_in (sc_window *sc, int r, int c, char *str){
    int i=0, j=0;
  if (!_is_visible(sc)) return 0;
  if (r >= sc->nr || c >= sc->nc) return 0;
  
  for(i=0; i<sc->nr; i++) {
       if((sc->nc-c)>=strlen(str)) {
           return win_write_at (sc, r, c, str);
       }
       for(j=0; j<sc->nc; j++){
          if(str[sc->nc*i-c+j]=='\n'){
              
             write_in(sc, i+r+1, 0, &str[sc->nc*i-c+j+1]);
              return 1;
           }
                 
          else{
                 _move_to(sc,r+i, c+j);
                 printf("%c", str[sc->nc*i-c+j-1]);
                 fflush(stdout);
          }
          /*c=0;*/ /*Not aligned*/
      }
    }
    return 1;
}




int win_write_at_color(sc_window *sc, int r, int c, char *str, int color){
      char *pt = NULL;  
  if (!_is_visible(sc)) return 0;
  if (r >= sc->nr || c >= sc->nc) return 0;

      
  /*_prepare_font(sc);*/
  printf("%c[%d;%d;%dm", 27, color, sc->fg_color, sc->ch_effect);

  pt = str;
  while(r < sc->nr) {
    win_write_line_at(sc, r, c, pt);
    r++;
    pt = strchr(pt, '\n');
    if (pt) {
        pt++;
        c=0;/*Not aligned*/}
    else break;
  }
  return 1;
}