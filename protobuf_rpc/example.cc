#include <google/protobuf/util/json_util.h>
std::string result;
google::protobuf::util::JsonPrintOptions options;
options.add_whitespace = true;
auto status = google::protobuf::util::MessageToJsonString(message, &result, options);
