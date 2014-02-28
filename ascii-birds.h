/** 
 * @brief   A little game for linux.
 * 
 * This modules contains the prototypes of the functions that
 * control the game playing. 
 * @file ascii-birds.h
 * @author Carlos Ruiz Pastor
 * @version 1.0
 * @date 16-02-2014
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include "Interface.h"

#ifndef CAVERUNNER_H
#define	CAVERUNNER_H


 #define NUM_PIPES 6
 #define PIPE_TAM 17
 #define BIRD_POS 20
 #define BIRD_TAM 4
 #define BIRD_LONG 10

typedef struct {
	short height;
	short mode;
	short fall;
} BIRD;

typedef struct {
	short number;
	short min;
	short max;
	short position;
} PIPE;

/* Necessary functions*/
char getch();
int aleat_num(int inf, int sup);

/* Game functions*/
int ini_bird();
int play_AsciiBirds(INTERF * interf);
int ini_pipes();
int calculate_next();
int copy_pipes();
int calculate_next_u(int i);
int copy_pipes_u(int i);
void * pipes_function(void * pointer);
PIPE * actual_pipe();
int go_up();
void * free_fall(void * pointer);
int crash();

/* Printing*/
int print_pipe(INTERF * interf, PIPE * pipe);
int print_bird(INTERF * interf);
int erase_pipe(INTERF * interf, PIPE * pipe);
int erase_bird(INTERF * interf);
int erase_first(INTERF * interf);




#endif	/* CAVERUNNER_H */