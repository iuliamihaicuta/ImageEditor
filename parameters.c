// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elementar.h"
#include "parameters.h"

// allocate memory for a kernel
double **alloc_kernel(void)
{
	double **kernel;
	// memory allocation for the line adresses
	kernel = (double **)malloc(3 * sizeof(double *));

	if (!kernel)
		fprintf(stderr, "malloc() failed\n");

	for (int i = 0; i < 3; i++) {
		// allocate memory for each individual value
		kernel[i] = calloc(3, sizeof(double));
		if (!kernel) {
			fprintf(stderr, "calloc() failed\n");
			while (i >= 0) {
				free(kernel[i]);
				i--;
			}
			free(kernel);
		}
	}

	return kernel;
}

// free kernel memory
void free_kernel(double **kernel)
{
	for (int i = 0; i < 3; i++)
		free(kernel[i]);
	free(kernel);
}

// limit the number to the unsigned char range
double clamp(double x)
{
	if (x < 0)
		x = 0;
	else if (x > 255)
		x = 255;
	return x;
}

// find the values for each pixel color
colors find_value(int i, int j, pixel_map image, double **kernel, int div)
{
	i--;
	j--;
	int x = j;

	double red[3][3], green[3][3], blue[3][3];

	// find the intermediate values for each color
	for (int m = 0; m < 3; ++m) {
		for (int n = 0; n < 3; ++n) {
			red[m][n] = kernel[m][n] * image.map[i][j].red;
			green[m][n] = kernel[m][n] * image.map[i][j].green;
			blue[m][n] = kernel[m][n] * image.map[i][j].blue;
			j++;
		}
		i++;
		j = x;
	}

	double r = 0, g = 0, b = 0;

	// find the sum of the intermediate values for each color
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j) {
			r += red[i][j];
			g += green[i][j];
			b += blue[i][j];
		}

	// find the final values and save them in a colors variable
	colors value;
	value.red = clamp(r / (div * 1.0));
	value.green = clamp(g / (div * 1.0));
	value.blue = clamp(b / (div * 1.0));

	return value;
}

// save the new values in the appropriate pixel_map structure
pixel_map save_values(image img, double **kernel, int div)
{
	int nl = img.image.nr_lines;
	int nc = img.image.nr_collums;
	pixel_map edit;
	// if there is no selection, apply the filter to the whole image
	if (img.index[0] == -1) {
		edit = copy_map(edit, img.image);
		for (int i = 1; i < nl - 1; ++i)
			for (int j = 1; j < nc - 1; ++j) {
				// find the values for each pixel color
				colors value = find_value(i, j, img.image, kernel, div);
				edit.map[i][j].red = value.red;
				edit.map[i][j].green = value.green;
				edit.map[i][j].blue = value.blue;
			}
		free_pixel_map(img.image);
	// otherwise, apply the filter to the selection
	} else {
		edit = copy_map(edit, img.selection);
		for (int i = img.index[1]; i < img.index[3]; ++i)
			for (int j = img.index[0]; j < img.index[2]; ++j) {
				if (i - 1 >= 0 && j - 1 >= 0 && i + 1 < nl && j + 1 < nc) {
					// find the values for each pixel color
					colors value = find_value(i, j, img.image, kernel, div);
					edit.map[i - img.index[1]][j - img.index[0]].red =
					value.red;
					edit.map[i - img.index[1]][j - img.index[0]].green =
					value.green;
					edit.map[i - img.index[1]][j - img.index[0]].blue =
					value.blue;
				}
			}
		free_pixel_map(img.selection);
	}
	return edit;
}

// save the new values in the structure
image new_image(image img, double **kernel, int div)
{
	// choose where the parameter will be applied
	if (img.index[0] == -1)
		img.image = save_values(img, kernel, div);
	else
		img.selection = save_values(img, kernel, div);

	free_kernel(kernel);

	return img;
}

// apply the edge parameter
image apply_edge(image img)
{
	double **edge = alloc_kernel();

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			edge[i][j] = -1;
	edge[1][1] = 8;

	img = new_image(img, edge, 1);
	printf("APPLY EDGE done\n");

	return img;
}

// apply the sharpen parameter
image apply_sharpen(image img)
{
	double **sharpen = alloc_kernel();

	sharpen[0][0] = 0;
	sharpen[0][1] = -1;
	sharpen[0][2] = 0;
	sharpen[1][0] = -1;
	sharpen[1][1] = 5;
	sharpen[1][2] = -1;
	sharpen[2][0] = 0;
	sharpen[2][1] = -1;
	sharpen[2][2] = 0;

	img = new_image(img, sharpen, 1);
	printf("APPLY SHARPEN done\n");

	return img;
}

// apply the blur parameter
image apply_blur(image img)
{
	double **blur = alloc_kernel();

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			blur[i][j] = 1;

	img = new_image(img, blur, 9);
	printf("APPLY BLUR done\n");

	return img;
}

// apply the gaussian blur parameter
image apply_gaussian_blur(image img)
{
	double **gauss = alloc_kernel();

	gauss[0][0] = 1;
	gauss[0][1] = 2;
	gauss[0][2] = 1;
	gauss[1][0] = 2;
	gauss[1][1] = 4;
	gauss[1][2] = 2;
	gauss[2][0] = 1;
	gauss[2][1] = 2;
	gauss[2][2] = 1;

	img = new_image(img, gauss, 16);
	printf("APPLY GAUSSIAN_BLUR done\n");

	return img;
}

// find the parameter ti be applied
image apply_parameter(image img, char *c)
{
	// select the parameter from the full command
	char *parameter = (char *)malloc((strlen(c) - 5) * sizeof(char));
	if (!parameter)
		fprintf(stderr, "malloc() failed\n");
	memcpy(parameter, &c[6], strlen(c) - 5);

	// apply the desired parameter or display the error message
	if (strcmp(parameter, "EDGE") == 0)
		img = apply_edge(img);
	else if (strcmp(parameter, "SHARPEN") == 0)
		img = apply_sharpen(img);
	else if (strcmp(parameter, "BLUR") == 0)
		img = apply_blur(img);
	else if (strcmp(parameter, "GAUSSIAN_BLUR") == 0)
		img = apply_gaussian_blur(img);
	else
		printf("APPLY parameter invalid\n");

	free(parameter);

	return img;
}
