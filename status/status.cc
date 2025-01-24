#include "status.h"
#include <sstream>

namespace ai_matrix {

const Status Status::OK = Status();
const Status Status::CANCELLED = Status(StatusCode::CANCELLED, "");

namespace {
std::string statuscode_2_string(int32_t code) {
    switch (code) {
        case StatusCode::OK:
            return "OK";
        case StatusCode::CANCELLED:
            return "CANCELLED";
        case StatusCode::INVALID_ARGUMENT:
            return "INVALID_ARGUMENT";
        case StatusCode::DEADLINE_EXCEEDED:
            return "DEADLINE_EXCEEDED";
        case StatusCode::NOT_FOUND:
            return "NOT_FOUND";
        case StatusCode::ALREADY_EXISTS:
            return "ALREADY_EXISTS";
        case StatusCode::PERMISSION_DENIED:
            return "PERMISSION_DENIED";
        case StatusCode::UNAUTHENTICATED:
            return "UNAUTHENTICATED";
        case StatusCode::RESOURCE_EXHAUSTED:
            return "RESOURCE_EXHAUSTED";
        case StatusCode::FAILED_PRECONDITION:
            return "FAILED_PRECONDITION";
        case StatusCode::ABORTED:
            return "ABORTED";
        case StatusCode::OUT_OF_RANGE:
            return "OUT_OF_RANGE";
        case StatusCode::UNIMPLEMENTED:
            return "UNIMPLEMENTED";
        case StatusCode::INTERNAL:
            return "INTERNAL";
        case StatusCode::UNAVAILABLE:
            return "UNAVAILABLE";
        case StatusCode::DATA_LOSS:
            return "DATA_LOSS";
        case StatusCode::DO_NOT_USE:
            return "DO_NOT_USE";
    }
    return "UNKNOWN";
}
} // namespace

}  // namespace ai_matrix


namespace std {

std::ostream& operator<<(std::ostream& os, const ai_matrix::Status& status) {
    os << ai_matrix::statuscode_2_string(status.code()) << " " << status.message();
    return os;
}

std::string to_string(const ai_matrix::Status& status) {
    std::stringstream ss;
    ss << status;
    return ss.str();
}

}  // namespace std
