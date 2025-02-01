#include "src/raylib.h"
#include <stdio.h>

int main(int argc, char **argv) {
    printf("%d: %s\n", argc, argv[1]);
    Image img = LoadImage("res/BirdSprite.png");
    ExportImageAsCode(img, "res/BirdSprite.h");
}
