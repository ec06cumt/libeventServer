// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: LOGIN_SC_IN.proto

#ifndef PROTOBUF_LOGIN_5fSC_5fIN_2eproto__INCLUDED
#define PROTOBUF_LOGIN_5fSC_5fIN_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
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

namespace Login {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_LOGIN_5fSC_5fIN_2eproto();
void protobuf_AssignDesc_LOGIN_5fSC_5fIN_2eproto();
void protobuf_ShutdownFile_LOGIN_5fSC_5fIN_2eproto();

class Login_SC;

// ===================================================================

class Login_SC : public ::google::protobuf::Message {
 public:
  Login_SC();
  virtual ~Login_SC();

  Login_SC(const Login_SC& from);

  inline Login_SC& operator=(const Login_SC& from) {
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
  static const Login_SC& default_instance();

  void Swap(Login_SC* other);

  // implements Message ----------------------------------------------

  Login_SC* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Login_SC& from);
  void MergeFrom(const Login_SC& from);
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

  // required string rtcode = 1;
  inline bool has_rtcode() const;
  inline void clear_rtcode();
  static const int kRtcodeFieldNumber = 1;
  inline const ::std::string& rtcode() const;
  inline void set_rtcode(const ::std::string& value);
  inline void set_rtcode(const char* value);
  inline void set_rtcode(const char* value, size_t size);
  inline ::std::string* mutable_rtcode();
  inline ::std::string* release_rtcode();
  inline void set_allocated_rtcode(::std::string* rtcode);

  // required int32 uid = 2;
  inline bool has_uid() const;
  inline void clear_uid();
  static const int kUidFieldNumber = 2;
  inline ::google::protobuf::int32 uid() const;
  inline void set_uid(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Login.Login_SC)
 private:
  inline void set_has_rtcode();
  inline void clear_has_rtcode();
  inline void set_has_uid();
  inline void clear_has_uid();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* rtcode_;
  ::google::protobuf::int32 uid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_LOGIN_5fSC_5fIN_2eproto();
  friend void protobuf_AssignDesc_LOGIN_5fSC_5fIN_2eproto();
  friend void protobuf_ShutdownFile_LOGIN_5fSC_5fIN_2eproto();

  void InitAsDefaultInstance();
  static Login_SC* default_instance_;
};
// ===================================================================


// ===================================================================

// Login_SC

// required string rtcode = 1;
inline bool Login_SC::has_rtcode() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Login_SC::set_has_rtcode() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Login_SC::clear_has_rtcode() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Login_SC::clear_rtcode() {
  if (rtcode_ != &::google::protobuf::internal::kEmptyString) {
    rtcode_->clear();
  }
  clear_has_rtcode();
}
inline const ::std::string& Login_SC::rtcode() const {
  return *rtcode_;
}
inline void Login_SC::set_rtcode(const ::std::string& value) {
  set_has_rtcode();
  if (rtcode_ == &::google::protobuf::internal::kEmptyString) {
    rtcode_ = new ::std::string;
  }
  rtcode_->assign(value);
}
inline void Login_SC::set_rtcode(const char* value) {
  set_has_rtcode();
  if (rtcode_ == &::google::protobuf::internal::kEmptyString) {
    rtcode_ = new ::std::string;
  }
  rtcode_->assign(value);
}
inline void Login_SC::set_rtcode(const char* value, size_t size) {
  set_has_rtcode();
  if (rtcode_ == &::google::protobuf::internal::kEmptyString) {
    rtcode_ = new ::std::string;
  }
  rtcode_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Login_SC::mutable_rtcode() {
  set_has_rtcode();
  if (rtcode_ == &::google::protobuf::internal::kEmptyString) {
    rtcode_ = new ::std::string;
  }
  return rtcode_;
}
inline ::std::string* Login_SC::release_rtcode() {
  clear_has_rtcode();
  if (rtcode_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = rtcode_;
    rtcode_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Login_SC::set_allocated_rtcode(::std::string* rtcode) {
  if (rtcode_ != &::google::protobuf::internal::kEmptyString) {
    delete rtcode_;
  }
  if (rtcode) {
    set_has_rtcode();
    rtcode_ = rtcode;
  } else {
    clear_has_rtcode();
    rtcode_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required int32 uid = 2;
inline bool Login_SC::has_uid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Login_SC::set_has_uid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Login_SC::clear_has_uid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Login_SC::clear_uid() {
  uid_ = 0;
  clear_has_uid();
}
inline ::google::protobuf::int32 Login_SC::uid() const {
  return uid_;
}
inline void Login_SC::set_uid(::google::protobuf::int32 value) {
  set_has_uid();
  uid_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Login

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_LOGIN_5fSC_5fIN_2eproto__INCLUDED
