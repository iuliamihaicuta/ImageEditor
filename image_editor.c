// Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elementar.h"
#include "load.h"
#include "select.h"
#include "rotate_and_crop.h"
#include "parameters.h"
#include "save.h"

int main(void)
{
	image img;
	img.type = -1;

	char *c = read_full_command();
	if (!c)
		fprintf(stderr, "malloc() failed\n");

	char *command = read_command(c);
	if (!command)
		fprintf(stderr, "malloc() failed\n");

	// check if the program is to be continued
	while (strcmp(command, "EXIT") != 0) {
		if (strcmp(command, "LOAD") == 0) {
			img = load_file(c, img);
		} else if (img.type == -1) {
			printf("No image loaded\n");
		} else if (strcmp(command, "ROTATE") == 0) {
			// choose where to apply the rotation command
			if (img.index[0] == -1)
				img.image = rotate_image(img.image, c);
			else
				img.selection = rotate_image(img.selection, c);
		} else if (strcmp(command, "CROP") == 0) {
			img = crop(img);
		} else if (strcmp(command, "SELECT") == 0) {
			// check what type of selection was chosen
			char check[3];
			memcpy(check, &c[7], 3);

			if (strcmp(check, "ALL") == 0)
				select_all(img);
			else if ((c[7] > 47 && c[7] < 58) || c[7] == '-')
				img = select_part(img, c);
			else
				printf("Invalid command\n");
		} else if (strcmp(command, "APPLY") == 0) {
			// check if the command has a parameter and if the image is rgb
			if (c[5] == '\0')
				printf("Invalid command\n");
			else if (img.type == 2 || img.type == 5)
				printf("Easy, Charlie Chaplin\n");
			else
				img = apply_parameter(img, c);
		} else if (strcmp(command, "SAVE") == 0) {
			save_image(img, c);
		} else {
			printf("Invalid command\n");
		}
		// release the memory allocated to the commands
		free(c);
		c = read_full_command();

		free(command);
		command = read_command(c);
	}

	// check for an image in memory and release all resources
	if (img.type == -1)
		printf("No image loaded\n");
	exit_program(img);

	free(c);
	free(command);

	return 0;
}
