// Intermediate Programming 601.220 Midterm Project 10/06/2017
// Randy Kuang rkuang2 rkuang2@jhu.edu
// Justin Huang jhuan104 jhuan104@jhu.edu

// contains all the functions pertaining to image manipulation algorithms
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "imageManip.h"
#include "ppmIO.h"

#define PI 3.14159265

void swap(Image *im) {
  int temp;
  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      temp = (im->data[(r * im->cols) + c]).r;
      im->data[(r * im->cols) + c].r = im->data[(r * im->cols) + c].g;
      im->data[(r * im->cols) + c].g = im->data[(r * im->cols) + c].b;
      im->data[(r * im->cols) + c].b = temp;
    }
  }
}

/**
 * Helper method to clamp the brightness adjusting math 
 * so that it gives a result between 0 and 255.
 * @param rgb the starting r, g or b values
 * @param adjust the value entered by the user. Positive values
 * brighten image while negative values darken image.
 */
unsigned char saturateAdd(unsigned char rgb, int adjust) {
  int newValue = rgb;
  newValue += adjust;
  if (newValue > 255) {
    newValue = 255;
  } else if (newValue < 0) {
    newValue = 0;
    }
  return (unsigned char) newValue;
}

/**
 * Helper method to clamp the contrast adjusting math
 * so that it gives a result between -127.5 and 127.5.
 * @param rgb the starting r, g or b values
 * @param adjust the value entered by the user. Positive values
 * increase contrast of image while negative values decrease contrast.
 */
unsigned char saturateMult(unsigned char rgb, double adjust) {
  double newContrastValue = rgb - 127.5;
  newContrastValue *= adjust;
  if ((double) newContrastValue > 127.5) {
    newContrastValue = 127.5;
  } else if ((double) newContrastValue < -127.5) {
    newContrastValue = -127.5;
    }
  return (unsigned char) (newContrastValue + 127.5);
}

void adj_brightness(Image *im, int adjust) {
  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      im->data[(r * im->cols) + c].r =
	saturateAdd(im->data[(r * im->cols) + c].r, adjust);
      im->data[(r * im->cols) + c].g =
	saturateAdd(im->data[(r * im->cols) + c].g, adjust);
      im->data[(r * im->cols) + c].b =
	saturateAdd(im->data[(r * im->cols) + c].b, adjust);
    }
  }
}

void grayscale(Image *im) {
  unsigned char intensity;
  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      intensity = im->data[(r * im->cols) + c].r * 0.3
	+ im->data[(r * im->cols) + c].g * 0.59
	+ im->data[(r * im->cols) + c].b * 0.11;
      im->data[(r * im->cols) + c].r = intensity;
      im->data[(r * im->cols) + c].g = intensity;
      im->data[(r * im->cols) + c].b = intensity;
    }
  }
}

void contrast(Image *im, double input) {
  if (input <= 0) {
    printf("Error: Contrast factor cannot be less than or equal to 0\n");
    return;
  }
  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      im->data[(r * im->cols) + c].r =
	saturateMult(im->data[(r * im->cols) + c].r, input);
      im->data[(r * im->cols) + c].g =
	saturateMult(im->data[(r * im->cols) + c].g, input);
      im->data[(r * im->cols) + c].b =
	saturateMult(im->data[(r * im->cols) + c].b, input);
    }
  }
}

void crop(Image *im, int x1, int y1, int x2, int y2) {
  //check for errors

  if (x1 < 0 || y1 < 0 || x2 >= im->cols || y2 >= im->rows) {
    printf("Error: Invalid crop parameters\n");
    return;
  }
  
  int cols = x2 - x1 + 1;
  int rows = y2 - y1 + 1;
  int orig_cols = im->cols;
  
  
  Pixel *cropped_pixels = create_pix(rows, cols);
  for (int i = 0; i < rows; i++) 
    for (int j = 0; j < cols; j++)
      cropped_pixels[i * cols + j] = im->data[(y1 + i) * orig_cols + x1 + j];

  free(im->data);
  im->data = cropped_pixels;
  im->rows = rows;
  im->cols = cols;
  
}

// Image copy method used when we need to keep track of original image
// while making a new image (blur and edge detection)
void copy_image(Image *orig, Image* copy) {
  copy->rows = orig->rows;
  copy->cols = orig->cols;
  copy->colors = orig->colors;
  copy->data = create_pix(copy->rows, copy->cols);
  for (int r = 0; r < copy->rows; r++) {
    for (int c = 0; c < copy->cols; c++) {
      copy->data[r * copy->cols + c].r = orig->data[r * orig->cols + c].r;
      copy->data[r * copy->cols + c].g = orig->data[r * orig->cols + c].g;
      copy->data[r * copy->cols + c].b = orig->data[r * orig->cols + c].b;
    }
  }
}

double* make_matrix(double variance) {
  int size = (int) (variance * 5);
  int length = 2 * size + 1;
  double *matrix = malloc(length * length * sizeof(double));

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < length; j++) {
      matrix[i * length + j] = (1.0 / (2.0 * PI * variance * variance)) * exp( -((i - size) * (i - size) + (j - size) * (j - size)) / (2 * variance * variance));
    }
  }
  return matrix; 
}

