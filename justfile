
build: embed-all
    clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a src/main.c -o Swallow

tools:
    clang -framework IOKit -framework Cocoa lib/libraylib.a embed.c -o embed

embed-all:
    just embed res/BirdSprite.png
    just embed res/Fruit.png

embed IMAGE: tools
    ./embed {{IMAGE}}

clean:
    rm embed &> /dev/null || true
    rm Swallow &> /dev/null || true
    rm res/*.h &> /dev/null || true
