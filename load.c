// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elementar.h"
#include "load.h"

// read a P2 image
pixel_map read_pgm_ASCII(pixel_map image, FILE *file)
{
	for (int i = 0; i < image.nr_lines; ++i)
		for (int j = 0; j < image.nr_collums; ++j)
			fscanf(file, "%lf", &image.map[i][j].gray);

	return image;
}

// read a P5 image
pixel_map read_pgm_binary(pixel_map image, FILE *file)
{
	for (int i = 0; i < image.nr_lines; ++i)
		for (int j = 0; j < image.nr_collums; ++j) {
			// the reading is done in an unsigned char auxiliary
			unsigned char aux;
			fread(&aux, sizeof(unsigned char), 1, file);
			image.map[i][j].gray = aux;
		}

	return image;
}

// check what kind of grayscale immage will be read
image read_grayscale(FILE *file, image img)
{
	img.image = alloc_pixel_map(img.image.nr_lines, img.image.nr_collums);

	if (img.type == 2)
		img.image = read_pgm_ASCII(img.image, file);
	else
		img.image = read_pgm_binary(img.image, file);

	return img;
}

// read a P3 image
pixel_map read_ppm_ASCII(pixel_map image, FILE *file)
{
	for (int i = 0; i < image.nr_lines; ++i)
		for (int j = 0; j < image.nr_collums; ++j) {
			fscanf(file, "%lf", &image.map[i][j].red);
			fscanf(file, "%lf", &image.map[i][j].green);
			fscanf(file, "%lf", &image.map[i][j].blue);
		}

	return image;
}

// read a P6 image
pixel_map read_ppm_binary(pixel_map image, FILE *file)
{
	for (int i = 0; i < image.nr_lines; ++i)
		for (int j = 0; j < image.nr_collums; ++j) {
			// the reading is done in an unsigned char* auxiliary
			unsigned char aux[3];
			fread(&aux, sizeof(unsigned char), 3, file);
			image.map[i][j].red = aux[0];
			image.map[i][j].green = aux[1];
			image.map[i][j].blue = aux[2];
		}

	return image;
}

// check what kind of rgb image will be read
image read_rgb(FILE *file, image img)
{
	img.image = alloc_pixel_map(img.image.nr_lines, img.image.nr_collums);

	if (img.type == 3)
		img.image = read_ppm_ASCII(img.image, file);
	else
		img.image = read_ppm_binary(img.image, file);

	return img;
}

// load an image into memory
image load_file(char *c, image img)
{
	// release the previous image if any
	exit_program(img);
	FILE *file;

	unsigned long i = 0;
	// find the lenght of the file name to be uploaded
	while (c[5 + i] != ' ' && (5 + i) < strlen(c))
		i++;

	// allocate memory for file name
	img.filename = (char *)malloc(i + 1);
	if (!img.filename)
		fprintf(stderr, "malloc() failed\n");
	memcpy(img.filename, &c[5], i);
	img.filename[i] = '\0';

	// open the file and check is the operation was successful
	file = fopen(img.filename, "r+b");
	if (!file) {
		printf("Failed to load %s\n", img.filename);
		free(img.filename);
		img.type = -1;
	} else {
		getc(file);

		// read image specification
		fscanf(file, "%d", &img.type);
		fscanf(file, "%d %d", &img.image.nr_collums, &img.image.nr_lines);
		fscanf(file, "%d\n", &img.nr_max);

		// allocate memory for indexes of a possible selection
		img.index = (int *)malloc(4 * sizeof(int));
		if (!img.index)
			fprintf(stderr, "malloc() failed\n");
		for (int i = 0; i < 4; ++i)
			img.index[i] = -1;

		// check what kind of image is to be read
		if (img.type == 2 || img.type == 5)
			img = read_grayscale(file, img);
		else if (img.type == 3 || img.type == 6)
			img = read_rgb(file, img);

		// close the file and display the desired message
		fclose(file);
		printf("Loaded %s\n", img.filename);
	}

	return img;
}
