// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#ifndef ELEMENTAR_H_INCLUDED
#define ELEMENTAR_H_INCLUDED

typedef struct {
	double red;
	double green;
	double blue;
	double gray;
} colors;

typedef struct {
	colors **map;
	int nr_lines;
	int nr_collums;
} pixel_map;

typedef struct {
	int type;
	int *index;
	int nr_max;
	pixel_map image;
	pixel_map selection;
	char *filename;
} image;

pixel_map alloc_pixel_map(int nl, int nc);

void free_pixel_map(pixel_map image);

char *read_command(char *c);

char *read_full_command(void);

image unite(image img);

pixel_map copy_map(pixel_map image, pixel_map selection);

void exit_program(image img);

#endif

