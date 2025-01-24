// from grpc
#pragma once
#include <string>

namespace ai_matrix {

enum StatusCode : int32_t {
    OK = 0,
    CANCELLED = 1,
    UNKNOWN = 2,
    INVALID_ARGUMENT = 3,
    DEADLINE_EXCEEDED = 4,
    NOT_FOUND = 5,
    ALREADY_EXISTS = 6,
    PERMISSION_DENIED = 7,
    UNAUTHENTICATED = 16,
    RESOURCE_EXHAUSTED = 8,
    FAILED_PRECONDITION = 9,
    ABORTED = 10,
    OUT_OF_RANGE = 11,
    UNIMPLEMENTED = 12,
    INTERNAL = 13,
    UNAVAILABLE = 14,
    DATA_LOSS = 15,
    DO_NOT_USE = -1
};

class Status {
 public:
    Status() = default;

    Status(StatusCode code, const std::string& message)
        : code_(code), message_(message) {}

    static const Status OK;
    static const Status CANCELLED;

    StatusCode code() const { return code_; }
    const std::string& message() const { return message_; }

    bool ok() const { return code_ == StatusCode::OK; }

 private:
    StatusCode code_ = StatusCode::OK;
    std::string message_;
};

}  // namespace ai_matrix


namespace std {

std::ostream& operator<<(std::ostream& os, const ai_matrix::Status& status);
std::string to_string(const ai_matrix::Status& status);

}  // namespace std
