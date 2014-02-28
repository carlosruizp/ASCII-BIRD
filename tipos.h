/* 
 * File:   tipos.h
 * Author: carlos
 *
 * Created on October 14, 2013, 12:09 AM
 */

#ifndef TIPOS_H
#define	TIPOS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*We are going to define the several types of the most common errors*/

    typedef enum {
        OK = 0, NO_MEMORY = -1, NULL_POINTER = -2, IN_PARAM_ERR = -3, WRONG_EXPRESSION = -4
    } STATUS;

    typedef enum {
        TRUE = 1, FALSE = 0
    } BOOL;

    /*
     * NO_MEMORY: It can't be possible to reserve memory
     * NULL_POINTER: A null pointer was given as argument
     * IN_PARAM_ERR: Wrong input parameters
     */




#ifdef	__cplusplus
}
#endif

#endif	/* TIPOS_H */

