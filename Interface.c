#include "Interface.h"
#include "scwindow.h"
#include <stdio.h>
#include <stdlib.h>
#include "tipos.h"
#include <string.h>

/*This function creates a new interface
 *Parameters:
 * File *f: A pointer to the file where the characteristics of the windows are written.
 * Returns: A pointer to interface
 */
INTERF* createInterface(FILE *f, FILE *g) {
    INTERF *interf = NULL;
    sc_window **window = NULL;

    int r1, c1, nr, nc, bgcol, fgcol, ch_effect, i, j, num_win, win_act, error;

    if (f == NULL || g == NULL) return NULL;
    interf = (INTERF*) malloc(sizeof (INTERF));

    if (interf == NULL) return NULL;

    if (fscanf(f, "%d %d\n", &num_win, &win_act) == 0) {
        free(interf);
        return NULL;
    }/*Reading the number of windows*/

    window = (sc_window**) malloc(sizeof (sc_window*) * num_win);

    if (window == NULL) {
        free(interf);
        return NULL;
    }

    for (i = 0; i < num_win; i++) {
        if (fscanf(f, "%d %d %d %d %d %d %d\n", &r1, &c1, &nr, &nc, &bgcol, &fgcol, &ch_effect) == 0) {
            for (j = 0; j < i; j++) {
                win_delete(window[j]);
            }
            free(window);
            free(interf);
            return NULL;
        }
        window[i] = win_new(r1, c1, nr, nc, bgcol, fgcol, ch_effect);
        if (window[i] == NULL) {
            for (j = 0; j < i; j++) {
                win_delete(window[j]);
            }
            free(window);
            free(interf);
            return NULL;
        }


    }
    if (fscanf(f, "%d %d %d %d", &interf->scr_row, &interf->scr_col, &interf->n_row, &interf->n_col) == 0) {
        for (j = 0; j < i; j++) {
            win_delete(window[j]);
        }
        free(window);
        free(interf);
        return NULL;
    }
    interf->window = window;
    interf->n_window = num_win;
    interf->in_window = interf->window[win_act];

    error = change_interf_design(interf, g);
    if (error < 0) {
        for (j = 0; j < num_win; j++) {
            free(window[j]);
        }
        free(window);
        free(interf);
        return NULL;
    }

    for (j = 0; j < num_win; j++)
        _interf_prepare_font(interf, j);

    return interf;
}

/*printf("%c[8;r;ct",27);*/

/*This function sets the input window
 Parameters:
 interf: Interface that we want to modify
 win_number: the window that we will set as the input one*/
void set_input_window(INTERF * interf, int win_number) {
    if (interf == NULL || win_number < 0 || win_number >= interf->n_window)
        return;
    interf->in_window = interf->window[win_number];
    return;
}

/*
 * This function changes the matrix with the whole interface design
 * Parameters:
 * - interf: The interface that is going to be modified
 * - f: The file where the design is written 
 */
int change_interf_design(INTERF * interf, FILE * f) {
    int i, j;
    char ** string;
    char buf[MAX_LINE_NUM], *pt;
    int r, c;
    if (f == NULL || interf == NULL) return NULL_POINTER;
    if ((fgets(buf, MAX_LINE_NUM, f)) == NULL) return IN_PARAM_ERR;
    r = atoi(buf);
    pt = strchr(buf, ' ');
    if (pt == NULL) return IN_PARAM_ERR;
    c = atoi(pt + 1);

    string = (char**) malloc(r * sizeof (char*));
    if (string == NULL) return NO_MEMORY;

    for (i = 0; i < r; i++) {
        string[i] = (char*) malloc(MAX_LINE_NUM * sizeof (char));
        if (string[i] == NULL) {
            for (j = 0; j < i; j++)
                free(string[j]);
            free(string);
            return NO_MEMORY;
        }
        fgets(string[i], MAX_LINE_NUM, f);
        if (strlen(string[i]) > 0)
            string[i][strlen(string[i]) - 1] = 0;
    }

    interf->draw = (char**) malloc(r * sizeof (char*));
    if (!interf->draw) {
        for (j = 0; j < i; j++)
            free(string[j]);
        free(string);
        return NO_MEMORY;
    }
    for (i = 0; i < r; i++) {
        interf->draw[i] = (char*) malloc(MAX_LINE_NUM * sizeof (char));
        if (interf->draw[i] == NULL) {
            for (j = 0; j < i; j++) {
                free(interf->draw[j]);
                free(string[j]);
            }
            free(string);
            free(interf->draw);
            return NO_MEMORY;
        }
        strcpy(interf->draw[i], string[i]);
    }

    /*Libero las variables auxiliares*/
    for (j = 0; j < r; j++) {
        free(string[j]);
    }
    free(string);
    return OK;
}

