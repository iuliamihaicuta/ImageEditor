// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elementar.h"

// memory allocation for image pixels
pixel_map alloc_pixel_map(int nl, int nc)
{
	pixel_map image;
	// memory allocation for pixel matrix line adresses
	image.map = (colors **)malloc(nl * sizeof(colors *));
	// defensive programming
	if (!image.map)
		fprintf(stderr, "malloc() failed\n");

	// pixel memory allocation
	for (int i = 0; i < nl; i++) {
		image.map[i] = (colors *)calloc(nc, sizeof(colors));

		if (!image.map) {
			fprintf(stderr, "calloc failed()\n");
			while (i >= 0) {
				free(image.map[i]);
				i--;
			}
			free(image.map);
		}
	}

	// save the number of rows and columns in the structure
	image.nr_lines = nl;
	image.nr_collums = nc;

	return image;
}

// free the image memory
void free_pixel_map(pixel_map image)
{
	for (int i = 0; i < image.nr_lines; ++i)
		free(image.map[i]);
	free(image.map);
}

// select the command from the string
char *read_command(char *c)
{
	// find out how many letters the first word has
	unsigned long i = 0;
	while (c[i] != ' ' && i < strlen(c))
		i++;

	// allocate memory and verify that it was performed successfully
	char *command = (char *)malloc((i + 1) * sizeof(char));
	if (!command)
		fprintf(stderr, "malloc() failed\n");

	// copy the word and return it
	memcpy(command, &c[0], i);
	command[i] = '\0';

	return command;
}

// read the command from stdin
char *read_full_command(void)
{
	// lenght - rememger de
	int lenght = 0;
	char c;
	char *input;

	// allocate memory and verify that it was performed successfully
	input = (char *)malloc(sizeof(char));
	if (!input)
		fprintf(stderr, "malloc() failed\n");

	// read the command character by character and reallocate memory
	while ((c = getchar()) != '\n' && c != EOF) {
		input = (char *)realloc(input, lenght + 1);
		if (!input)
			fprintf(stderr, "realloc() failed\n");
		input[lenght] = c;
		lenght++;
	}

	// mark the ending of the line and return the string
	input = (char *)realloc(input, lenght + 1);
	if (!input)
		fprintf(stderr, "realloc() failed\n");

	input[lenght] = '\0';

	return input;
}

// copy pixels of the image and its size
pixel_map copy_map(pixel_map image, pixel_map selection)
{
	// allocate memory for the new image
	image = alloc_pixel_map(selection.nr_lines, selection.nr_collums);

	// copy the initial image pixel by pixel
	for (int i = 0; i < image.nr_lines; ++i)
		for (int j = 0; j < image.nr_collums; ++j)
			image.map[i][j] = selection.map[i][j];

	return image;
}

// add the selection back to the image
image unite(image img)
{
	// the algorithm is executed only if img.selection exists
	if (img.index[0] != -1) {
		for (int i = img.index[1]; i < img.index[3]; ++i)
			for (int j = img.index[0]; j < img.index[2]; ++j)
			// the selection values are assigned to the image
				img.image.map[i][j] =
				img.selection.map[i - img.index[1]][j - img.index[0]];
	}
	return img;
}

// release all program resources
void exit_program(image img)
{
	if (img.type != -1) {
		free_pixel_map(img.image);

		if (img.index[0] != -1)
			free_pixel_map(img.selection);

		free(img.index);
		free(img.filename);
	}
}
