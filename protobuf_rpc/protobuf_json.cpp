#include "google/protobuf/util/json_util.h"

template<typename ProtoType>
std::string pb_2_json_string(const ProtoType *msg) {
    std::string strJson;
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    options.always_print_primitive_fields = true;
    options.always_print_enums_as_ints = false;
    options.preserve_proto_field_names = true;
    auto status = google::protobuf::util::MessageToJsonString(*msg, &strJson, options);
    if (!status.ok()) {
        LOG(ERROR) << "Failed to convert protobuf to json: " << status.error_message();
        return "";
    }
    return strJson;
}

template<typename ProtoType>
Json::Value pb_2_json(const ProtoType *msg) {
    std::string strJson = pb_2_json_string(msg);
    Json::Value     root;
    Json::Reader    reader;
    if (!reader.parse(strJson, root)) {
        LOG(ERROR) << "Failed to create json from pb msg: " << strJson;
        return Json::Value();
    }
    return root;
}

Json::Value jsonReq = pb_2_json(&request_pb);
if (pred_ctxt->has_common_features) {
    jsonReq["inputs"]["common_features"].removeMember("int64_val");
    jsonReq["inputs"]["common_features"]["feature_bundle"] = pred_ctxt->p_bundle_common->ToJson();
}
jsonReq["inputs"]["examples"].removeMember("int64_val");
for (const auto& bundle : pred_ctxt->p_bundle_vec) {
    jsonReq["inputs"]["examples"]["feature_bundles"].append(bundle->ToJson());
}
