/*
 * sp_image_proc_util.cpp
 *
 *  Created on: Jan 15, 2017
 *      Author: U&D
 */

#include "sp_image_proc_util.h"
#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>


#define POINTSNUMBER 3
#define THIRD 0.33
#define MEM_PEOBLEM "An error occurred - allocation failure\n"
#define IMG_CANNT_BE_LOADED "Image cannot be loaded - %s:\n"

extern "C" {
#include "SPBPriorityQueue.h"
#include "SPPoint.h"
}
using namespace cv;
using namespace std;

SPPoint** spGetRGBHist(const char* str, int imageIndex, int nBins) {

	//Validate input
	if ((str == NULL) || (nBins <= 0)) {
		return NULL;
	}

	//Load image
	//Return NULL if fails
	Mat src;
	src = imread(str, CV_LOAD_IMAGE_COLOR);
	if (src.empty()) {
		printf(IMG_CANNT_BE_LOADED,str);
		return NULL;
	}

	//Separate the image in 3 places (B,G and R)
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	float range[] = { 0, 256 };
	const float* histRange = { range };

	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &nBins, &histRange);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &nBins, &histRange);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &nBins, &histRange);

	SPPoint** arrPoint = (SPPoint**) malloc(POINTSNUMBER * sizeof(SPPoint*));
	if (arrPoint == NULL) {
		printf(MEM_PEOBLEM);
		return NULL;
	}

	double* red = (double*) malloc(nBins * sizeof(double));
	if (red == NULL) {
		printf(MEM_PEOBLEM);
		free(arrPoint);
		return NULL;
	}

	double* blue = (double*) malloc(nBins * sizeof(double));
	if (blue == NULL) {
		printf(MEM_PEOBLEM);
		free(red);
		free(arrPoint);
		return NULL;
	}

	double* green = (double*) malloc(nBins * sizeof(double));
	if (green == NULL) {
		printf(MEM_PEOBLEM);
		free(red);
		free(blue);
		free(arrPoint);
		return NULL;
	}

	for (int i = 0; i < nBins; i++) {
		red[i] = r_hist.at<double>(0, i);
		blue[i] = b_hist.at<double>(0, i);
		green[i] = g_hist.at<double>(0, i);
	}

	arrPoint[0] = spPointCreate(red, nBins, imageIndex);
	//Allocation failed - Free resources
	if (arrPoint[0] == NULL) {
		printf(MEM_PEOBLEM);
		free(arrPoint);
		free(red);
		free(blue);
		free(green);
		return NULL;
	}

	arrPoint[1] = spPointCreate(blue, nBins, imageIndex);
	//Allocation failed - Free resources
	if (arrPoint[1] == NULL) {
		printf(MEM_PEOBLEM);
		free(arrPoint[0]);
		free(arrPoint);
		free(red);
		free(blue);
		free(green);
		return NULL;
	}

	arrPoint[2] = spPointCreate(green, nBins, imageIndex);
	//Allocation failed - Free resources
	if (arrPoint[2] == NULL) {
		printf(MEM_PEOBLEM);
		free(arrPoint[0]);
		free(arrPoint[1]);
		free(arrPoint);
		free(red);
		free(blue);
		free(green);
		return NULL;
	}

	//Free resources
	free(red);
	free(blue);
	free(green);
	return arrPoint;
}

double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB) {

	//Validate
	if ((rgbHistA == NULL) || (rgbHistB == NULL)) {
		return -1;
	}
	int chan01 = sizeof(rgbHistA) / sizeof(SPPoint*);
	int chan02 = sizeof(rgbHistB) / sizeof(SPPoint*);
	if (chan01 != chan02) {
		return -1;
	}

	//Calculate
	double first, second, third;
	first = spPointL2SquaredDistance(rgbHistA[0], rgbHistB[0]);
	second = spPointL2SquaredDistance(rgbHistA[1], rgbHistB[1]);
	third = spPointL2SquaredDistance(rgbHistA[2], rgbHistB[2]);

	return (THIRD * first + THIRD * second + THIRD * third);
}

