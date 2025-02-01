
build: embed-all
    clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a src/main.c -o Swallow

tools:
    clang -framework IOKit -framework Cocoa lib/libraylib.a embed.c -o embed

embed-all:
    just embed res/*.png

embed IMAGE: tools
    ./embed {{IMAGE}} 

clean:
    rm embed || true
    rm Swallow || true
    rm res/*.h
