#include <iostream>
#include <string>
#include <unordered_map>

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
    using StatusCodeTable = std::unordered_map<StatusCode, std::string>;

 public:
    Status() = default;

    Status(StatusCode code, const std::string& error_message)
        : code_(code), error_message_(error_message) {}

    static const Status& OK;

    StatusCode error_code() const { return code_; }
    const std::string& error_message() const { return error_message_; }

    bool ok() const { return code_ == StatusCode::OK; }

    // TEST
    static const StatusCodeTable& code_2_str_;
    const std::string& code_str() const {
        static std::string unknown("UNKNOWN");
        auto it = code_2_str_.find(code_);
        return (it == code_2_str_.end() ? unknown : it->second);
    }

 private:
    StatusCode code_ = StatusCode::OK;
    std::string error_message_;
};

const Status& Status::OK = Status();

// TEST
// awk '{printf "\{StatusCode::%s, \"%s\"\},\n", $1, $1}' /tmp/a.txt
const Status::StatusCodeTable& Status::code_2_str_ = {
    {StatusCode::OK, "OK"},
    {StatusCode::CANCELLED, "CANCELLED"},
    {StatusCode::UNKNOWN, "UNKNOWN"},
    {StatusCode::INVALID_ARGUMENT, "INVALID_ARGUMENT"},
    {StatusCode::DEADLINE_EXCEEDED, "DEADLINE_EXCEEDED"},
    {StatusCode::NOT_FOUND, "NOT_FOUND"},
    {StatusCode::ALREADY_EXISTS, "ALREADY_EXISTS"},
    {StatusCode::PERMISSION_DENIED, "PERMISSION_DENIED"},
    {StatusCode::UNAUTHENTICATED, "UNAUTHENTICATED"},
    {StatusCode::RESOURCE_EXHAUSTED, "RESOURCE_EXHAUSTED"},
    {StatusCode::FAILED_PRECONDITION, "FAILED_PRECONDITION"},
    {StatusCode::ABORTED, "ABORTED"},
    {StatusCode::OUT_OF_RANGE, "OUT_OF_RANGE"},
    {StatusCode::UNIMPLEMENTED, "UNIMPLEMENTED"},
    {StatusCode::INTERNAL, "INTERNAL"},
    {StatusCode::UNAVAILABLE, "UNAVAILABLE"},
    {StatusCode::DATA_LOSS, "DATA_LOSS"},
    {StatusCode::DO_NOT_USE, "DO_NOT_USE"},
};

int main() {
    using namespace std;

    cout << Status::OK.ok() << endl;
    cout << Status::OK.code_str() << endl;

    return 0;
}

