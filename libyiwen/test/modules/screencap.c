#include "../../include/command.h"
#include <assert.h>
#include <stdio.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <X11/Xlib.h>


cairo_write_func_t my_write(void *colsure, char *data, unsigned int length){
    return length == fwrite(data, 1, length, stdout) ? CAIRO_STATUS_SUCCESS: CAIRO_STATUS_WRITE_ERROR;
}

int screencap(int argc, char** argv) {
    Display *disp;
    Window root;
    cairo_surface_t *surface;
    int scr;
    if( argc < 2) {
        fprintf(stderr,
                "Usage:\n"
                "    <%s> <display> [output file]\n"
                "        display The name of the display that you want to capture.\n"
                "                You can find it with command \"xdpyinfo\"\n"
                "        output  The picture you want to store. Figure will redirect\n"
                "                to stdout if you leave it blank.\n"
                , argv[0]);
    	return 1;
    }
    disp = XOpenDisplay( argv[1] );
    if( disp == NULL ){
    	fprintf(stderr, "Given display cannot be found, exiting: %s\n" , argv[1]);
    	return 1;
    }
    scr = DefaultScreen(disp);
    root = DefaultRootWindow(disp);
    surface = cairo_xlib_surface_create(disp, root, DefaultVisual(disp, scr),
                                                    DisplayWidth(disp, scr),
                                                    DisplayHeight(disp, scr));
    if (argc == 3)
        cairo_surface_write_to_png(surface, argv[2]);
    else
        cairo_surface_write_to_png_stream(surface, my_write, NULL); //(surface, argv[2]);

    cairo_surface_destroy(surface);
    return 0;
}

static int 
load(int cmd, void *arg){
    int error = 0;

    switch (cmd){
        case MOD_LOAD:
            printf("Screencap command installed\n");
            break;
        case MOD_UNLOAD:
            printf("Screencap command uninstalled.\n");
            break;
        default:
            error = -1;
            break;
    }
    return error;
}

const struct command_data data = {
    .name = "screencap",
    .desc = "fuckyou",
    .evh = &screencap,
};

COMMAND_MODULE(screencap, &data, load, NULL, main);