Pixel pixel_blur_value(Image *im, double *matrix, int row, int col, double variance) {
  Pixel new_pix;
  int row_max = im->rows;
  int col_max = im->cols;
  int length = ((int) (variance * 5)) * 2 + 1; 
  int size = (length - 1) / 2; //this variable names are confusing, but size is the length from the middle of the matrix out to one of the edges
  int left = col - size;
  int right = col + size;
  int top = row - size;
  int bot = row + size;
  double r_sum = 0;
  double g_sum = 0;
  double b_sum = 0;
  double weight = 0;
  int matrix_count = 0; //keeps track of where in the gaussian matrix we're at 

  for (int i = top; i <= bot; i++) {
    for (int j = left; j <= right; j++, matrix_count++) {
      if (i >= 0 && j >= 0 && i < row_max && j < col_max) {
	r_sum += matrix[matrix_count] * im->data[i * col_max + j].r;
	g_sum += matrix[matrix_count] * im->data[i * col_max + j].g;
	b_sum += matrix[matrix_count] * im->data[i * col_max + j].b;
	weight += matrix[matrix_count];
      }
    }
  }
  new_pix.r = (unsigned char) (r_sum / weight);
  new_pix.g = (unsigned char) (g_sum / weight);
  new_pix.b = (unsigned char) (b_sum / weight);
  return new_pix;
}

// Make sure to free the Gaussian matrix at the end of this
void apply_blur(Image *im, double variance) {
  if (variance == 0) {
    printf("Error: Variance parameter cannot be equal to 0\n");
    return;
  }
  Image* blur_im = malloc(sizeof(Image));
  copy_image(im, blur_im);
  double *gaussian = make_matrix(variance);
  
  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      im->data[(r * im->cols) + c] =  pixel_blur_value(blur_im, gaussian, r, c, variance);
    }
  }
  destroy(blur_im, blur_im->data);
  free(gaussian);
}


void sharpen(Image *im, double variance, double sharpening) {
  double *gaussian = make_matrix(variance);
  
  int diff_red;
  int diff_green;
  int diff_blue;
  Pixel blur_pix; 

  for (int r = 0; r < im->rows; r++)
    for (int c = 0; c < im->cols; c++) {
      blur_pix = pixel_blur_value(im, gaussian, r, c, variance); 
      diff_red = (int) (sharpening * (im->data[(r * im->cols) + c].r - blur_pix.r));
      diff_green = (int) (sharpening * (im->data[(r * im->cols) + c].g - blur_pix.g));
      diff_blue = (int) (sharpening * (im->data[(r * im->cols) + c].b - blur_pix.b));
      im->data[(r * im->cols) + c].r = saturateAdd(im->data[(r * im->cols) + c].r, diff_red);
      im->data[(r * im->cols) + c].g = saturateAdd(im->data[(r * im->cols) + c].g, diff_green);
      im->data[(r * im->cols) + c].b = saturateAdd(im->data[(r * im->cols) + c].b, diff_blue);
    }
  free(gaussian); 
}

void edge_detect(Image *im, double variance, int threshold) {
  if (variance == 0) {
    printf("Error: Variance parameter cannot be equal to 0\n");
    return;
  }
  
  double *gaussian = make_matrix(variance);

  Image *edge = malloc(sizeof(Image));
  copy_image(im, edge);

  grayscale(edge);
  int x_intensity = 0;
  int y_intensity = 0;
  
  // Blurring
  for (int r = 0; r < edge->rows; r++) {
    for (int c = 0; c < edge->cols; c++) {
      edge->data[(r * edge->cols) + c] =  pixel_blur_value(edge, gaussian, r, c, variance);
    }
  }
  
  // Calculating intensities for each pixel, using only r value (since all rgb are the same)
  for (int r = 1; r < edge->rows - 1; r++) {
    for (int c = 1; c < edge->cols - 1; c++) {
      x_intensity = (edge->data[(r * edge->cols) + c + 1].r - edge->data[(r * edge->cols) + c - 1].r) / 2;
      y_intensity = (edge->data[((r + 1) * edge->cols) + c].r - edge->data[((r - 1) * edge->cols) + c].r ) / 2;
     im->data[(r * im->cols) + c].r = sqrt(x_intensity * x_intensity + y_intensity * y_intensity);
    }
  }

  // Comparing to threshold to determine which spots are edges
  for (int r = 1; r < edge->rows - 1; r++) {
    for (int c = 1; c < edge->cols - 1; c++) {
      if (im->data[(r * im->cols) + c].r > threshold) {
	im->data[(r * edge->cols) + c].r = 0;
	im->data[(r * edge->cols) + c].g = 0;
	im->data[(r * edge->cols) + c].b = 0;
      } else {
	im->data[(r * edge->cols) + c].r = 255;
	im->data[(r * edge->cols) + c].g = 255;
	im->data[(r * edge->cols) + c].b = 255;
      }
    }
  }
  destroy(edge, edge->data);
  free(gaussian);
}
