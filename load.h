// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#ifndef LOAD_H_INCLUDED
#define LOAD_H_INCLUDED
#include "elementar.h"
#include <stdio.h>

pixel_map read_pgm_ASCII(pixel_map image, FILE * file);

pixel_map read_pgm_binary(pixel_map image, FILE *file);

image read_grayscale(FILE *file, image img);

pixel_map read_ppm_ASCII(pixel_map image, FILE *file);

pixel_map read_ppm_binary(pixel_map image, FILE *file);

image read_rgb(FILE *file, image img);

image load_file(char *c, image img);

#endif

