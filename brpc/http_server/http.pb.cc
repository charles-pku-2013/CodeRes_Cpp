// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: http.proto

#include "http.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)

namespace example {
class HttpRequestDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<HttpRequest>
      _instance;
} _HttpRequest_default_instance_;
class HttpResponseDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<HttpResponse>
      _instance;
} _HttpResponse_default_instance_;
}  // namespace example
namespace protobuf_http_2eproto {
static void InitDefaultsHttpRequest() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::example::_HttpRequest_default_instance_;
    new (ptr) ::example::HttpRequest();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::example::HttpRequest::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_HttpRequest =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsHttpRequest}, {}};

static void InitDefaultsHttpResponse() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::example::_HttpResponse_default_instance_;
    new (ptr) ::example::HttpResponse();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::example::HttpResponse::InitAsDefaultInstance();
}

::google::protobuf::internal::SCCInfo<0> scc_info_HttpResponse =
    {{ATOMIC_VAR_INIT(::google::protobuf::internal::SCCInfoBase::kUninitialized), 0, InitDefaultsHttpResponse}, {}};

void InitDefaults() {
  ::google::protobuf::internal::InitSCC(&scc_info_HttpRequest.base);
  ::google::protobuf::internal::InitSCC(&scc_info_HttpResponse.base);
}

::google::protobuf::Metadata file_level_metadata[2];
const ::google::protobuf::ServiceDescriptor* file_level_service_descriptors[4];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::example::HttpRequest, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::example::HttpRequest, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::example::HttpResponse, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::example::HttpResponse, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 5, sizeof(::example::HttpRequest)},
  { 5, 10, sizeof(::example::HttpResponse)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::example::_HttpRequest_default_instance_),
  reinterpret_cast<const ::google::protobuf::Message*>(&::example::_HttpResponse_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  AssignDescriptors(
      "http.proto", schemas, file_default_instances, TableStruct::offsets,
      file_level_metadata, NULL, file_level_service_descriptors);
}

void protobuf_AssignDescriptorsOnce() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 2);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\nhttp.proto\022\007example\"\r\n\013HttpRequest\"\016\n\014"
      "HttpResponse2\177\n\013HttpService\0223\n\004Echo\022\024.ex"
      "ample.HttpRequest\032\025.example.HttpResponse"
      "\022;\n\014EchoProtobuf\022\024.example.HttpRequest\032\025"
      ".example.HttpResponse2L\n\013FileService\022=\n\016"
      "default_method\022\024.example.HttpRequest\032\025.e"
      "xample.HttpResponse2\262\001\n\014QueueService\0224\n\005"
      "start\022\024.example.HttpRequest\032\025.example.Ht"
      "tpResponse\0223\n\004stop\022\024.example.HttpRequest"
      "\032\025.example.HttpResponse\0227\n\010getstats\022\024.ex"
      "ample.HttpRequest\032\025.example.HttpResponse"
      "2G\n\016HttpSSEService\0225\n\006stream\022\024.example.H"
      "ttpRequest\032\025.example.HttpResponseB\003\200\001\001"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 518);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "http.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static ::google::protobuf::internal::once_flag once;
  ::google::protobuf::internal::call_once(once, AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_http_2eproto
namespace example {

// ===================================================================

void HttpRequest::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

HttpRequest::HttpRequest()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_http_2eproto::scc_info_HttpRequest.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:example.HttpRequest)
}
HttpRequest::HttpRequest(const HttpRequest& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:example.HttpRequest)
}

void HttpRequest::SharedCtor() {
}

HttpRequest::~HttpRequest() {
  // @@protoc_insertion_point(destructor:example.HttpRequest)
  SharedDtor();
}

void HttpRequest::SharedDtor() {
}

void HttpRequest::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* HttpRequest::descriptor() {
  ::protobuf_http_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_http_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const HttpRequest& HttpRequest::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_http_2eproto::scc_info_HttpRequest.base);
  return *internal_default_instance();
}


