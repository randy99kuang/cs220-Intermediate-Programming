// Intermediate Programming 601.220 Midterm Project 10/06/2017
// Randy Kuang rkuang2 rkuang2@jhu.edu
// Justin Huang jhuan104 jhuan104@jhu.edu
#include <stdio.h>
#include <math.h>
#include "ppmIO.h"
//header file for image manipulation stuff

#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H

#include <stdio.h>

void swap(Image *im);

void adj_brightness(Image *im, int adjust);

unsigned char saturateAdd(unsigned char rgb, int adjust);

unsigned char saturateMult(unsigned char rgb, double adjust);

void grayscale(Image *im);

void contrast(Image *im, double input);

void crop(Image *im, int x1, int y1, int x2, int y2); 

double* make_matrix(double variance);

Pixel pixel_blur_value(Image *im, double *matrix, int row, int col, double variance);

void apply_blur(Image *im, double variance);

void sharpen(Image *im, double variance, double sharpening); 

void edge_detect(Image *im, double variance, int threshold);
#endif
