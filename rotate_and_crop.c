// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elementar.h"
#include "rotate_and_crop.h"

// rotate the image clockwise
pixel_map rotate(pixel_map image)
{
	pixel_map img = alloc_pixel_map(image.nr_collums, image.nr_lines);

	// save the new values in an auxiliary array
	for (int i = 0; i < img.nr_lines; ++i)
		for (int j = 0; j < img.nr_collums; ++j)
			img.map[i][j] = image.map[image.nr_lines - j - 1][i];

	free_pixel_map(image);
	return img;
}

// find out the desired way of rotating the image
pixel_map rotate_image(pixel_map image, char *c)
{
	// find the lenght and value of the angle
	char *angle = (char *)malloc(strlen(c) - 6);
	if (!angle)
		fprintf(stderr, "malloc() failed\n");
	memcpy(angle, &c[7], strlen(c) - 6);

	int x = atoi(angle);
	free(angle);
	int y = x;

	// check if the angle value is supported
	if (x % 90 == 0 && (x < 361 || x > -361)) {
	    if (x < 0)
		    x = 360 + x;
		// rotate the image clockwise as long as necesary
	    while (x) {
		    image = rotate(image);
		    x -= 90;
	    }
	    printf("Rotated %d\n", y);
	} else {
		printf("Unsupported rotation angle\n");
	}
	return image;
}

image crop(image img)
{
	// check if there is a selection
	if (img.index[0] != -1) {
		free_pixel_map(img.image);
		// the complete image recives the selection values
		img.image = copy_map(img.image, img.selection);

		free_pixel_map(img.selection);
		img.index[0] = -1;
	}
	printf("Image cropped\n");

	return img;
}
