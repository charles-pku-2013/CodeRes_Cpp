// https://www.tensorflow.org/extend/adding_an_op#inputs_and_outputs
#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/shape_inference.h"
#include <iostream>

using namespace std;
using namespace tensorflow;

REGISTER_OP("ZeroOut")
    .Input("to_zero: int32")
    .Output("zeroed: int32")
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
    //   c->set_output(0, c->input(0));
        return Status::OK();
    });

class ZeroOutOp : public OpKernel {
public:
    explicit ZeroOutOp(OpKernelConstruction* context) : OpKernel(context) 
    { cout << "ZeroOutOp construct" << endl; }

    void Compute(OpKernelContext* context) override {
        cout << "ZeroOutOp doing compute" << endl;

        // Grab the input tensor
        const Tensor& input_tensor = context->input(0);
        auto input = input_tensor.flat_inner_dims<int32>();

        assert(input.rank() <= 2); // not support more than 2D

        std::cout << "num_inputs = " << context->num_inputs() << std::endl;
        std::cout << "input_tensor shape = " << input_tensor.shape() << std::endl;
        std::cout << "input vector size = " << input.size() << std::endl;
        std::cout << "input vector rank = " << input.rank() << std::endl;
        std::cout << "input vector dim_0 = " << input.dimension(0) << std::endl;
        std::cout << "input vector dim_1 = " << input.dimension(1) << std::endl;
        auto dims = input.dimensions();
        std::cout << "dims.rank = " << dims.rank() << std::endl;
        for (int i = 0; i < dims.rank(); ++i) {
            std::cout << "dims_" << i << " = " << dims[i] << std::endl;
        }
        
        // Create an output tensor
        Tensor* output_tensor = NULL;
        OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                    &output_tensor));
        auto output_flat = output_tensor->flat_inner_dims<int32>();

        // Set all but the first element of the output tensor to 0.
        for (int i = 0; i < output_flat.dimension(0); ++i) {
            for (int j = 0; j < output_flat.dimension(1); ++j) {
                if (0 == j)
                    output_flat(i, j) = input(i, j);
                else
                    output_flat(i, j) = 0;
            }
        } // for
    }

    // void Compute(OpKernelContext* context) override {
    //     cout << "ZeroOutOp doing compute" << endl;

    //     // Grab the input tensor
    //     const Tensor& input_tensor = context->input(0);
    //     auto input = input_tensor.flat<int32>();

    //     // Create an output tensor
    //     Tensor* output_tensor = NULL;
    //     OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
    //                 &output_tensor));
    //     auto output_flat = output_tensor->flat<int32>();

    //     // Set all but the first element of the output tensor to 0.
    //     const int N = input.size();
    //     for (int i = 1; i < N; i++) {
    //         output_flat(i) = 0;
    //     }

    //     // Preserve the first input value if possible.
    //     if (N > 0) output_flat(0) = input(0);
    // }
};
REGISTER_KERNEL_BUILDER(Name("ZeroOut").Device(DEVICE_CPU), ZeroOutOp);

