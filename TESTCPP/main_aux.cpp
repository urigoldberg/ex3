#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main_aux.h"


void FreeOneArrayOfPoints (SPPoint** array){
	int size = (int)(sizeof(array)/sizeof(SPPoint*));
	for (int i = 0; i < size; i++)
		spPointDestroy(array[i]);
	free(array);
}

//Free all Elements in DB then free DB itself
//DB is an array of arrays of SPPoints

void FreeDB (SPPoint*** DB, int size) {
	for (int j = 0; j < size; j++)
		FreeOneArrayOfPoints(DB[j]);
	free(DB);
}


void createAllDB(int* NumOfSiftExtracted,SPPoint*** RGB_DB, SPPoint*** SIFTS_DB, char* dir,
		char* picName, int HowManypic, char* sufName, int nBin, int nSift) {


	/** Allocate 2 Data-Bases
	 *  RGB_DB -> Data Base for RGB_histograms
	 *  SIFTS_DB -> Data Base for SIFT
	 */

	RGB_DB = (SPPoint***) malloc(sizeof(SPPoint**) * HowManypic);
	if (RGB_DB == NULL) {
		return;
	}

	SIFTS_DB = (SPPoint***) malloc(sizeof(SPPoint**) * HowManypic);
	if (SIFTS_DB == NULL) {
		free(RGB_DB);
		return;
	}

	//initialize array of ints for number of sifts extracted from each image
	NumOfSiftExtracted = (int*) malloc(sizeof(int)*HowManypic);
	if (NumOfSiftExtracted == NULL) {
		free(SIFTS_DB);
		free(RGB_DB);
		return;
		}

	//Inserts values to DB for each image
	for (int i = 0; i < HowManypic; i++) {

		//Concate path
		char src[STRLENGTH];
		char C = i +'0';
		strcpy(src, dir);
		strcat(src, picName);
		strcat(src, &C );
		strcat(src, sufName);

		RGB_DB[i] = spGetRGBHist(src,i,nBin);

		//Allocation failed
		if (RGB_DB[i] == NULL) {
			FreeDB(RGB_DB,i);
			FreeDB(SIFTS_DB,i);
			free(NumOfSiftExtracted);
			return;
		}

		SIFTS_DB[i] = spGetSiftDescriptors(src,i,nSift,&NumOfSiftExtracted[i]);
		//Allocation failed
		if (SIFTS_DB[i] == NULL) {
			//we have one more in RGB
			FreeDB(RGB_DB,i+1);
			FreeDB(SIFTS_DB,i);
			free(NumOfSiftExtracted);
			return;
			}
	}

}
void terminate(SPPoint*** RGB_DB, SPPoint***  SIFTS_DB, int* NumOfSiftExtracted, int HowManypic,bool print) {
	FreeDB(RGB_DB,HowManypic);
	FreeDB(SIFTS_DB,HowManypic);
	free(NumOfSiftExtracted);
	if (print)
		printf(EXIT);
	return;
}


void print_5_GlobalFeatures ( SPPoint** RBG_HIST ,SPPoint*** RGB_DB, int HowManypic) {

}
