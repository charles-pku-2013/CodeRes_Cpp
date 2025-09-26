#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>

namespace py = pybind11;

py::array_t<float> P220::inference(const std::vector<std::string>& in_files) {
    inference_impl(in_files);

    ResultTensorMap tensor(result_.data(), n_images_, n_outputs_, out_img_size_);

#ifndef PY_BIND
    return tensor;

#else
    const auto& dimensions = tensor.dimensions();
    DLOG(INFO) << "result tensor shape: " << fmt::to_string(dimensions);

    std::vector<std::size_t> strides;
    std::size_t cur_stride = sizeof(decltype(tensor)::Scalar);
    for (int i = dimensions.size() - 1; i >= 0; --i) {
        strides.emplace_back(cur_stride);
        cur_stride *= dimensions[i];
    }
    std::reverse(strides.begin(), strides.end());
    DLOG(INFO) << "result tensor strides: " << fmt::to_string(strides);

    /*
     * Using python to check
     * import numpy as np
     * mat = np.zeros((1000, 9, 65536), dtype=np.float32)
     * print(mat.strides)
     */

    return py::array_t<float>(
        tensor.dimensions(),
        strides,
        tensor.data(),
        py::cast(nullptr)
    );
#endif
}

PYBIND11_MODULE(p220, m) {
    m.doc() = "P220 module";
    py::class_<P220>(m, "P220")
        .def(py::init<std::string, std::string, std::string>(), py::arg("snap_path"),
             py::arg("upload_format") = "NHWC", py::arg("download_format") = "NHWC")
        .def("inference", &P220::inference, py::arg("input_files"));
        // .def("inference", &P220::inference, py::arg("input_files"), py::return_value_policy::reference_internal);
}
