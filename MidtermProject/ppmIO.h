// Intermediate Programming 601.220 Midterm Project 10/06/2017
// Randy Kuang rkuang2 rkuang2@jhu.edu
// Justin Huang jhuan104 jhuan104@jhu.edu

// header file for all the reading and writing of PPM images

#ifndef PPMIO_H
#define PPMIO_H
#include <stdio.h>
#include <stdlib.h>

typedef struct _pixel {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} Pixel;

typedef struct _image {
  Pixel *data;
  int rows;
  int cols;
  int colors;
} Image;

Pixel *create_pix(int rows, int cols);

Image *create_im(); 

void destroy(Image *im, Pixel *pix);

int readPPM(char *filename, Image *im); 

int writePPM(Pixel *image, int rows, int cols, int colors, FILE *fp);

int writePPMImage(Image *im, FILE *fp);

int writePPMImageFile(Image *im, char *filename);

#endif
