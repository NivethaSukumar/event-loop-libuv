#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "uv.h"
#include <time.h>
#include <stdlib.h>
double percentage;
uv_loop_t *loop;
uv_async_t async;

// srand(time(NULL));   // Initialization, should only be called once.

void fake_download(uv_work_t *req) {
    int size = *((int*) req->data);
    int downloaded = 0;
    while (downloaded < size) {
        percentage = downloaded*100.0/size;
        async.data = (void*) &percentage;
        uv_async_send(&async);

        uv_sleep(1);
        downloaded += (200+rand())%1000; // can only download max 1000bytes/sec,
                                           // but at least a 200;
    }
}

void print_progress(uv_async_t *handle) {
    double percentage = *((double*) handle->data);
    fprintf(stderr, "Downloaded %.2f%%\n", percentage);
}


//cleaning up of the watcher
void after(uv_work_t *req, int status) {
    int size = *((int*) req->data);

    fprintf(stderr, "Download complete %d\n", size);
    uv_close((uv_handle_t*) &async, NULL);
    fprintf(stdout, "Status is %d", status);
}

void gui(void * args)
{
    (void)args;
    uv_loop_t* guiEventLoop = uv_default_loop();
    uv_async_init(guiEventLoop, &async, print_progress);
    uv_run(guiEventLoop, UV_RUN_DEFAULT);
}

int main() 
{
    loop = uv_default_loop();

    uv_work_t req;
    int size = 10240;
    req.data = (void*) &size;

    uv_thread_t gui_thread_id;
    // run a GUI thread
    // Get inputs from user
    // Add work to the queue 
    // Print the output to the user once its ready
    uv_thread_create(&gui_thread_id, gui, NULL);
    // Queing work runs in a separate thread
    uv_queue_work(loop, &req, fake_download, after);
    uv_thread_join(&gui_thread_id);

    return uv_run(loop, UV_RUN_DEFAULT);
}