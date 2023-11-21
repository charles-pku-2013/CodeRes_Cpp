// 文本互转 parse protobuf
// from DebugString()
#include "google/protobuf/message.h"
#include "google/protobuf/text_format.h"
if (!google::protobuf::TextFormat::ParseFromString(content, &cfg)) {
    LOG(ERROR) << "Fail to parsxe model config file " << config_file;
    return false;
}

// to json
#include "google/protobuf/util/json_util.h"
using protobuf::util::MessageToJsonString;
JsonPrintOptions opts;
opts.add_whitespace = true;
opts.always_print_primitive_fields = true;
const auto& status = MessageToJsonString(response, output, opts);
if (!status.ok()) {...}

// from json
LIBPROTOBUF_EXPORT util::Status JsonStringToMessage(const string& input,
                                   Message* message,
                                   const JsonParseOptions& options);

// compare protobuf messages is equal
google::protobuf::util::MessageDifferencer::Equals(proto1, proto2);