/*This function writes a sentence in a window of our interface
parametres:
win_number: the window where we want to write
row: the row we want to start writing
column: the column we want to start writing
phrase: the phrase we want to write
returns nothing */

void write_interf(INTERF * interf, int win_number, int row, int column, char * phrase) {
    if (interf == NULL || phrase == NULL || win_number < 0 || win_number >= interf->n_window || row < 0 || column < 0)
        return;
    if ((win_write_at(interf->window[win_number], row, column, phrase)) == 0) {
        /*printf("ERROR!!!");*/       
        return;
    }
    interf->in_window = interf->window[win_number];
    return;
}

/*This function writes a sentence in a window of our interface
parametres:
win_number: the window where we want to write
row: the row we want to start writing
column: the column we want to start writing
phrase: the phrase we want to write
returns nothing */
void write_interf_color(INTERF * interf, int win_number, int row, int column, char * phrase, int color) {
    if (interf == NULL || phrase == NULL || win_number < 0 || win_number >= interf->n_window || row < 0 || column < 0)
        return;
    if ((win_write_at_color(interf->window[win_number], row, column, phrase, color)) == 0) {        
        return;
    }
    interf->in_window = interf->window[win_number];
    return;
}

/*This functions clears a window.
 Parameters:
 interf: The interface where is the windows we want to clear
 win_number: The window that we want to clear
 * returns nothing */

void clear_interf(INTERF * interf, int win_number) {
    if (interf == NULL || win_number < 0 || win_number >= interf->n_window)
        return;
    win_cls(interf->window[win_number]);
    return;
}

/*This function clears all the windows on the interface
 Parameters:
 interf: The interface we want to clear
 returns nothing*/
void master_clear_interf(INTERF * interf) {
    int i;
    if (interf == NULL)
        return;
    for (i = 0; i < interf->n_window; i++) {
        win_cls(interf->window[i]);
    }
}

/*
 *This function destroys a window of the interface
 * (Input)Parameters:
 * - Interf * interf: The interface that contains the window
 * - int win_number: Number in the array of the window we want to destroy
 * returns OK or an error.
 */
int destroy_window(INTERF * interf, int win_number) { /*Buena*/
    if (interf == NULL || win_number < 0)
        return IN_PARAM_ERR;
    win_delete(interf->window[win_number]);
    return OK;
}

/*
 *This function destroys the interface
 * (Input)Parameters:
 * - Interf * interf: The interface to destroy
 * returns OK or an error
 */
int destroy_interf(INTERF* interf) { /*Buena*/
    int i;
    if (interf == NULL) return NULL_POINTER;
    for (i = 0; i < interf->n_window; i++) {
        destroy_window(interf, i);
    }
    free(interf->window);
    destroy_draw(interf);
    free(interf);
    return OK;
}

void destroy_draw(INTERF* interf) { /*Buena*/
    int i;
    if (interf == NULL) return;
    for (i = 0; i < interf->n_row; i++) {
        free(interf->draw[i]);
    }
    free(interf->draw);
}

/*It prints the draw** from the structure interf on the screen*/
int print_design(INTERF* interf) {
    int i;
    int r, c;
    if (interf == NULL)
        return NULL_POINTER;


    printf("%c[%d;%dH", 27, interf->scr_row, interf->scr_col);
    printf("%c[%d;%d;%dm", 27, 40, 30, 1);
    
    r=interf->scr_row;
    c=interf->scr_col;
    r++;
    for (i = 0; i < interf->n_row; i++, r++) {
        printf("%c[%d;%dH", 27, r, c);
        printf("%s", interf->draw[i]);
        /*printf("\n");*/
        fflush(stdout);
    }
    return OK;
}

