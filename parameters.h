// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#ifndef PARAMETERS_H_INCLUDED
#define PARAMETERS_H_INCLUDED
#include "elementar.h"

// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elementar.h"
#include "parameters.h"

double **alloc_kernel(void);

void free_kernel(double **kernel);

double clamp(double x);

colors find_value(int i, int j, pixel_map image, double **kernel, int div);

pixel_map save_values(image img, double **kernel, int div);

image new_image(image img, double **kernel, int div);

image apply_edge(image img);

image apply_sharpen(image img);

image apply_blur(image img);

image apply_gaussian_blur(image img);

image apply_parameter(image img, char *c);

#endif

