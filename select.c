// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elementar.h"
#include "select.h"

// swap two numbers between them
void swap(int *x, int *y)
{
	int aux = *x;
	*x = *y;
	*y = aux;
}

// check that the coordinates are valid
int is_valid(image img, int *index)
{
	int ok = 0;
	/* check that the parameters do not exceed the image size
	and are not negative */
	for (int i = 0; i < 4; ++i)
		if (i % 2 != 0 && index[i] > img.image.nr_lines)
			ok = 1;
		else if (i % 2 == 0 && index[i] > img.image.nr_collums)
			ok = 1;
		else if (index[i] < 0)
			ok = 1;

	// check if the selection size is larger than 0
	if (index[0] == index[2])
		ok = 1;
	if (index[1] == index[3])
		ok = 1;

	return ok;
}

// select the parameters from the command
int *find_selection_values(char *values)
{
	int *index = (int *)malloc(4 * sizeof(int));
	if (!index)
		fprintf(stderr, "malloc() failed\n");

	// select values one by one
	for (int i = 0; i < 3; ++i) {
		int j = 0;
		while (values[j] != ' ')
			j++;
		char *aux = (char *)malloc(j + 1);
		if (!aux)
			fprintf(stderr, "malloc() failed\n");
		memcpy(aux, &values[0], j + 1);

		// convert from char to int
		index[i] = atoi(aux);
		memcpy(values, &values[j + 1], strlen(values) - j);

		free(aux);
	}
	index[3] = atoi(values);

	return index;
}

// check if the character is a number
int is_number(char c)
{
	if (c > 47 && c < 58)
		return 0;
	return 1;
}

// check if the command contains 4 numbers
int are_numbers(char *v)
{
	// check for different characters
	for (unsigned long i = 0; i < strlen(v); ++i) {
		if (is_number(v[i]) == 1 && v[i] != '-' && v[i] != ' ' && v[i] != '\0')
			return 1;
	}

	// counts the number of spaces between 2 numbers
	int nr_numbers = 0;
	for (unsigned long i = 1; i < strlen(v) - 1; ++i)
		if (is_number(v[i - 1]) == 0 && v[i] == ' ')
			if (is_number(v[i + 1]) == 0 || v[i + 1] == '-')
				nr_numbers++;

	// check if there are 4 numbers
	if (nr_numbers != 3)
		return 1;

	return 0;
}

image select_part(image img, char *c)
{
	// paste the selection back into the full image
	img = unite(img);

	// select the parameters from the command
	char *values = (char *)malloc(strlen(c) - 6);
	if (!values)
		fprintf(stderr, "malloc() failed\n");
	memcpy(values, &c[7], strlen(c) - 6);

	// check that the command is valid
	if (are_numbers(values) == 0) {
		int *index = find_selection_values(values);

		// check that the selection parameters have been entered in order
		if (index[0] > index[2])
			swap(&index[0], &index[2]);

		if (index[1] > index[3])
			swap(&index[1], &index[3]);

		// check if the selection is valid
		int ok = is_valid(img, index);

		if (ok == 0) {
			// release the previous selection
			if (img.index[0] != -1)
				free_pixel_map(img.selection);

			for (int i = 0; i < 4; ++i)
				img.index[i] = index[i];

			// check that the selection covers the entire image
			int k = 1;
			if (img.index[0] == 0 && img.index[2] == img.image.nr_collums)
				if (img.index[1] == 0 && img.index[3] == img.image.nr_lines) {
					img.index[0] = -1;
					k = 0;
				}
			if (k == 1) {
				// find the size of the selection
				int nl = img.index[3] - img.index[1];
				int nc = img.index[2] - img.index[0];

				img.selection = alloc_pixel_map(nl, nc);

				// save the values in the img.selection
				for (int i = 0; i < nl; ++i)
					for (int j = 0; j < nc; ++j) {
						img.selection.map[i][j] =
						img.image.map[i + img.index[1]][j + img.index[0]];
					}
			}
			printf("Selected");
			for (int i = 0; i < 4; ++i)
				printf(" %d", index[i]);
			printf("\n");

		} else {
			printf("Invalid set of coordinates\n");
		}
	free(index);
	} else {
		printf("Invalid command\n");
	}
	free(values);
	return img;
}

// switch selection to the full image
image select_all(image img)
{
	if (img.index[0] != -1) {
		// paste the selection back into the full image
		img = unite(img);
		free_pixel_map(img.selection);
		img.index[0] = -1;
	}
	printf("Selected ALL\n");

	return img;
}