SPPoint** spGetSiftDescriptors(const char* str, int imageIndex,
		int nFeaturesToExtract, int *nFeatures) {
	//Validate input
	if ((str == NULL) || (nFeatures == NULL) || (nFeaturesToExtract <= 0)) {
		return NULL;
	}

	//Load image
	//Return NULL if fails
	Mat src;
	src = imread(str, CV_LOAD_IMAGE_GRAYSCALE);
	if (src.empty()) {
		printf(IMG_CANNT_BE_LOADED,str);
		return NULL;
	}

	//Key points will be stored in kp1
	vector<cv::KeyPoint> kp1;

	//Feature values will be stored in ds1;
	Mat ds1;

	//Creating  a Sift Descriptor extractor
	Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
			xfeatures2d::SIFT::create(nFeaturesToExtract);
	//Extracting features
	detect->detect(src, kp1, Mat());
	detect->compute(src, kp1, ds1);
	*nFeatures = ds1.rows;

	//Creating array of points with size k
	SPPoint** arrPoint = (SPPoint**) malloc(ds1.rows * sizeof(SPPoint*));
	if (arrPoint == NULL) {
		printf(MEM_PEOBLEM);
		return NULL;
	}

	double* SIFTfeatures = (double*) malloc(ds1.cols * (sizeof(double)));
	if (SIFTfeatures == NULL) {
		printf(MEM_PEOBLEM);
		free(arrPoint);
		return NULL;
	}
	for (int i = 0; i < ds1.rows; i++) {
		for (int j = 0; j < ds1.cols; j++) {
			SIFTfeatures[j] = ds1.at<double>(i, j);
		}
		arrPoint[i] = spPointCreate(SIFTfeatures, ds1.cols, imageIndex);
		//Allocation for one point failed
		if (arrPoint[i] == NULL) {
			printf(MEM_PEOBLEM);
			for (int t = 0; i < i; t++) {
				//free all points
				spPointDestroy(arrPoint[i]);
			}
			//free array
			free(arrPoint);
		}
	}
	return arrPoint;
}

int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
		SPPoint*** databaseFeatures, int numberOfImages,
		int* nFeaturesPerImage) {

	//Validation
	if ((queryFeature == NULL) || (databaseFeatures == NULL)
			|| (numberOfImages <= 1) || (nFeaturesPerImage == NULL)) {
		return NULL;
	}

	SPBPQueue* KCLSQueue = spBPQueueCreate(kClosest);
	//Allocation failed
	if (KCLSQueue == NULL) {
		printf(MEM_PEOBLEM);
		return NULL;
	}

	//Full queue with distances from database
	for (int i = 0; i < numberOfImages; i++) {
		for (int j = 0; j < nFeaturesPerImage[i]; j++) {
			spBPQueueEnqueue(KCLSQueue, i,
					spPointL2SquaredDistance((databaseFeatures[i][j]),
							queryFeature));
		}
	}

	//Copy queue into int array

	int* arrayOfPic = (int*) malloc(sizeof(int) * kClosest);
	//Allocation failed
	if (arrayOfPic == NULL) {
		printf(MEM_PEOBLEM);
		spBPQueueDestroy(KCLSQueue);
		return NULL;
	}

	//Allocate BPQueueElement for peeking from queue
	BPQueueElement* ElemForEnqueue = (BPQueueElement*) malloc(
			sizeof(BPQueueElement));
	//Allocation failed
	if (ElemForEnqueue == NULL) {
		printf(MEM_PEOBLEM);
		free(arrayOfPic);
		spBPQueueDestroy(KCLSQueue);
		return NULL;
	}
	/**
	 * ====================VERY IMPORTANT====================
	 * FOR TIE BREAK - CHANGE QUEUE FOR CASES OF EQUAL VALUE
	 * 				(In Enqueue)
	 * ======================================================
	 */
	for (int i = 0; i < kClosest; i++) {
		spBPQueuePeek(KCLSQueue, ElemForEnqueue);
		spBPQueueDequeue(KCLSQueue);
		arrayOfPic[i] = ElemForEnqueue->index;
	}

	//Free all resources
	free(ElemForEnqueue);
	spBPQueueDestroy(KCLSQueue);

	return arrayOfPic;
}
