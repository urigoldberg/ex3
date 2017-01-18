/*
 * main_aux.h
 *
 *  Created on: Jan 18, 2017
 *      Author: uri
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_
#include "sp_image_proc_util.h"

//DEFINE ALL STRINGS - REQUEST
#define DIR_REQUEST "Enter images directory path:\n"
#define PRE_REQUEST "Enter images prefix:\n"
#define SUF_REQUEST "Enter images suffix:\n"
#define IMAGE_NUM_ERQUSET "Enter number of images:\n"
#define BIN_ERQUSET "Enter number of bins:\n"
#define SIFT_REQUEST "Enter number of features:\n"
#define Q_IMAGE_ERQUSET "Enter a query image or # to terminate:\n"
#define EXIT "Exiting...\n"

//DEFINE ALL STRINGS - ERRORS
#define IMAGE_NUM_ERROR "An error occurred - invalid number of images\n"
#define NUM_BIN_ERROR "An error occurred - invalid number of bins\n"
#define NUM_SIFT_ERROR "An error occurred - invalid number of features\n"

//DEFINE NUMBERS
#define STRLENGTH 1024

//FUNCTIONS
void terminate(SPPoint*** RGB_DB, SPPoint***  SIFTS_DB, int* NumOfSiftExtracted, int HowManypic,bool print);

void createAllDB(int* NumOfSiftExtracted,SPPoint*** RGB_DB, SPPoint*** SIFTS_DB, char* dir,
		char* picName, int HowManypic, char* sufName, int nBin, int nSift);

void FreeDB (SPPoint*** DB, int size);

void FreeOneArrayOfPoints (SPPoint** array);

#endif /* MAIN_AUX_H_ */
