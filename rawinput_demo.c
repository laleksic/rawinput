#include <stdio.h>
#include <string.h>
#include "rawinput.h"

#ifdef _WIN32
#include <windows.h>
#endif

#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

/* Event handling callbacks */
void sample_on_key_down(void *tag, Raw_Key key, void *_) {
    printf("Device %d: Key %s was pressed.\n", (int)tag, raw_key_str(key));
}

void sample_on_key_up(void *tag, Raw_Key key, void *_) {
    printf("Device %d: Key %s was released.\n", (int)tag, raw_key_str(key));
}

void sample_on_rel(void *tag, Raw_Axis axis, int delta, void *_) {
    printf("Device %d: Movement %d on axis %s.\n", (int)tag, delta, raw_axis_str(axis));
}

void sample_on_plug(int idx, void *_) {
    // Use a counter to assign a unique id to each device as it's plugged in
    int *next_tag = (int*)_;
    int tag = *next_tag;
    *next_tag = tag + 1;

    raw_open(idx, (void*)tag);
    printf("Device %d at idx %d plugged.\n", (int)tag, idx);
}

void sample_on_unplug(void *tag, void *_) {
    raw_close(tag);
    printf("Device %d unplugged.\n", (int)tag);
}
/**/

int main (int argc, char **argv) {
    // Initialize library
    raw_init();

    // Count available devices
    int dev_cnt = raw_dev_cnt();
    printf("Detected %d devices.\n", dev_cnt);

    #ifdef __linux__
    if (dev_cnt == 0) {
        /* Linux receives raw input events from all windows, however
         * the price is that the user that runs the program has to do
         * some system administration to make sure the /dev/input/event*
         * files are readable. Usually the simplest way is to just run
         * the program as root. */
        puts("Did you make sure that /dev/input/event* files are readable by this user?");
    }
    #endif

    // Open all available devices
    int next_tag = 0;
    for (int i=0; i<dev_cnt; ++i) {
        raw_open(i, (void*)(next_tag++));
    }

    // Register event handlers
    raw_on_key_down(sample_on_key_down, 0);
    raw_on_key_up(sample_on_key_up, 0);
    raw_on_rel(sample_on_rel, 0);
    raw_on_unplug(sample_on_unplug, 0);
    raw_on_plug(sample_on_plug, &next_tag);

    #ifdef _WIN32
    /* Windows requires us to have a window open to receive raw input events,
     * and we only receive events from windows owned by our process.
     *
     * Usually you'd open a window anyway and use a cross-platform library
     * like SDL, but here for simplicity we use win32 and only open a window
     * when running on windows. */
    WNDCLASS wc;
    HINSTANCE instance = 0;
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = "sample_class";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("sample_class", "sample", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 640, 480, HWND_DESKTOP, NULL, instance, NULL);
    #endif

    puts("Entering event loop.");
    #ifdef _WIN32
    puts("Close the window to terminate.");
    #else
    puts("Press Control+C to terminate.");
    #endif

    #ifdef _WIN32
    /* Here we handle the "windowing" events that rawinput doesn't handle;
     * Again usually you'd use cross-platform SDL code here instead of win32 */
    MSG msg;
    while (GetMessage(&msg, hwnd, 0, 0) >= 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        // Check for raw input and call event handlers
        raw_poll();
    }
    #elif __linux__
    for (;;) {
        // Check for raw input and call event handlers
        raw_poll();
    }
    #endif

    // Clean up
    raw_quit();
}
