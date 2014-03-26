#include "rfkpebble.h"

/*
** Forward declarations
*/

    static void init(void);
    static void deinit(void);
    int main(void);

/*
** Own storage
*/

    static Window *window = 0;
    static Layer *window_layer = 0;

static void init(void) {
    window = window_create();
    if (window == 0) {
        // error...
        return;
    }
    window_set_fullscreen(window, true);
    window_layer = window_get_root_layer(window);
    window_stack_push(window, true);
}

static void deinit(void) {
    if (window != 0) {
        window_destroy(window);
    }
}

int main(void) {

    init();
    app_event_loop();
    deinit();
}
