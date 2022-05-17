// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#ifndef SAVE_H_INCLUDED
#define SAVE_H_INCLUDED
#include "elementar.h"

void save_ASCII_pgm(image img, FILE *file);

void save_ASCII_ppm(image img, FILE *file);

void save_binary_pgm(image img, FILE *file);

void save_binary_ppm(image img, FILE *file);

void save_image(image img, char *c);

#endif

