
for (std::size_t b = 0; b < n_batches_; ++b) {
    (void)read(ppreorder_pre[0], &pre_buffid, sizeof(int));
    (void)read(ppreorder_post[0], &post_buffid, sizeof(int));

    DLOG(INFO) << fmt::format("thread_reorder batch = {} pre_buffid = {}, post_buffid = {}", b,
                              pre_buffid, post_buffid);

    std::size_t start = b * batch_size_;
    std::size_t end = std::min(n_images_, start + batch_size_);
    std::size_t this_batch_size = end - start;

    // #pragma omp parallel for
    for (std::size_t j = 0; j < batch_size_; ++j) {
        // 得到的数据长度是 output[i].ddrimgsize
        std::size_t idx = j < this_batch_size ? j : 0;
        err = zebra_reorder(
            in_tensor_, gb_preprocessed_[pre_buffid][idx].data,  // src 原意想用0去凑够batch_size
            in_tensor_->hwacc_addrs[post_buffid][j].ddr_vaddr);  // dst 这里原来写成idx是不对的
        if (err) {
            throw std::runtime_error(
                fmt::format("zebra_reorder fail: {}", vaisw_embd_error::exception_message(err)));
        }
    }

    (void)write(ppresize_post[1], &pre_buffid, sizeof(int));
    (void)write(ppwrddr_pre[1], &post_buffid, sizeof(int));
}  // for b
