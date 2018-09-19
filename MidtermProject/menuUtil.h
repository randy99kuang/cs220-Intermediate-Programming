// Intermediate Programming 601.220 Midterm Project 10/06/2017
// Randy Kuang rkuang2 rkuang2@jhu.edu
// Justin Huang jhuan104 jhuan104@jhu.edu

// header file for all the menu output and input functions

#ifndef MENUUTIL_H
#define MENUUTIL_H

#include <stdio.h>
#include <stdlib.h>

void print_menu();

void skip_spaces();

int check_null(Image *im);

void wrong_parameters(); 

void input_r(Image *im);

void input_w(Image *im);

void input_s(Image *im);

void input_br(Image *im);

void input_c(Image *im);

void input_g(Image *im);

void input_cn(Image *im);

void input_bl(Image *im);

void input_sh(Image *im);

void input_e(Image *im);

void main_loop(Image *im); 



#endif
