py::array_t<float> P220::inference_np(py::list& imgs) {
    LOG(INFO) << "n_images: " << imgs.size();
    for (const py::handle& handle : imgs) {
        py::array_t<uint8_t> arr = py::cast<py::array_t<uint8_t>>(handle);
        auto buf_info = arr.request();
        LOG(INFO) << fmt::format("buf_info itemsize={}, size={}, format={}, ndim={}, shape={}, ptr={:x}",
                buf_info.itemsize, buf_info.size, buf_info.format, buf_info.ndim, buf_info.shape, (uint64_t)(buf_info.ptr));
    }

    return py::array_t<float>();
}
