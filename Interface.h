/* 
 * File:   Interface.h
 * Author: e280270
 *
 * Created on 3 de octubre de 2013, 19:22
 */

#ifndef INTERFACE_H
#define	INTERFACE_H


#include <stdio.h>
#include "scwindow.h"
#include "tipos.h"
#include <string.h>
#define MAX_LINE_NUM 300

typedef struct{
    int scr_row;
    int n_row;
    int scr_col;
    int n_col;
    int n_window; /*Number of windows inside the interface*/
    sc_window **window; /*Array of windows*/
    sc_window *in_window; /*Pointer to the active window*/
    char **draw; /*Matrix with the whole interface design*/
}INTERF;

INTERF* createInterface(FILE *f, FILE *g);
void set_input_window(INTERF * interf, int win_number);
int change_interf_design(INTERF * interf, FILE * f) ;
void write_interf(INTERF * interf, int win_number, int row, int column, char * phrase);
void write_interf_color(INTERF * interf, int win_number, int row, int column, char * phrase, int color);
void clear_interf(INTERF * interf, int win_number);
void master_clear_interf(INTERF * interf);
int destroy_window(INTERF * interf, int win_number);
int destroy_interf(INTERF* interf);
void destroy_draw(INTERF* interf);
int print_design(INTERF* interf);

char *get_string(INTERF *interf, int no, int r, int c);
void set_interf_fgcolor (INTERF *interf, int win_number, int col);
void set_interf_bgcolor (INTERF *interf, int win_number, int col);
void _interf_prepare_font(INTERF*interf, int win_number);

int load_map (INTERF * interf, int win_number, FILE *f);
int load_map2 (INTERF * interf, int win_number, int row, int column, FILE *f);
int load_map3(INTERF * interf, int win_number, int row, int column, FILE * fmap, FILE * fcolors);

#endif	/* INTERFACE_H */

