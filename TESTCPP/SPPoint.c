#include "SPPoint.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct sp_point_t {
	int index;
	int dim;
	double* data;
};

SPPoint* spPointCreate(double* data, int dim, int index) {
	if (data == NULL || dim < 1 || index < 0) {
		return NULL;
	}
	SPPoint* p = (SPPoint*) malloc(sizeof(SPPoint));
	if (p == NULL) {
		return NULL;
	}
	p->data = (double*) malloc(sizeof(double) * dim);
	if (p->data == NULL) {
		free(p);
		return NULL;
	}
	for (int i = 0; i < dim; ++i) {
		p->data[i] = data[i];
	}
	p->dim = dim;
	p->index = index;

	return p;
}

SPPoint* spPointCopy(SPPoint* source) {
	assert(source != NULL);
	SPPoint* copy = (SPPoint*) malloc(sizeof(SPPoint));
	if (copy == NULL) {
		return NULL;
	}
	int i = 0;
	copy->data = (double*) malloc(sizeof(double) * source->dim);
	if (copy->data == NULL) {
		free(copy);
		return NULL;
	}
	for (i = 0; i < source->dim; ++i) {
		copy->data[i] = source->data[i];
	}
	copy->dim = source->dim;
	copy->index = source->index;

	return copy;
}

void spPointDestroy(SPPoint* point) {
	if (point != NULL) {
		free(point->data);
		free(point);
	}
}

int spPointGetDimension(SPPoint* point) {
	assert(point != NULL);
	return (point->dim);
}

int spPointGetIndex(SPPoint* point) {
	assert(point != NULL);
	return (point->index);
}

double spPointGetAxisCoor(SPPoint* point, int axis) {
	assert(point != NULL);
	assert(axis < point->dim);
	return (point->data[axis]);
}

double spPointL2SquaredDistance(SPPoint* p, SPPoint* q) {
	assert(p != NULL && q != NULL);
	assert(p->dim == q->dim);
	double result = 0;
	int i = 0;
	for (i = 0; i < p->dim; ++i) {
		result += ((q->data[i] - p->data[i]) * (q->data[i] - p->data[i]));
	}
	return result;
}
