#define GRPC_RETURN_ERROR(err_code, msg) \
    do { \
        std::ostringstream _oss_stream; \
        _oss_stream << msg << std::flush; \
        std::string _err_msg = _oss_stream.str(); \
        LOG(ERROR) << _err_msg; \
        return grpc::Status(grpc::StatusCode::err_code, _err_msg); \
    } while (0)

#define GRPC_RETURN_ERROR_IF(cond, err_code, msg) \
    do { \
        if (cond) { \
            GRPC_RETURN_ERROR(err_code, msg); \
        } \
    } while (0)

#define KNN_SEARCH(handle, ...) \
    do { \
        try { \
            (handle)->search(__VA_ARGS__); \
        } catch (const std::exception &ex) { \
            LOG(ERROR) << "Knn search error: " << ex.what(); \
            GRPC_RETURN_ERROR(INTERNAL, "Knn search error: " << ex.what()); \
        } catch (...) { \
            LOG(ERROR) << "Knn search unknown error"; \
            GRPC_RETURN_ERROR(INTERNAL, "Knn search unknown error"); \
        } \
    } while (0)

#define BUILD_GRPC_STATUS(status, err_code, msg) \
    do { \
        std::ostringstream _oss_stream; \
        _oss_stream << msg << std::flush; \
        std::string _err_msg = _oss_stream.str(); \
        status = grpc::Status(grpc::StatusCode::err_code, _err_msg); \
    } while (0)

BUILD_GRPC_STATUS(status_, ABORTED,
        "Proxy client PrepareCall to " << svr_call_data_->svr_ctx().method() << " fail");

KNN_SEARCH(knn_bundle, 1, embedding_result.data(), top_k, scores.data(), labels.data());

GRPC_RETURN_ERROR_IF(!request->has_model_spec(),
            INVALID_ARGUMENT, "KnnPredSearch model_spec is empty!");
GRPC_RETURN_ERROR_IF(request->embedding_model_name().empty(),
            INVALID_ARGUMENT, "KnnPredSearch embedding_model_name not specified!");
GRPC_RETURN_ERROR_IF(request->knn_model_name().empty(),
            INVALID_ARGUMENT, "KnnPredSearch knn_model_name not specified!");
GRPC_RETURN_ERROR_IF(request->top_k() <= 0,
            INVALID_ARGUMENT, "KnnPredSearch top_k must be greater than 0!");

// get this model
ServableHandle<KnnPredServable> bundle;
auto status = core_->GetServableHandle(request->model_spec(), &bundle);
GRPC_RETURN_ERROR_IF(status != tensorflow::Status::OK(), UNAVAILABLE,
    "KnnPredSearch Requested model " << request->model_spec().name()
            << (request->model_spec().has_version() ?
                    "/" + std::to_string(request->model_spec().version().value()) : "")
            << " is unavailable!");