void HttpRequest::Clear() {
// @@protoc_insertion_point(message_clear_start:example.HttpRequest)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool HttpRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:example.HttpRequest)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, _internal_metadata_.mutable_unknown_fields()));
  }
success:
  // @@protoc_insertion_point(parse_success:example.HttpRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:example.HttpRequest)
  return false;
#undef DO_
}

void HttpRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:example.HttpRequest)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:example.HttpRequest)
}

::google::protobuf::uint8* HttpRequest::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:example.HttpRequest)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:example.HttpRequest)
  return target;
}

size_t HttpRequest::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:example.HttpRequest)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void HttpRequest::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:example.HttpRequest)
  GOOGLE_DCHECK_NE(&from, this);
  const HttpRequest* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const HttpRequest>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:example.HttpRequest)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:example.HttpRequest)
    MergeFrom(*source);
  }
}

void HttpRequest::MergeFrom(const HttpRequest& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:example.HttpRequest)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void HttpRequest::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:example.HttpRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void HttpRequest::CopyFrom(const HttpRequest& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:example.HttpRequest)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HttpRequest::IsInitialized() const {
  return true;
}

void HttpRequest::Swap(HttpRequest* other) {
  if (other == this) return;
  InternalSwap(other);
}
void HttpRequest::InternalSwap(HttpRequest* other) {
  using std::swap;
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata HttpRequest::GetMetadata() const {
  protobuf_http_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_http_2eproto::file_level_metadata[kIndexInFileMessages];
}


// ===================================================================

void HttpResponse::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

HttpResponse::HttpResponse()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  ::google::protobuf::internal::InitSCC(
      &protobuf_http_2eproto::scc_info_HttpResponse.base);
  SharedCtor();
  // @@protoc_insertion_point(constructor:example.HttpResponse)
}
HttpResponse::HttpResponse(const HttpResponse& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:example.HttpResponse)
}

void HttpResponse::SharedCtor() {
}

HttpResponse::~HttpResponse() {
  // @@protoc_insertion_point(destructor:example.HttpResponse)
  SharedDtor();
}

void HttpResponse::SharedDtor() {
}

void HttpResponse::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ::google::protobuf::Descriptor* HttpResponse::descriptor() {
  ::protobuf_http_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_http_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const HttpResponse& HttpResponse::default_instance() {
  ::google::protobuf::internal::InitSCC(&protobuf_http_2eproto::scc_info_HttpResponse.base);
  return *internal_default_instance();
}


void HttpResponse::Clear() {
// @@protoc_insertion_point(message_clear_start:example.HttpResponse)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool HttpResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:example.HttpResponse)
  for (;;) {
    ::std::pair<::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, _internal_metadata_.mutable_unknown_fields()));
  }
success:
  // @@protoc_insertion_point(parse_success:example.HttpResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:example.HttpResponse)
  return false;
#undef DO_
}

void HttpResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:example.HttpResponse)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:example.HttpResponse)
}

::google::protobuf::uint8* HttpResponse::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:example.HttpResponse)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:example.HttpResponse)
  return target;
}

size_t HttpResponse::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:example.HttpResponse)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void HttpResponse::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:example.HttpResponse)
  GOOGLE_DCHECK_NE(&from, this);
  const HttpResponse* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const HttpResponse>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:example.HttpResponse)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:example.HttpResponse)
    MergeFrom(*source);
  }
}

void HttpResponse::MergeFrom(const HttpResponse& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:example.HttpResponse)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

}

void HttpResponse::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:example.HttpResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void HttpResponse::CopyFrom(const HttpResponse& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:example.HttpResponse)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HttpResponse::IsInitialized() const {
  return true;
}

void HttpResponse::Swap(HttpResponse* other) {
  if (other == this) return;
  InternalSwap(other);
}
void HttpResponse::InternalSwap(HttpResponse* other) {
  using std::swap;
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
}

::google::protobuf::Metadata HttpResponse::GetMetadata() const {
  protobuf_http_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_http_2eproto::file_level_metadata[kIndexInFileMessages];
}


