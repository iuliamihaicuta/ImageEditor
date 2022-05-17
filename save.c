// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "elementar.h"
#include "save.h"

// save a P2 image
void save_ASCII_pgm(image img, FILE *file)
{
	// save the header
	fprintf(file, "P2\n%d %d\n", img.image.nr_collums, img.image.nr_lines);
	fprintf(file, "%d\n", img.nr_max);

	// save the values as text in the file using an unsigned char auxiliary
	unsigned char aux;
	for (int i = 0; i < img.image.nr_lines; i++) {
		for (int j = 0; j < img.image.nr_collums - 1; ++j) {
			aux = img.image.map[i][j].gray;
			fprintf(file, "%d ", aux);
		}
		aux = img.image.map[i][img.image.nr_collums - 1].gray;
		fprintf(file, "%d\n", aux);
	}
}

// save a P3 image
void save_ASCII_ppm(image img, FILE *file)
{
	// save the header
	fprintf(file, "P3\n%d %d\n", img.image.nr_collums, img.image.nr_lines);
	fprintf(file, "%d\n", img.nr_max);

	for (int i = 0; i < img.image.nr_lines; i++) {
		for (int j = 0; j < img.image.nr_collums; ++j) {
			/* save the values as text in the file using an
			unsigned char auxiliary array */
			unsigned char aux[3];
			aux[0] = round(img.image.map[i][j].red);
			aux[1] = round(img.image.map[i][j].green);
			aux[2] = round(img.image.map[i][j].blue);
			for (int k = 0; k < 3; ++k)
				fprintf(file, "%d ", aux[k]);
		}
		fprintf(file, "\n");
	}
}

// save a P5 image
void save_binary_pgm(image img, FILE *file)
{
	// save the header
	fprintf(file, "P5\n%d %d\n", img.image.nr_collums, img.image.nr_lines);
	fprintf(file, "%d\n", img.nr_max);

	// save the binary values in the file using an unsigned char auxiliary
	unsigned char aux;
	for (int i = 0; i < img.image.nr_lines; i++)
		for (int j = 0; j < img.image.nr_collums; ++j) {
			aux = round(img.image.map[i][j].gray);
			fwrite(&aux, sizeof(unsigned char), 1, file);
		}
}

// save a P6 image
void save_binary_ppm(image img, FILE *file)
{
	// save the header
	fprintf(file, "P6\n%d %d\n", img.image.nr_collums, img.image.nr_lines);
	fprintf(file, "%d\n", img.nr_max);

	for (int i = 0; i < img.image.nr_lines; i++)
		for (int j = 0; j < img.image.nr_collums; ++j) {
			/* save the binary values in the file using
			an unsigned char auxiliary array */
			unsigned char aux[3];
			aux[0] = round(img.image.map[i][j].red);
			aux[1] = round(img.image.map[i][j].green);
			aux[2] = round(img.image.map[i][j].blue);
			fwrite(&aux, sizeof(unsigned char), 3, file);
		}
}

// save an image to a file
void save_image(image img, char *c)
{
	// find the lenght and name of the file in which we save the image
	int i = 5;
	while (c[i] != ' ' && c[i] != '\0')
		i++;

	char filename[50];
	memcpy(filename, &c[5], (i - 5) * sizeof(char));
	filename[i - 5] = '\0';

	// paste the selection back into the full image
	img = unite(img);

	// open a new file
	FILE *new_file;
	new_file = fopen(filename, "w");

	// check how the data will be saved
	char *type = (char *)malloc(6 * sizeof(char));
	if (!type)
		fprintf(stderr, "malloc() failed\n");
	if (strlen(c) - i > 5)
		memcpy(type, &c[i + 1], 6);
	else if (strlen(c) - i == 0)
		type[0] = '-';
	else
		memcpy(type, &c[i + 1], (strlen(c) - i));

	if (strcmp(type, "ascii") == 0) {
		if (img.type == 2 || img.type == 5)
			// save a grayscale image to a text file
			save_ASCII_pgm(img, new_file);
		else if (img.type == 3 || img.type == 6)
			// save a rgb image to a text file
			save_ASCII_ppm(img, new_file);
	} else {
		if (img.type == 5 || img.type == 2)
			// save a grayscale image to a binary file
			save_binary_pgm(img, new_file);
		else if (img.type == 6 || img.type == 3)
			// save a rgb image to a binary file
			save_binary_ppm(img, new_file);
	}
	fclose(new_file);
	free(type);

	printf("Saved %s\n", filename);
}
