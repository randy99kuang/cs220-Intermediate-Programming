// Intermediate Programming 601.220 Midterm Project 10/06/2017
// Randy Kuang rkuang2 rkuang2@jhu.edu
// Justin Huang jhuan104 jhuan104@jhu.edu

// contains all the functions pertaining to to printing the menu and reading in user input

#include "ppmIO.h"
#include "menuUtil.h"
#include "imageManip.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void print_menu() {
  printf(
	 "Main menu:\n"
	 "\tr <filename> - read image from <filename>\n"
	 "\tw <filename> - write image to <filename>\n"
	 "\ts - swap color channels\n"
	 "\tbr <amt> - change brightness (up or down) by the given amount\n"
	 "\tc <x1> <y1> <x2> <y2> - crop image to the box with the given corners\n"
	 "\tg - convert to grayscale\n"
	 "\tcn <amt> - change contrast (up or down) by the given amount\n"
	 "\tbl <sigma> - Gaussian blur with the given radius (sigma)\n"
	 "\tsh <sigma> <amt> - sharpen by given amount (intensity), with radius (sigma)\n"
	 "\te <sigma> <threshold> - detect edges with intensity gradient greater than the given threshold\n"
	 "\tq - quit\n"
	 "Enter choice: "
	 );
}

//function that skips spaces that user may have entered in input
void skip_spaces() {
  
  char temp;
  temp = fgetc(stdin);
  while (isspace(temp))
    temp = fgetc(stdin);
  ungetc(temp, stdin); 
}

int check_null(Image *im) {
  if (im->data == NULL) {
    printf("Error: You must read in an image first!\n");
    while (fgetc(stdin) != '\n'); //goes to a newline 
    return 1;
  }
  else
    return 0;
}

void wrong_parameters() {
  printf("Error: Incorrectly formatted parameters!\n");
  while (fgetc(stdin) != '\n'); //goes to a newline
  return;
}

void input_r(Image *im) {
  skip_spaces();
  char temp; 
 
  char *filename = malloc(1);
  filename[0] = '\0'; 
  int counter = 0;
  while ((temp = fgetc(stdin)) != '\n') {
    filename[counter] = temp;
    counter++;
    filename = realloc(filename, counter + 1);
    filename[counter] = '\0';
  }
  printf("Reading from %s...\n", filename);
  readPPM(filename, im);
  free(filename);
}

void input_w(Image *im) {
  if (check_null(im))
    return; 
  
  skip_spaces();
  char *filename = malloc(1);
  filename[0] = '\0';
  int counter = 0;
  char temp; 
  while ((temp = fgetc(stdin)) != '\n') {
    filename[counter] = temp;
    counter++;
    filename = realloc(filename, counter + 1);
    filename[counter] = '\0'; 
  }

  printf("Writing to %s...\n", filename);
  writePPMImageFile(im, filename); 
  free(filename);
}

void input_s(Image *im) {
  /*
  input_s and input_g do not use the check_null function because they do not have additional parameters, and the check_null function thus causes problems because it tries to skip parameters and go to a new line. However, since input_s and input_g already do not have parameters, things get weird. I couldn't figure out a better way of doing this.
  */ 
  if (im->data == NULL) {
    printf("You must read in an image first!\n");
    return;
  }
    
  printf("Swapping color channels...\n");
  swap(im); 
}

void input_br(Image *im) {
  if (check_null(im))
    return; 
       
  skip_spaces();
  int adjust;
  if (scanf("%d", &adjust) != 1) {
    wrong_parameters(); 
    return;
  }
  printf("Adjusting brightness by %d...\n", adjust); 
  adj_brightness(im, adjust);
}

void input_c(Image *im) {
  if (check_null(im))
    return; 
    
  int x1, x2, y1, y2;
  skip_spaces();
  if (scanf("%d", &x1) != 1) {
    wrong_parameters();
    return;
  }
  skip_spaces();
  if (scanf("%d", &y1) != 1) {
    wrong_parameters();
    return;
  }
  skip_spaces();
  if (scanf("%d", &x2) != 1) {
    wrong_parameters();
    return;
  }
  skip_spaces();
  if (scanf("%d", &y2) != 1) {
    wrong_parameters();
    return;
  }

  printf("Cropping region from (%d, %d), to (%d, %d)...\n", x1, y1, x2, y2);
  crop(im, x1, y1, x2, y2);
}

void input_g(Image *im) {
  if (im->data == NULL) {
    printf("You must read in an image first!\n"); 
    return;
  }
    
  printf("Converting to grayscale...\n");
  grayscale(im); 
}

void input_cn(Image *im) {
  if (check_null(im))
    return; 
    
  double input;
  skip_spaces();
  if (scanf("%lf", &input) != 1) {
    wrong_parameters();
    return;
  }
  printf("Adjusting contrast by %.2f...\n", input);
  contrast(im, input); 
}


void input_bl(Image *im) {
  if (check_null(im))
    return; 
    
  double input;
  skip_spaces();
  if (scanf("%lf", &input) != 1) {
    wrong_parameters();
    return;
  }
  printf("Applying blur filter, sigma %.2f...\n", input);
  apply_blur(im, input); 
}


void input_sh(Image *im) {
  if (check_null(im))
    return; 
  
  double variance, sharpening;
  skip_spaces();
  if (scanf("%lf", &variance) != 1) {
    wrong_parameters();
    return;
  }
  skip_spaces();
  if (scanf("%lf", &sharpening) != 1) {
    wrong_parameters();
    return;
  }
  printf("Applying sharpen filter, sigma %.2f, intensity %.2f...\n", variance, sharpening);
  sharpen(im, variance, sharpening); 
}

void input_e(Image *im) {
  if (check_null(im))
    return;
  
  double variance;
  int threshold;
  skip_spaces();
  if (scanf("%lf", &variance) != 1) {
    wrong_parameters();
    return;
  }
  skip_spaces();
  if (scanf("%d", &threshold) != 1) {
    wrong_parameters();
    return;
  }
  printf("Doing edge detection with sigma %.2f and threshold %d...\n", variance, threshold); 
  edge_detect(im, variance, threshold); 
}

void main_loop(Image *im) {
  char input;
  int stop_flag = 1;
  print_menu();
  while(stop_flag) {
    input = fgetc(stdin);
    
    if (input == 'q')
      break;
    else if (input == 'r')
      input_r(im);
    else if (input == 'w')
      input_w(im);
    else if (input == 's') {
      if (fgetc(stdin) == 'h')
	input_sh(im);
      else
	input_s(im);
    }
    else if (input == 'b') {
      char temp = fgetc(stdin);
      if (temp == 'r') {
	input_br(im);
      }
      else if (temp == 'l')
	input_bl(im);
      else {
	printf("Bad menu option %c%c\n", input, temp);
	while (fgetc(stdin) != '\n'); //moves pointer to new line
      }
    }
    else if (input == 'c') {
      char temp = fgetc(stdin);
      if (temp == 'n')
	input_cn(im);
      else if (temp == ' ')
	input_c(im);
      else {
	printf("Bad menu option %c%c\n", input, temp);
	while (fgetc(stdin) != '\n'); //moves pointer to new line
      }
    }
    else if (input == 'g')
      input_g(im);
    else if (input == 'e')
      input_e(im); 
    else if (isspace(input))
	    continue;
    else {
      printf("Bad menu option %c\n", input);
      while (fgetc(stdin) != '\n'); //moves pointer to new line
    }

    print_menu();
  }
}

