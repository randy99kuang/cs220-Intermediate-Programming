// Intermediate Programming 601.220 Midterm Project 10/06/2017
// Randy Kuang rkuang2 rkuang2@jhu.edu
// Justin Huang jhuan104 jhuan104@jhu.edu

// contains implementation of all the functions pertaining to reading, writing, creating, destroying, copying images using the PPM format

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ppmIO.h"

// might need functions that create, destroy, and copy images 
Pixel *create_pix(int rows, int cols) {
  Pixel *pix = malloc(sizeof(Pixel) * rows * cols);
  return pix; 
}

Image *create_im() {
  Image *im = malloc(sizeof(Image));
  return im; 
}

void destroy(Image *im, Pixel *pix) {
  free(pix);
  free(im);
}
  
// A function that reads in content from a PPM image to an Image struct
int readPPM(char *filename, Image *im) {
  FILE *fp = fopen(filename, "rb");

  if (!fp) {
    fprintf(stderr, "Error:ppmIO - unable to open file \"%s\" for reading!\n", filename);
    return 0;
  } 

  char tag[3];
  int pic_info[3] = {0, 0, 0};
  int index = 0;
  char temp; 
  

  fscanf(fp, "%s", tag);

  //checking to make sure the file type is correct
  if (strcmp(tag, "P6") != 0) {
    printf("Error:ppmIO - wrong file type, file must be .ppm");
    return 0;
  }
  
  fgetc(fp);

  while (index < 3) {
    temp = fgetc(fp); 
    if (temp == '#')
      while (fgetc(fp) != '\n');
    else {
      ungetc(temp, fp);
      fscanf(fp, "%d", &pic_info[index]);
      fgetc(fp);
      index++;
    }
  } 

  //check to make sure 255 colors were read in 
  if (pic_info[2] != 255) {
    printf("Error:ppmIO - image does not have 255 colors");
    return 0;
  }

  //printf("%s %d %d %d\n", tag, pic_info[0], pic_info[1], pic_info[2]); 

  int cols = pic_info[0];
  int rows = pic_info[1];
  int colors = pic_info[2];
  
  Pixel *pixels = create_pix(rows, cols); 

  int read = fread(pixels, sizeof(Pixel), rows * cols, fp);
  fclose(fp);
  
  if (read != (rows * cols))
    fprintf(stderr, "Error:ppmIO - failed to read data from file!\n");

  im->data = pixels;
  im->rows = rows;
  im->cols = cols;
  im->colors = colors; 
  return read; 
}

// A function to write a PPM image to an already open filehandle
// Retruns written, or what should be (rows * col)
int writePPM(Pixel *image, int rows, int cols, int colors, FILE *fp) {
  if (!fp) {
    fprintf(stderr, "Error:ppmIO - writePPM given a bad filehandle\n");
    return 0;
  }

  //write tag
  fprintf(fp, "P6\n");

  //write dimensions
  fprintf(fp, "%d %d\n%d\n", cols, rows, colors);

  //write pixels, returns the number of pixels written 
  int written = fwrite(image, sizeof(Pixel), rows * cols, fp);
  if (written != (rows *cols))
    fprintf(stderr, "Error:ppmIO - failed to write data to file!\n");

  return written; 
}

// A function that takes an Image struct and writes to an open file pointer
// Also returns written 
int writePPMImage(Image *im, FILE *fp) {
  return writePPM(im->data, im->rows, im->cols, im->colors, fp);
}

// A function that takes a filename, handles opening/closing the file, and writes an Image struct to it
// Also returns written 
int writePPMImageFile(Image *im, char *filename) {
  FILE *fp = fopen(filename, "wb");

  if (!fp) {
    fprintf(stderr, "Error:ppmIO - unable to open file \"%s\" for writing!\n", filename);
    return 0;
  }

  int written = writePPM(im->data, im->rows, im->cols, im->colors, fp);
  fclose(fp);

  return written; 
}
  
