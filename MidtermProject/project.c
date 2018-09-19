// Intermediate Programming 601.220 Midterm Project 10/06/2017
// Randy Kuang rkuang2 rkuang2@jhu.edu
// Justin Huang jhuan104 jhuan104@jhu.edu

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"
#include "imageManip.h"
#include "menuUtil.h"

//make sure this is super short 
int main() {
  Image *im = malloc(sizeof(Image));
  main_loop(im);
  destroy(im, im->data);
  return 0;
}