// ===================================================================

HttpService::~HttpService() {}

const ::google::protobuf::ServiceDescriptor* HttpService::descriptor() {
  protobuf_http_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_http_2eproto::file_level_service_descriptors[0];
}

const ::google::protobuf::ServiceDescriptor* HttpService::GetDescriptor() {
  return descriptor();
}

void HttpService::Echo(::google::protobuf::RpcController* controller,
                         const ::example::HttpRequest*,
                         ::example::HttpResponse*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method Echo() not implemented.");
  done->Run();
}

void HttpService::EchoProtobuf(::google::protobuf::RpcController* controller,
                         const ::example::HttpRequest*,
                         ::example::HttpResponse*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method EchoProtobuf() not implemented.");
  done->Run();
}

void HttpService::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                             ::google::protobuf::RpcController* controller,
                             const ::google::protobuf::Message* request,
                             ::google::protobuf::Message* response,
                             ::google::protobuf::Closure* done) {
  GOOGLE_DCHECK_EQ(method->service(), protobuf_http_2eproto::file_level_service_descriptors[0]);
  switch(method->index()) {
    case 0:
      Echo(controller,
             ::google::protobuf::down_cast<const ::example::HttpRequest*>(request),
             ::google::protobuf::down_cast< ::example::HttpResponse*>(response),
             done);
      break;
    case 1:
      EchoProtobuf(controller,
             ::google::protobuf::down_cast<const ::example::HttpRequest*>(request),
             ::google::protobuf::down_cast< ::example::HttpResponse*>(response),
             done);
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

const ::google::protobuf::Message& HttpService::GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::example::HttpRequest::default_instance();
    case 1:
      return ::example::HttpRequest::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(method->input_type());
  }
}

const ::google::protobuf::Message& HttpService::GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::example::HttpResponse::default_instance();
    case 1:
      return ::example::HttpResponse::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(method->output_type());
  }
}

HttpService_Stub::HttpService_Stub(::google::protobuf::RpcChannel* channel)
  : channel_(channel), owns_channel_(false) {}
HttpService_Stub::HttpService_Stub(
    ::google::protobuf::RpcChannel* channel,
    ::google::protobuf::Service::ChannelOwnership ownership)
  : channel_(channel),
    owns_channel_(ownership == ::google::protobuf::Service::STUB_OWNS_CHANNEL) {}
HttpService_Stub::~HttpService_Stub() {
  if (owns_channel_) delete channel_;
}

void HttpService_Stub::Echo(::google::protobuf::RpcController* controller,
                              const ::example::HttpRequest* request,
                              ::example::HttpResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(0),
                       controller, request, response, done);
}
void HttpService_Stub::EchoProtobuf(::google::protobuf::RpcController* controller,
                              const ::example::HttpRequest* request,
                              ::example::HttpResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(1),
                       controller, request, response, done);
}
// ===================================================================

FileService::~FileService() {}

const ::google::protobuf::ServiceDescriptor* FileService::descriptor() {
  protobuf_http_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_http_2eproto::file_level_service_descriptors[1];
}

const ::google::protobuf::ServiceDescriptor* FileService::GetDescriptor() {
  return descriptor();
}

void FileService::default_method(::google::protobuf::RpcController* controller,
                         const ::example::HttpRequest*,
                         ::example::HttpResponse*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method default_method() not implemented.");
  done->Run();
}

void FileService::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                             ::google::protobuf::RpcController* controller,
                             const ::google::protobuf::Message* request,
                             ::google::protobuf::Message* response,
                             ::google::protobuf::Closure* done) {
  GOOGLE_DCHECK_EQ(method->service(), protobuf_http_2eproto::file_level_service_descriptors[1]);
  switch(method->index()) {
    case 0:
      default_method(controller,
             ::google::protobuf::down_cast<const ::example::HttpRequest*>(request),
             ::google::protobuf::down_cast< ::example::HttpResponse*>(response),
             done);
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

const ::google::protobuf::Message& FileService::GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::example::HttpRequest::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(method->input_type());
  }
}

