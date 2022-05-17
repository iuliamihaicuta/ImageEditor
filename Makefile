# Copyright Mihaicuta Iulia-Andreea-Naomi 314CAb 2021-2022
# compiler setup
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# define targets
TARGETS=image_editor

build: $(TARGETS)

image_editor: image_editor.c
	$(CC) $(CFLAGS) image_editor.c elementar.c load.c save.c parameters.c select.c rotate_and_crop.c -lm -o image_editor


clean:
	rm -f $(TARGETS)

.PHONY: pack clean
