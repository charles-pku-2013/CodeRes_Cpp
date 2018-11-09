#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "level_dnn_utils.h"
#include <iostream>
// #include <boost/format.hpp>

using namespace tensorflow;

REGISTER_OP("Tokenize")
    .Input("query: string")
    .Output("out: string")
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
          return Status::OK();
    });

REGISTER_OP("GetBigrams")
    .Input("query: string")
    .Output("out: string")
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
          return Status::OK();
    });

REGISTER_OP("GetUnigramsAndBigrams")
    .Input("query: string")
    .Output("out: string")
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
          return Status::OK();
    });

REGISTER_OP("TokenizeSparse")
    .Input("query: string")
    .Output("token_indices: int64")
    .Output("token_values: string")
    .Output("token_shapes: int64")
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
          return Status::OK();
    });

class TokenizerOp : public OpKernel {
public:
    explicit TokenizerOp(OpKernelConstruction* context) : OpKernel(context) {}

    void Compute(OpKernelContext* context) override {
        std::vector<std::string> out;

        // Grab the input tensor
        if (context->num_inputs() > 0) {
            const Tensor& input_tensor = context->input(0);
            auto input = input_tensor.flat<string>();
            // std::cout << "input_tensor shape = " << input_tensor.shape() << std::endl;
            // std::cout << "input vector rank = " << input.rank() << std::endl;
            // std::cout << "input vector size = " << input.size() << std::endl;
            // for (int i = 0; i < input.size(); ++i)
                // std::cout << boost::format("input[%d] = %s") % i % input(i) << std::endl;
            if (input.size())
                search_level::tokenize(input(0), &out);
        }

        Tensor* output_tensor = NULL;
        OP_REQUIRES_OK(context, context->allocate_output(
                0, TensorShape{static_cast<int64>(out.size())}, &output_tensor));
        auto output_flat = output_tensor->flat<string>();
        for (int i = 0; i < out.size(); ++i) {
            output_flat(i) = std::move(out[i]);
        }
    }
};
REGISTER_KERNEL_BUILDER(Name("Tokenize").Device(DEVICE_CPU), TokenizerOp);


class TokenizeSparseOp : public OpKernel {
public:
    explicit TokenizeSparseOp(OpKernelConstruction* context) : OpKernel(context) {}
    void Compute(OpKernelContext* context) override {
        Tensor* indices_tensor = NULL;
        Tensor* values_tensor = NULL;
        Tensor* shapes_tensor = NULL;
        const Tensor& input_tensor = context->input(0);
        auto input = input_tensor.flat<string>();
        const int N = input.size();
        std::vector<std::pair<int64, int64>> indices;
        std::vector<string> values;
        std::vector<int64> shapes;
        for (int i = 0; i < N; i++) {
            std::vector<string> out;
            search_level::tokenize(input(i), &out);
            for (int j = 0; j < out.size(); ++j) {
                indices.emplace_back(i, j);
                values.push_back(out[j]);
            }
            shapes.push_back(out.size());
        }
        OP_REQUIRES_OK(context, context->allocate_output(
            0, TensorShape{static_cast<int64>(values.size()), 1}, &indices_tensor));  // 2}, &indices_tensor));
        OP_REQUIRES_OK(context, context->allocate_output(
            1, TensorShape{static_cast<int64>(values.size())}, &values_tensor));
        OP_REQUIRES_OK(context, context->allocate_output(
            2, TensorShape{static_cast<int64>(shapes.size())}, &shapes_tensor));
        auto output_indices = indices_tensor->matrix<int64>();
        auto output_values = values_tensor->flat<string>();
        auto output_shapes = shapes_tensor->flat<int64>();
        for (int i = 0; i < indices.size(); ++i) {
            const std::pair<int, int>& index_pair = indices[i];
            // output_indices(i, 0) = index_pair.first;
            // output_indices(i, 1) = index_pair.second;
            output_indices(i, 0) = i;
        }
        for (int i = 0; i < values.size(); ++i) {
            output_values(i) = values[i];
        }
        // for (int i = 0; i < shapes.size(); ++i) {
        //     output_shapes(i) = shapes[i];
        // }
        output_shapes(0) = values.size();
    }
};
REGISTER_KERNEL_BUILDER(Name("TokenizeSparse").Device(DEVICE_CPU), TokenizeSparseOp);


class GetBigramsOp : public OpKernel {
public:
    explicit GetBigramsOp(OpKernelConstruction* context) : OpKernel(context) {}

    void Compute(OpKernelContext* context) override {
        std::vector<std::string> tokens, out;

        if (context->num_inputs() > 0) {
            const Tensor& input_tensor = context->input(0);
            auto input = input_tensor.flat<string>();

            tokens.reserve(input.size());
            for (int i = 0; i < input.size(); ++i) {
                tokens.emplace_back(std::move(input(i)));
            }
            search_level::getBigrams(tokens, &out);
        }

        Tensor* output_tensor = NULL;
        OP_REQUIRES_OK(context, context->allocate_output(
                0, TensorShape{static_cast<int64>(out.size())}, &output_tensor));
        auto output_flat = output_tensor->flat<string>();
        for (int i = 0; i < out.size(); ++i) {
            output_flat(i) = std::move(out[i]);
        }
    }
};
REGISTER_KERNEL_BUILDER(Name("GetBigrams").Device(DEVICE_CPU), GetBigramsOp);

class GetUnigramsAndBigramsOp : public OpKernel {
public:
    explicit GetUnigramsAndBigramsOp(OpKernelConstruction* context) : OpKernel(context) {}

    void Compute(OpKernelContext* context) override {
        std::vector<std::string> out;

        if (context->num_inputs() > 0) {
            const Tensor& input_tensor = context->input(0);
            auto input = input_tensor.flat<string>();
            // std::cout << "input_tensor shape = " << input_tensor.shape() << std::endl;
            // std::cout << "input vector rank = " << input.rank() << std::endl;
            // std::cout << "input vector size = " << input.size() << std::endl;
            // for (int i = 0; i < input.size(); ++i)
                // std::cout << boost::format("input[%d] = %s") % i % input(i) << std::endl;
            if (input.size())
                search_level::getUnigramsAndBigrams(input(0), &out);
        }

        Tensor* output_tensor = NULL;
        OP_REQUIRES_OK(context, context->allocate_output(
                0, TensorShape{static_cast<int64>(out.size())}, &output_tensor));
        auto output_flat = output_tensor->flat<string>();
        for (int i = 0; i < out.size(); ++i) {
            output_flat(i) = std::move(out[i]);
        }
    }
};
REGISTER_KERNEL_BUILDER(Name("GetUnigramsAndBigrams").Device(DEVICE_CPU), GetUnigramsAndBigramsOp);
