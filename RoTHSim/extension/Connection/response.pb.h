// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: response.proto

#ifndef PROTOBUF_response_2eproto__INCLUDED
#define PROTOBUF_response_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace comm {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_response_2eproto();
void protobuf_AssignDesc_response_2eproto();
void protobuf_ShutdownFile_response_2eproto();

class Ack;
class Error;

// ===================================================================

class Ack : public ::google::protobuf::Message {
 public:
  Ack();
  virtual ~Ack();

  Ack(const Ack& from);

  inline Ack& operator=(const Ack& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Ack& default_instance();

  void Swap(Ack* other);

  // implements Message ----------------------------------------------

  Ack* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Ack& from);
  void MergeFrom(const Ack& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int32 code = 1;
  inline bool has_code() const;
  inline void clear_code();
  static const int kCodeFieldNumber = 1;
  inline ::google::protobuf::int32 code() const;
  inline void set_code(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:comm.Ack)
 private:
  inline void set_has_code();
  inline void clear_has_code();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 code_;
  friend void  protobuf_AddDesc_response_2eproto();
  friend void protobuf_AssignDesc_response_2eproto();
  friend void protobuf_ShutdownFile_response_2eproto();

  void InitAsDefaultInstance();
  static Ack* default_instance_;
};
// -------------------------------------------------------------------

class Error : public ::google::protobuf::Message {
 public:
  Error();
  virtual ~Error();

  Error(const Error& from);

  inline Error& operator=(const Error& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Error& default_instance();

  void Swap(Error* other);

  // implements Message ----------------------------------------------

  Error* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Error& from);
  void MergeFrom(const Error& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string message = 1;
  inline bool has_message() const;
  inline void clear_message();
  static const int kMessageFieldNumber = 1;
  inline const ::std::string& message() const;
  inline void set_message(const ::std::string& value);
  inline void set_message(const char* value);
  inline void set_message(const char* value, size_t size);
  inline ::std::string* mutable_message();
  inline ::std::string* release_message();
  inline void set_allocated_message(::std::string* message);

  // @@protoc_insertion_point(class_scope:comm.Error)
 private:
  inline void set_has_message();
  inline void clear_has_message();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* message_;
  friend void  protobuf_AddDesc_response_2eproto();
  friend void protobuf_AssignDesc_response_2eproto();
  friend void protobuf_ShutdownFile_response_2eproto();

  void InitAsDefaultInstance();
  static Error* default_instance_;
};
// ===================================================================


// ===================================================================

// Ack

// required int32 code = 1;
inline bool Ack::has_code() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Ack::set_has_code() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Ack::clear_has_code() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Ack::clear_code() {
  code_ = 0;
  clear_has_code();
}
inline ::google::protobuf::int32 Ack::code() const {
  // @@protoc_insertion_point(field_get:comm.Ack.code)
  return code_;
}
inline void Ack::set_code(::google::protobuf::int32 value) {
  set_has_code();
  code_ = value;
  // @@protoc_insertion_point(field_set:comm.Ack.code)
}

// -------------------------------------------------------------------

// Error

// optional string message = 1;
inline bool Error::has_message() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Error::set_has_message() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Error::clear_has_message() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Error::clear_message() {
  if (message_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    message_->clear();
  }
  clear_has_message();
}
inline const ::std::string& Error::message() const {
  // @@protoc_insertion_point(field_get:comm.Error.message)
  return *message_;
}
inline void Error::set_message(const ::std::string& value) {
  set_has_message();
  if (message_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    message_ = new ::std::string;
  }
  message_->assign(value);
  // @@protoc_insertion_point(field_set:comm.Error.message)
}
inline void Error::set_message(const char* value) {
  set_has_message();
  if (message_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    message_ = new ::std::string;
  }
  message_->assign(value);
  // @@protoc_insertion_point(field_set_char:comm.Error.message)
}
inline void Error::set_message(const char* value, size_t size) {
  set_has_message();
  if (message_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    message_ = new ::std::string;
  }
  message_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:comm.Error.message)
}
inline ::std::string* Error::mutable_message() {
  set_has_message();
  if (message_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    message_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:comm.Error.message)
  return message_;
}
inline ::std::string* Error::release_message() {
  clear_has_message();
  if (message_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = message_;
    message_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Error::set_allocated_message(::std::string* message) {
  if (message_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete message_;
  }
  if (message) {
    set_has_message();
    message_ = message;
  } else {
    clear_has_message();
    message_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:comm.Error.message)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace comm

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_response_2eproto__INCLUDED