const ::google::protobuf::Message& FileService::GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::example::HttpResponse::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(method->output_type());
  }
}

FileService_Stub::FileService_Stub(::google::protobuf::RpcChannel* channel)
  : channel_(channel), owns_channel_(false) {}
FileService_Stub::FileService_Stub(
    ::google::protobuf::RpcChannel* channel,
    ::google::protobuf::Service::ChannelOwnership ownership)
  : channel_(channel),
    owns_channel_(ownership == ::google::protobuf::Service::STUB_OWNS_CHANNEL) {}
FileService_Stub::~FileService_Stub() {
  if (owns_channel_) delete channel_;
}

void FileService_Stub::default_method(::google::protobuf::RpcController* controller,
                              const ::example::HttpRequest* request,
                              ::example::HttpResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(0),
                       controller, request, response, done);
}
// ===================================================================

QueueService::~QueueService() {}

const ::google::protobuf::ServiceDescriptor* QueueService::descriptor() {
  protobuf_http_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_http_2eproto::file_level_service_descriptors[2];
}

const ::google::protobuf::ServiceDescriptor* QueueService::GetDescriptor() {
  return descriptor();
}

void QueueService::start(::google::protobuf::RpcController* controller,
                         const ::example::HttpRequest*,
                         ::example::HttpResponse*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method start() not implemented.");
  done->Run();
}

void QueueService::stop(::google::protobuf::RpcController* controller,
                         const ::example::HttpRequest*,
                         ::example::HttpResponse*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method stop() not implemented.");
  done->Run();
}

void QueueService::getstats(::google::protobuf::RpcController* controller,
                         const ::example::HttpRequest*,
                         ::example::HttpResponse*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method getstats() not implemented.");
  done->Run();
}

void QueueService::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                             ::google::protobuf::RpcController* controller,
                             const ::google::protobuf::Message* request,
                             ::google::protobuf::Message* response,
                             ::google::protobuf::Closure* done) {
  GOOGLE_DCHECK_EQ(method->service(), protobuf_http_2eproto::file_level_service_descriptors[2]);
  switch(method->index()) {
    case 0:
      start(controller,
             ::google::protobuf::down_cast<const ::example::HttpRequest*>(request),
             ::google::protobuf::down_cast< ::example::HttpResponse*>(response),
             done);
      break;
    case 1:
      stop(controller,
             ::google::protobuf::down_cast<const ::example::HttpRequest*>(request),
             ::google::protobuf::down_cast< ::example::HttpResponse*>(response),
             done);
      break;
    case 2:
      getstats(controller,
             ::google::protobuf::down_cast<const ::example::HttpRequest*>(request),
             ::google::protobuf::down_cast< ::example::HttpResponse*>(response),
             done);
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

const ::google::protobuf::Message& QueueService::GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::example::HttpRequest::default_instance();
    case 1:
      return ::example::HttpRequest::default_instance();
    case 2:
      return ::example::HttpRequest::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(method->input_type());
  }
}

const ::google::protobuf::Message& QueueService::GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::example::HttpResponse::default_instance();
    case 1:
      return ::example::HttpResponse::default_instance();
    case 2:
      return ::example::HttpResponse::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(method->output_type());
  }
}

QueueService_Stub::QueueService_Stub(::google::protobuf::RpcChannel* channel)
  : channel_(channel), owns_channel_(false) {}
QueueService_Stub::QueueService_Stub(
    ::google::protobuf::RpcChannel* channel,
    ::google::protobuf::Service::ChannelOwnership ownership)
  : channel_(channel),
    owns_channel_(ownership == ::google::protobuf::Service::STUB_OWNS_CHANNEL) {}
QueueService_Stub::~QueueService_Stub() {
  if (owns_channel_) delete channel_;
}

