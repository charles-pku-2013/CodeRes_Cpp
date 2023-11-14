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
