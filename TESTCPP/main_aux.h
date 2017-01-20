/*
 * main_aux.h
 *
 *  Created on: Jan 18, 2017
 *      Author: uri
 */

#ifndef MAIN_AUX_H_
#define MAIN_AUX_H_
#include "sp_image_proc_util.h"
extern "C" {
#include "SPBPriorityQueue.h"
}
//DEFINE ALL STRINGS - REQUEST & MESSAMGES
#define DIR_REQUEST "Enter images directory path:\n"
#define PRE_REQUEST "Enter images prefix:\n"
#define SUF_REQUEST "Enter images suffix:\n"
#define IMAGE_NUM_ERQUSET "Enter number of images:\n"
#define BIN_ERQUSET "Enter number of bins:\n"
#define SIFT_REQUEST "Enter number of features:\n"
#define Q_IMAGE_ERQUSET "Enter a query image or # to terminate:\n"
#define EXIT "Exiting...\n"
#define PRINT_BY_GLOBAL "Nearest images using global descriptors:\n"
#define PRINT_BY_LOCAL "Nearest images using local descriptors:\n"

//DEFINE ALL STRINGS - ERRORS
#define IMAGE_NUM_ERROR "An error occurred - invalid number of images\n"
#define NUM_BIN_ERROR "An error occurred - invalid number of bins\n"
#define NUM_SIFT_ERROR "An error occurred - invalid number of features\n"
#define MEM_PROBLEMS "An error occurred - allocation failure\n"

//DEFINE NUMBERS
#define STRLENGTH 1024
#define HOWMANYCLOSE 5

//FUNCTIONS
/**
 * terminate - gets the 2 DBs and 1 array we created and destroys them. gets how many pic in
 * order to destroy them. if bool == true print exiting. else - dont print
 *
 */
void terminate(SPPoint*** RGB_DB, SPPoint*** SIFTS_DB, int* NumOfSiftExtracted,
		int HowManypic, bool print);

void createAllDB(int* NumOfSiftExtracted, SPPoint*** RGB_DB,
		SPPoint*** SIFTS_DB, char* dir, char* picName, int HowManypic,
		char* sufName, int nBin, int nSift);

void FreeDB(SPPoint*** DB, int size);

void FreeOneArrayOfPoints(SPPoint** array);

int print_5_GlobalFeatures(SPPoint** RBG_HIST, SPPoint*** RGB_DB,
		int HowManypic);

int print_5_LocalFeatures(SPPoint** SIFTS_Query, SPPoint*** SIFTS_DB,int* NumOfSiftExtracted, int howManySiftsQuery,
		int HowManypic);

int cmp (const void* num1, const void* num2);

int min (int num1, int num2);

//concate string b at the end of string a
void Sconcate (char* a, char* b);

#endif /* MAIN_AUX_H_ */
