#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "src/raylib.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Usage: %s <input image>\n", argv[0]);
        return 1;
    }

    // convert image path to header file name
    // e.g. "res/BirdSprite.png" -> "res/BirdSprite.h"
    char *output = malloc(strlen(argv[1]) + 1); // little extra but ok
    strcpy(output, argv[1]);
    char *dot = strrchr(output, '.');
    if(dot == NULL) {
        printf("error: input image must have an extension\n");
        return 1;
    }
    strcpy(dot, ".h");

    // convert image
    Image img = LoadImage(argv[1]);
    if(!ExportImageAsCode(img, output)) {
        printf("error exporting image as code\n");
        return 1;
    }
}