/*It reads a string and write it in the window we hace selected.
interf: the interface we want to modify
no: the window where we want to write
r: the row where we want to wrinte
c: the column where we want to start wrinting
returns: the string we hace written*/
char * get_string(INTERF *interf, int no, int r, int c) {
    char buf[200];
    char* str=NULL;
    win_cls(interf->window[no]);
    _move_to(interf->window[no], r, c);
    printf("> ");
    fflush(stdout);
    fgets(buf, interf->window[no]->nc, stdin);
    if (strlen(buf) > 1) {
        buf[strlen(buf) - 1] = 0;
    }
    str=(char*)malloc(strlen(buf)+1);
    strcpy(str, buf);
    return str;
    /*return strdup(buf);*/
}

/*This function set the fgcolor of a window in our interface
interf: the interface we want to modify
win_number: the window we want to paint
col: the new color of thw window*/
void set_interf_fgcolor(INTERF *interf, int win_number, int col) {
    win_fgcol(interf->window[win_number], col);
    return;

}

/*This function set the bgcolor of a window in our interface
interf: the interface we want to modify
win_number: the window we want to paint
col: the new color of the window*/
void set_interf_bgcolor(INTERF *interf, int win_number, int col) {
    win_bgcol(interf->window[win_number], col);
    return;

}

/*
  Prints on the screen the style for writing on this window,
  foreground color, background color, and font style.
 */
void _interf_prepare_font(INTERF* interf, int win_number) {
    _prepare_font(interf->window[win_number]);
    return;

}

int load_map(INTERF * interf, int win_number, FILE *f) {
    int i;
    if (f == NULL)
        return IN_PARAM_ERR;

    rewind(f);
    if (interf == NULL)
        return NULL_POINTER;

    char * buf = NULL;
    buf = (char*) malloc(MAX_LINE_NUM * sizeof (char));

    if (!buf)
        return NO_MEMORY;

    for (i = 0; i < interf->window[win_number]->nr; i++) {
        fgets(buf, MAX_LINE_NUM, f);
        buf[strlen(buf) - 1] = 0;
        write_interf(interf, win_number, i, 0, buf);
    }

    free(buf);
    return OK;
}

int load_map2(INTERF * interf, int win_number, int row, int column, FILE *f) {
    int i;

    if (f == NULL)
        return IN_PARAM_ERR;

    rewind(f);
    if (interf == NULL)
        return NULL_POINTER;

    char * buf = NULL;
    buf = (char*) malloc(MAX_LINE_NUM * sizeof (char));

    if (!buf)
        return NO_MEMORY;

    for (i = row; i < interf->window[win_number]->nr && !feof(f); i++) {
        fgets(buf, MAX_LINE_NUM, f);
        if (strlen(buf) > 1) {
            buf[strlen(buf) - 1] = 0;
            write_interf(interf, win_number, i, column, buf);

        }


    }

    free(buf);
    return OK;
}

int load_map3(INTERF * interf, int win_number, int row, int column, FILE * fmap, FILE * fcolors) {
    int color;
    int window_color=interf->window[win_number]->bg_color;
    char Aux;
    int i, j;
    char * buf;
    if (win_number > interf->n_window - 1)
        return IN_PARAM_ERR;

    if (fmap == NULL || fcolors == NULL)
        return IN_PARAM_ERR;
    rewind(fmap);
    rewind(fcolors);

    if (interf == NULL)
        return NULL_POINTER;

    buf = NULL;
    buf = (char*) malloc((interf->window[win_number]->nc) * sizeof (char));

    if (!buf)
        return NO_MEMORY;

    for (i = column, j=row; j < interf->window[win_number]->nr /*&& !feof(fmap) && !feof(fcolors)*/; i++) {
        fscanf(fmap, "%c", &Aux);
        fscanf(fcolors, "%d", &color);
        if(Aux=='\n'){
            i=column-1;
            j++;
        }
        else{
        
        strcpy(buf, "aa");    
        buf[0] = Aux;
        if (strlen(buf) > 1) {
            buf[strlen(buf) - 1] = 0;
        }
        write_interf_color(interf, win_number, j, i, buf, color);

    }

    }
    interf->window[win_number]->bg_color=window_color;
    free(buf);
    return OK;
}
