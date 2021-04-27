// print rpc endpoint
brpc::Controller *cntl;
LOG(ERROR) << absl::StrFormat("Successfully request searcher from %s to %s",
    endpoint2str(cntl->local_side()).c_str(), endpoint2str(cntl->remote_side()).c_str());

