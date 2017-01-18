#include <stdio.h>
#include "sp_image_proc_util.h"
#include "main_aux.h"


int main() {

	//Ask for PATH
	printf(DIR_REQUEST);
	char dir[STRLENGTH];
	fgets(dir, STRLENGTH, stdin);

	//Ask for PREFIX
	printf(PRE_REQUEST);
	char picName[STRLENGTH];
	fgets(picName, STRLENGTH, stdin);

	//Ask for NUM OF PICTURES
	printf(IMAGE_NUM_ERQUSET);
	int HowManypic;
	scanf("%d", &HowManypic);

	//Check if the user entered an integer that is less than 1
	if (HowManypic < 1) {
		printf(IMAGE_NUM_ERROR);
		return 0;
	}

	//Ask for SUFFIX
	printf(SUF_REQUEST);
	char sufName[STRLENGTH];
	fgets(sufName, STRLENGTH, stdin);


	//Ask for BINS
	printf(BIN_ERQUSET);
	int nBin;
	scanf("%d",&nBin);

	//Check if the user entered a number that is less than 1 or greater than 255
	if ((nBin < 1)||(nBin > 255)) {
		printf(NUM_BIN_ERROR);
		return 0;
		}

	//Ask for SIFTS
	printf(SIFT_REQUEST);
	int nSift;
	scanf("%d",&nSift);

	//Check if the user entered a number that is less than 1 or greater than 255
	if (nSift < 1) {
		printf(NUM_SIFT_ERROR);
		return 0;
		}

	//Create DBs
	SPPoint*** RGB_DB = NULL;
	SPPoint***  SIFTS_DB = NULL;
	int* NumOfSiftExtracted = NULL;

	//Initialize DBs and inserts values
	createAllDB(NumOfSiftExtracted, RGB_DB, SIFTS_DB, dir,
			picName, HowManypic, sufName, nBin, nSift);

	//Initialize DBs failed
	if (RGB_DB == NULL) {
		return 0;
	}

	//Ask for a query image
	printf(Q_IMAGE_ERQUSET);
	char Query[STRLENGTH];
	fgets(Query, STRLENGTH, stdin);

	if (Query[0] == '#') {
		terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,true);
		return 0;
	}

	//Query is not '#'. Calculate query and afterwards ask for anther one
	while (Query[0] != '#') {

		//Calculate RBG HIST for query
		SPPoint** RBG_query = spGetRGBHist(Query,-1,nBin);
		if (RBG_query == NULL) {
			terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,false);
			return 0;
		}

		//Calculate SIFT for query
		int howManySiftsQuery;
		SPPoint** SIFTS_Query = spGetSiftDescriptors(Query,-1,nSift,&howManySiftsQuery);
			if (SIFTS_Query == NULL) {
				terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,false);
				FreeOneArrayOfPoints(RBG_query);
				return 0;
			}




	}
	return 0;

}