void QueueService_Stub::start(::google::protobuf::RpcController* controller,
                              const ::example::HttpRequest* request,
                              ::example::HttpResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(0),
                       controller, request, response, done);
}
void QueueService_Stub::stop(::google::protobuf::RpcController* controller,
                              const ::example::HttpRequest* request,
                              ::example::HttpResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(1),
                       controller, request, response, done);
}
void QueueService_Stub::getstats(::google::protobuf::RpcController* controller,
                              const ::example::HttpRequest* request,
                              ::example::HttpResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(2),
                       controller, request, response, done);
}
// ===================================================================

HttpSSEService::~HttpSSEService() {}

const ::google::protobuf::ServiceDescriptor* HttpSSEService::descriptor() {
  protobuf_http_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_http_2eproto::file_level_service_descriptors[3];
}

const ::google::protobuf::ServiceDescriptor* HttpSSEService::GetDescriptor() {
  return descriptor();
}

void HttpSSEService::stream(::google::protobuf::RpcController* controller,
                         const ::example::HttpRequest*,
                         ::example::HttpResponse*,
                         ::google::protobuf::Closure* done) {
  controller->SetFailed("Method stream() not implemented.");
  done->Run();
}

void HttpSSEService::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                             ::google::protobuf::RpcController* controller,
                             const ::google::protobuf::Message* request,
                             ::google::protobuf::Message* response,
                             ::google::protobuf::Closure* done) {
  GOOGLE_DCHECK_EQ(method->service(), protobuf_http_2eproto::file_level_service_descriptors[3]);
  switch(method->index()) {
    case 0:
      stream(controller,
             ::google::protobuf::down_cast<const ::example::HttpRequest*>(request),
             ::google::protobuf::down_cast< ::example::HttpResponse*>(response),
             done);
      break;
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      break;
  }
}

const ::google::protobuf::Message& HttpSSEService::GetRequestPrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::example::HttpRequest::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(method->input_type());
  }
}

const ::google::protobuf::Message& HttpSSEService::GetResponsePrototype(
    const ::google::protobuf::MethodDescriptor* method) const {
  GOOGLE_DCHECK_EQ(method->service(), descriptor());
  switch(method->index()) {
    case 0:
      return ::example::HttpResponse::default_instance();
    default:
      GOOGLE_LOG(FATAL) << "Bad method index; this should never happen.";
      return *::google::protobuf::MessageFactory::generated_factory()
          ->GetPrototype(method->output_type());
  }
}

HttpSSEService_Stub::HttpSSEService_Stub(::google::protobuf::RpcChannel* channel)
  : channel_(channel), owns_channel_(false) {}
HttpSSEService_Stub::HttpSSEService_Stub(
    ::google::protobuf::RpcChannel* channel,
    ::google::protobuf::Service::ChannelOwnership ownership)
  : channel_(channel),
    owns_channel_(ownership == ::google::protobuf::Service::STUB_OWNS_CHANNEL) {}
HttpSSEService_Stub::~HttpSSEService_Stub() {
  if (owns_channel_) delete channel_;
}

void HttpSSEService_Stub::stream(::google::protobuf::RpcController* controller,
                              const ::example::HttpRequest* request,
                              ::example::HttpResponse* response,
                              ::google::protobuf::Closure* done) {
  channel_->CallMethod(descriptor()->method(0),
                       controller, request, response, done);
}

// @@protoc_insertion_point(namespace_scope)
}  // namespace example
namespace google {
namespace protobuf {
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::example::HttpRequest* Arena::CreateMaybeMessage< ::example::HttpRequest >(Arena* arena) {
  return Arena::CreateInternal< ::example::HttpRequest >(arena);
}
template<> GOOGLE_PROTOBUF_ATTRIBUTE_NOINLINE ::example::HttpResponse* Arena::CreateMaybeMessage< ::example::HttpResponse >(Arena* arena) {
  return Arena::CreateInternal< ::example::HttpResponse >(arena);
}
}  // namespace protobuf
}  // namespace google

// @@protoc_insertion_point(global_scope)