#include "FF.h"

#include <assert.h>
#include <stdio.h>

#include "args.h"
#include "utils/color.h"
#include "utils/debug.h"

#define ERR C_RED "ER " C_RESET
#define OK C_GRN "OK " C_RESET
void _FF_no_op_deallocator(void* data, size_t a, void* b) {}
FF* instance = NULL;

FF* FF_get_instance() {
    if (instance != NULL) return instance;
    char* model_path = getenv("FF_MODEL_PATH");
    if (model_path == NULL) {
        printif(FF_arg.verbose, "model path not set! Use default value\n");
        model_path = "FF_Model";
    }
    instance = FF_init(model_path);
    return instance;
}

FF* FF_init(const char* saved_model_dir) {
    FF* ff = malloc(sizeof(FF));
    ff->height = 0;
    ff->width = 0;
    ff->graph = TF_NewGraph();
    ff->status = TF_NewStatus();
    ff->sess_opts = TF_NewSessionOptions();
    TF_Buffer* run_opts = NULL;

    // const char* saved_model_dir = "lstm2/";
    const char* tags = "serve";
    const char* input_node = "serving_default_data";
    const char* output_node = "PartitionedCall";
    int ntags = 1;

    ff->session =
        TF_LoadSessionFromSavedModel(ff->sess_opts, run_opts, saved_model_dir,
                                     &tags, ntags, ff->graph, NULL, ff->status);
    if (TF_GetCode(ff->status) == TF_OK) {
        // TODO print debug
        printif(FF_arg.verbose, "TF_LoadSessionFromSavedModel OK\n");
    } else {
        printif(FF_arg.verbose, ERR "%s\n", TF_Message(ff->status));
        return NULL;
    }
    int n_inputs = 1;
    TF_Output inputs[1];  // = (TF_Output*)malloc(sizeof(TF_Output) * n_inputs);
    TF_Output input_0 = {TF_GraphOperationByName(ff->graph, input_node), 0};
    if (input_0.oper == NULL) {
        // TODO print denug
        printif(FF_arg.verbose, ERR "Failed TF_GraphOperationByName '%s'\n", input_node);
        return NULL;
    } else {
        printif(FF_arg.verbose, OK "TF_GraphOperationByName '%s' is OK\n", input_node);
    }
    ff->inputs[0] = input_0;
    TF_Output output_0 = {TF_GraphOperationByName(ff->graph, output_node), 0};
    if (output_0.oper == NULL) {
        printif(FF_arg.verbose, ERR "Failed TF_GraphOperationByName %s\n", output_node);
        return NULL;
    } else {
        printif(FF_arg.verbose, OK "TF_GraphOperationByName %s is OK\n", output_node);
    }
    ff->outputs[0] = output_0;

    for (int i = 0; i < sizeof(ff->inputs_values) / sizeof(ff->inputs_values[0]); i++) {
        ff->inputs_values[i] = NULL;
    }
    for (int i = 0; i < sizeof(ff->outputs_values) / sizeof(ff->outputs_values[0]); i++) {
        ff->outputs_values[i] = NULL;
    }

    return ff;
}

int FF_enhance(FF* ff, uint8_t* data, int width, int height, uint8_t** result, int* output_width, int* output_height) {
    int64_t dims[] = {height, width, 3};
    const int n_dims = 3;
    const int n_data = width * height * 3 * sizeof(uint8_t);
    if (ff == NULL) return 1;
    if (ff->width != width || ff->height != height || ff->inputs_values[0] == NULL) {
        if (ff->inputs_values[0] != NULL) {
            TF_DeleteTensor(ff->inputs_values[0]);
        }
        ff->inputs_values[0] = TF_NewTensor(
            TF_UINT8, dims, n_dims, data, n_data, &_FF_no_op_deallocator, NULL);
        if (ff->inputs_values[0] != NULL) {
            // TODO debug print
            printif(FF_arg.verbose, OK "TF_NewTensor is OK\n");
        } else {
            printif(FF_arg.verbose, ERR "Failed TF_NewTensor\n");
            return 1;
        }
    }

    TF_SessionRun(ff->session, NULL, ff->inputs, ff->inputs_values, 1,
                  ff->outputs, ff->outputs_values, 1, NULL, 0, NULL,
                  ff->status);

    if (TF_GetCode(ff->status) == TF_OK) {
        printif(FF_arg.verbose, OK "Session is OK\n");
    } else {
        printif(FF_arg.verbose, ERR "%s", TF_Message(ff->status));
    }

    void* buf = TF_TensorData(ff->outputs_values[0]);
    *result = (uint8_t*)buf;
    int64_t output_shape[4] = {0};
    assert(n_dims <= 4);
    for (int i = 0; i < n_dims; i++) {
        output_shape[i] = TF_Dim(ff->outputs_values[0], i);
    }
    *output_height = output_shape[0];
    *output_width = output_shape[1];
    TF_DeleteTensor(ff->outputs_values[0]);
    ff->outputs_values[0] = NULL;

    return 0;
}
void FF_close_instance() {
    if (instance != NULL) FF_destory(instance);
}
void FF_destory(FF* ff) {
    if (ff == NULL) return;
    if (ff->graph != NULL)
        TF_DeleteGraph(ff->graph);
    if (ff->session != NULL && ff->status != NULL) {
        TF_CloseSession(ff->session, ff->status);
        TF_DeleteSession(ff->session, ff->status);
    }
    if (ff->sess_opts != NULL)
        TF_DeleteSessionOptions(ff->sess_opts);
    if (ff->status != NULL)
        TF_DeleteStatus(ff->status);

    if (ff->inputs_values[0] != NULL)
        TF_DeleteTensor(ff->inputs_values[0]);
    if (ff->outputs_values[0] != NULL)
        TF_DeleteTensor(ff->outputs_values[0]);
    free(ff);
}