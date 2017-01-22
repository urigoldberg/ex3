#include <stdio.h>
#include "sp_image_proc_util.h"
#include "main_aux.h"


int main() {

	//Ask for PATH
	printf(DIR_REQUEST);
//	char dir[STRLENGTH];
//	fgets(dir, STRLENGTH, stdin);
	char dir[] = "/Users/dor/Downloads/SPAssignment3/images/";


	//Ask for PREFIX
	printf(PRE_REQUEST);
//	char picName[STRLENGTH];
//	fgets(picName, STRLENGTH, stdin);
	char picName[] = "img";

	//Ask for NUM OF PICTURES
	printf(IMAGE_NUM_ERQUSET);
	int HowManypic;
//	scanf("%d", &HowManypic);
	HowManypic = 17;
//	getchar();



	//Check if the user entered an integer that is less than 1
	if (HowManypic < 1) {
		printf(IMAGE_NUM_ERROR);
		return 0;
	}

	//Ask for SUFFIX
	printf(SUF_REQUEST);
//	char sufName[STRLENGTH];
//	fgets(sufName, STRLENGTH, stdin);
	char sufName[] = ".png";

	//Ask for BINS
	printf(BIN_ERQUSET);
	int nBin;
//	scanf("%d",&nBin);
	nBin = 3;
//	getchar();

	//Check if the user entered a number that is less than 1 or greater than 255
	if (((nBin < 1)||(nBin > 255))&&(false)) {
		printf(NUM_BIN_ERROR);
		return 0;
		}

	//Ask for SIFTS
	printf(SIFT_REQUEST);
	int nSift;
//	scanf("%d",&nSift);
	nSift = 15;
//	getchar();

	//Check if the user entered a number that is less than 1 or greater than 255
	if (nSift < 1) {
		printf(NUM_SIFT_ERROR);
		return 0;
		}

	//Create DBs
	SPPoint*** RGB_DB = NULL;
	SPPoint***  SIFTS_DB = NULL;
	int* NumOfSiftExtracted = NULL;


	/**
	 * you need to pass a pointer to RGB_DB and SIFTS_DB in order to get the results in here
	 * and inside the function you need to address them as *RGB_DB etc..
	 */
	//Initialize DBs and inserts values
	createAllDB(NumOfSiftExtracted, &RGB_DB, &SIFTS_DB, dir,
			picName, HowManypic, sufName, nBin, nSift);
	//Initialize DBs failed
	if (RGB_DB == NULL) {
		return 0;
	}


	//Ask for a query image
	printf(Q_IMAGE_ERQUSET);
//	char Query[STRLENGTH];
//	fgets(Query, STRLENGTH, stdin);
	char Query[] = "/Users/dor/Downloads/SPAssignment3/queryA.png";
//	removeNewline(Query);

	if (Query[0] == '#') {
		terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,true);
		return 0;
	}

	//Query is not '#'. Calculate query and afterwards ask for anther one
	while (Query[0] != '#') {

		//Calculate RBG HIST for query
		printf("\nline97\n");
		SPPoint** RBG_query = spGetRGBHist(Query,0,nBin);
		if (RBG_query == NULL) {
			terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,false);
			return 0;
		}

		//Calculate SIFT for query
		printf("\nline105\n");
		int howManySiftsQuery;
		SPPoint** SIFTS_Query = spGetSiftDescriptors(Query,0,nSift,&howManySiftsQuery);
			if (SIFTS_Query == NULL) {
				terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,false);
				FreeOneArrayOfPoints(RBG_query);
				return 0;
			}

		//Print 5 closest
			printf("\nline115\n");
		int test = print_5_GlobalFeatures(RBG_query, RGB_DB, HowManypic);

		//if test == -1, something went wrong...free all and run
		printf("\nline119\n");
		if (test == -1){
			terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,false);
			FreeOneArrayOfPoints(RBG_query);
			FreeOneArrayOfPoints(SIFTS_Query);
			return 0;
		}

		//Print 5 closest
		printf("\nline128\n");
		int test1 = print_5_LocalFeatures( SIFTS_Query, SIFTS_DB,NumOfSiftExtracted, howManySiftsQuery,
				 HowManypic);

		//if test1 == -1, something went wrong...free all and run
		if (test1 == -1){
			terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,false);
			FreeOneArrayOfPoints(RBG_query);
			FreeOneArrayOfPoints(SIFTS_Query);
			return 0;
			}

	}

	//Query is '#'. free all and bye bye
	terminate(RGB_DB,SIFTS_DB,NumOfSiftExtracted,HowManypic,true);
	return 0;

}


