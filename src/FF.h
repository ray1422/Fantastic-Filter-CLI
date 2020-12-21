#ifndef FF_H
#define FF_H
#include <tensorflow/c/c_api.h>

typedef struct FF {
    TF_Graph* graph;
    TF_Status* status;
    TF_SessionOptions* sess_opts;
    TF_Session* session;
    TF_Output inputs[1];
    TF_Output outputs[1];
    TF_Tensor* inputs_values[1];
    TF_Tensor* outputs_values[1];
    TF_Buffer *run_opts;
    int height;
    int width;
} FF;

FF* FF_init();
extern FF* FF_instance;
// TF_SessionOptions* sessionOpts;
// TF_Buffer* run_opts;

FF* FF_get_instance();
void FF_close_instance();
void FF_destory(FF* ff);

int FF_enhance(FF* ff, uint8_t* data, int width, int height, uint8_t** result, int* output_width, int* output_height);

typedef enum _ff_status {
    FF_OK,
    FF_MODEL_NOT_FOUND,
} FF_STATUS;
#endif