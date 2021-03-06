***Mihaicuta Iulia 314CA 2021 - 2022***

**Image Editor**

The program aims to implement a photo editor that can fulfill
certain commands, namely:
- select a portion of the image
- rotate the entire image or a selection of it
- image resizing
- application of filters
- save the modified version

The program supports PGM and PPM images that are described by P2, P3, P5, and P6 magic-words.

We read from stdin a set of commands that are about to be prelucrate,
using read_full_command(). We extract the command, leaving aside the
parameters using read_command(). Using the repetitive structure while(),
the program is executed until the output command is entered : "EXIT". To
load a file into memory, use the "LOAD <FILENAME>" command; any other
commands entered before it will display the message "No image loaded"

Selecting a portion of the image is possible by reading from stdin:
"SELECT <x1> <y1> <x1> <y2>". If four parameters have not been entered
or if they are invalid, the message "Invalid command", respectively
"Invalid set of coordinates" will be displayed on the screen. Otherwise,
the selection and coordinates are saved in memory. If "SELECT ALL" is read from stdin, the program returns to a state in which no selection was made.

Reading the "ROTATE <ANGLE>" command will rotate the image clockwise if
the angle is positive or in the opposite direction otherwise. Accepted
angle values are +-90, +-180, +-270 and +-360. The rotate_image()
function can be applied to part or to the whole image. "CROP" command.
"CROP" reduces the image to the current selection, if any.

The "APPLY <PARAMETER>" command supports the following parameters: "EDGE",
"SHARPEN", "BLUR" and "GAUSSIAN_BLUR"; they can be applied to part or to
the whole image. The function can only be applied to rgb images.

If we want to save the modified image to a file, we enter the command
"SAVE <filename> [ascii]". If the "ascii" string is missing, the image
will be saved in a binary file. The header will be written in ascii text
regardless of the file type.

Any other command will display the message "Invalid command".
