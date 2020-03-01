// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: agent.proto

#ifndef PROTOBUF_agent_2eproto__INCLUDED
#define PROTOBUF_agent_2eproto__INCLUDED

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
void  protobuf_AddDesc_agent_2eproto();
void protobuf_AssignDesc_agent_2eproto();
void protobuf_ShutdownFile_agent_2eproto();

class Command;
class Command_Vector;

// ===================================================================

class Command_Vector : public ::google::protobuf::Message {
 public:
  Command_Vector();
  virtual ~Command_Vector();

  Command_Vector(const Command_Vector& from);

  inline Command_Vector& operator=(const Command_Vector& from) {
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
  static const Command_Vector& default_instance();

  void Swap(Command_Vector* other);

  // implements Message ----------------------------------------------

  Command_Vector* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Command_Vector& from);
  void MergeFrom(const Command_Vector& from);
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

  // required double X = 1;
  inline bool has_x() const;
  inline void clear_x();
  static const int kXFieldNumber = 1;
  inline double x() const;
  inline void set_x(double value);

  // required double Y = 2;
  inline bool has_y() const;
  inline void clear_y();
  static const int kYFieldNumber = 2;
  inline double y() const;
  inline void set_y(double value);

  // required double Z = 3;
  inline bool has_z() const;
  inline void clear_z();
  static const int kZFieldNumber = 3;
  inline double z() const;
  inline void set_z(double value);

  // @@protoc_insertion_point(class_scope:comm.Command.Vector)
 private:
  inline void set_has_x();
  inline void clear_has_x();
  inline void set_has_y();
  inline void clear_has_y();
  inline void set_has_z();
  inline void clear_has_z();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  double x_;
  double y_;
  double z_;
  friend void  protobuf_AddDesc_agent_2eproto();
  friend void protobuf_AssignDesc_agent_2eproto();
  friend void protobuf_ShutdownFile_agent_2eproto();

  void InitAsDefaultInstance();
  static Command_Vector* default_instance_;
};
// -------------------------------------------------------------------

class Command : public ::google::protobuf::Message {
 public:
  Command();
  virtual ~Command();

  Command(const Command& from);

  inline Command& operator=(const Command& from) {
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
  static const Command& default_instance();

  void Swap(Command* other);

  // implements Message ----------------------------------------------

  Command* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Command& from);
  void MergeFrom(const Command& from);
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

  typedef Command_Vector Vector;

  // accessors -------------------------------------------------------

  // required int32 priority = 1;
  inline bool has_priority() const;
  inline void clear_priority();
  static const int kPriorityFieldNumber = 1;
  inline ::google::protobuf::int32 priority() const;
  inline void set_priority(::google::protobuf::int32 value);

  // required int32 id = 2;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional bool travel = 3;
  inline bool has_travel() const;
  inline void clear_travel();
  static const int kTravelFieldNumber = 3;
  inline bool travel() const;
  inline void set_travel(bool value);

  // optional .comm.Command.Vector heading = 4;
  inline bool has_heading() const;
  inline void clear_heading();
  static const int kHeadingFieldNumber = 4;
  inline const ::comm::Command_Vector& heading() const;
  inline ::comm::Command_Vector* mutable_heading();
  inline ::comm::Command_Vector* release_heading();
  inline void set_allocated_heading(::comm::Command_Vector* heading);

  // optional bool minRadius = 5;
  inline bool has_minradius() const;
  inline void clear_minradius();
  static const int kMinRadiusFieldNumber = 5;
  inline bool minradius() const;
  inline void set_minradius(bool value);

  // optional double minRadiusValue = 6;
  inline bool has_minradiusvalue() const;
  inline void clear_minradiusvalue();
  static const int kMinRadiusValueFieldNumber = 6;
  inline double minradiusvalue() const;
  inline void set_minradiusvalue(double value);

  // optional bool gravity = 7;
  inline bool has_gravity() const;
  inline void clear_gravity();
  static const int kGravityFieldNumber = 7;
  inline bool gravity() const;
  inline void set_gravity(bool value);

  // optional .comm.Command.Vector gravityLocation = 8;
  inline bool has_gravitylocation() const;
  inline void clear_gravitylocation();
  static const int kGravityLocationFieldNumber = 8;
  inline const ::comm::Command_Vector& gravitylocation() const;
  inline ::comm::Command_Vector* mutable_gravitylocation();
  inline ::comm::Command_Vector* release_gravitylocation();
  inline void set_allocated_gravitylocation(::comm::Command_Vector* gravitylocation);

  // optional double gravityStrength = 9;
  inline bool has_gravitystrength() const;
  inline void clear_gravitystrength();
  static const int kGravityStrengthFieldNumber = 9;
  inline double gravitystrength() const;
  inline void set_gravitystrength(double value);

  // optional double gravitySigma = 10;
  inline bool has_gravitysigma() const;
  inline void clear_gravitysigma();
  static const int kGravitySigmaFieldNumber = 10;
  inline double gravitysigma() const;
  inline void set_gravitysigma(double value);

  // optional bool resetGravity = 11;
  inline bool has_resetgravity() const;
  inline void clear_resetgravity();
  static const int kResetGravityFieldNumber = 11;
  inline bool resetgravity() const;
  inline void set_resetgravity(bool value);

  // optional bool userForce = 12;
  inline bool has_userforce() const;
  inline void clear_userforce();
  static const int kUserForceFieldNumber = 12;
  inline bool userforce() const;
  inline void set_userforce(bool value);

  // optional .comm.Command.Vector forceVector = 13;
  inline bool has_forcevector() const;
  inline void clear_forcevector();
  static const int kForceVectorFieldNumber = 13;
  inline const ::comm::Command_Vector& forcevector() const;
  inline ::comm::Command_Vector* mutable_forcevector();
  inline ::comm::Command_Vector* release_forcevector();
  inline void set_allocated_forcevector(::comm::Command_Vector* forcevector);

  // @@protoc_insertion_point(class_scope:comm.Command)
 private:
  inline void set_has_priority();
  inline void clear_has_priority();
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_travel();
  inline void clear_has_travel();
  inline void set_has_heading();
  inline void clear_has_heading();
  inline void set_has_minradius();
  inline void clear_has_minradius();
  inline void set_has_minradiusvalue();
  inline void clear_has_minradiusvalue();
  inline void set_has_gravity();
  inline void clear_has_gravity();
  inline void set_has_gravitylocation();
  inline void clear_has_gravitylocation();
  inline void set_has_gravitystrength();
  inline void clear_has_gravitystrength();
  inline void set_has_gravitysigma();
  inline void clear_has_gravitysigma();
  inline void set_has_resetgravity();
  inline void clear_has_resetgravity();
  inline void set_has_userforce();
  inline void clear_has_userforce();
  inline void set_has_forcevector();
  inline void clear_has_forcevector();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int32 priority_;
  ::google::protobuf::int32 id_;
  ::comm::Command_Vector* heading_;
  double minradiusvalue_;
  bool travel_;
  bool minradius_;
  bool gravity_;
  bool resetgravity_;
  bool userforce_;
  ::comm::Command_Vector* gravitylocation_;
  double gravitystrength_;
  double gravitysigma_;
  ::comm::Command_Vector* forcevector_;
  friend void  protobuf_AddDesc_agent_2eproto();
  friend void protobuf_AssignDesc_agent_2eproto();
  friend void protobuf_ShutdownFile_agent_2eproto();

  void InitAsDefaultInstance();
  static Command* default_instance_;
};
// ===================================================================


// ===================================================================

// Command_Vector

// required double X = 1;
inline bool Command_Vector::has_x() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Command_Vector::set_has_x() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Command_Vector::clear_has_x() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Command_Vector::clear_x() {
  x_ = 0;
  clear_has_x();
}
inline double Command_Vector::x() const {
  // @@protoc_insertion_point(field_get:comm.Command.Vector.X)
  return x_;
}
inline void Command_Vector::set_x(double value) {
  set_has_x();
  x_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.Vector.X)
}

// required double Y = 2;
inline bool Command_Vector::has_y() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Command_Vector::set_has_y() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Command_Vector::clear_has_y() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Command_Vector::clear_y() {
  y_ = 0;
  clear_has_y();
}
inline double Command_Vector::y() const {
  // @@protoc_insertion_point(field_get:comm.Command.Vector.Y)
  return y_;
}
inline void Command_Vector::set_y(double value) {
  set_has_y();
  y_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.Vector.Y)
}

// required double Z = 3;
inline bool Command_Vector::has_z() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Command_Vector::set_has_z() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Command_Vector::clear_has_z() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Command_Vector::clear_z() {
  z_ = 0;
  clear_has_z();
}
inline double Command_Vector::z() const {
  // @@protoc_insertion_point(field_get:comm.Command.Vector.Z)
  return z_;
}
inline void Command_Vector::set_z(double value) {
  set_has_z();
  z_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.Vector.Z)
}

// -------------------------------------------------------------------

// Command

// required int32 priority = 1;
inline bool Command::has_priority() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Command::set_has_priority() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Command::clear_has_priority() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Command::clear_priority() {
  priority_ = 0;
  clear_has_priority();
}
inline ::google::protobuf::int32 Command::priority() const {
  // @@protoc_insertion_point(field_get:comm.Command.priority)
  return priority_;
}
inline void Command::set_priority(::google::protobuf::int32 value) {
  set_has_priority();
  priority_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.priority)
}

// required int32 id = 2;
inline bool Command::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Command::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Command::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Command::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Command::id() const {
  // @@protoc_insertion_point(field_get:comm.Command.id)
  return id_;
}
inline void Command::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.id)
}

// optional bool travel = 3;
inline bool Command::has_travel() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Command::set_has_travel() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Command::clear_has_travel() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Command::clear_travel() {
  travel_ = false;
  clear_has_travel();
}
inline bool Command::travel() const {
  // @@protoc_insertion_point(field_get:comm.Command.travel)
  return travel_;
}
inline void Command::set_travel(bool value) {
  set_has_travel();
  travel_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.travel)
}

// optional .comm.Command.Vector heading = 4;
inline bool Command::has_heading() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Command::set_has_heading() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Command::clear_has_heading() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Command::clear_heading() {
  if (heading_ != NULL) heading_->::comm::Command_Vector::Clear();
  clear_has_heading();
}
inline const ::comm::Command_Vector& Command::heading() const {
  // @@protoc_insertion_point(field_get:comm.Command.heading)
  return heading_ != NULL ? *heading_ : *default_instance_->heading_;
}
inline ::comm::Command_Vector* Command::mutable_heading() {
  set_has_heading();
  if (heading_ == NULL) heading_ = new ::comm::Command_Vector;
  // @@protoc_insertion_point(field_mutable:comm.Command.heading)
  return heading_;
}
inline ::comm::Command_Vector* Command::release_heading() {
  clear_has_heading();
  ::comm::Command_Vector* temp = heading_;
  heading_ = NULL;
  return temp;
}
inline void Command::set_allocated_heading(::comm::Command_Vector* heading) {
  delete heading_;
  heading_ = heading;
  if (heading) {
    set_has_heading();
  } else {
    clear_has_heading();
  }
  // @@protoc_insertion_point(field_set_allocated:comm.Command.heading)
}

// optional bool minRadius = 5;
inline bool Command::has_minradius() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void Command::set_has_minradius() {
  _has_bits_[0] |= 0x00000010u;
}
inline void Command::clear_has_minradius() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void Command::clear_minradius() {
  minradius_ = false;
  clear_has_minradius();
}
inline bool Command::minradius() const {
  // @@protoc_insertion_point(field_get:comm.Command.minRadius)
  return minradius_;
}
inline void Command::set_minradius(bool value) {
  set_has_minradius();
  minradius_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.minRadius)
}

// optional double minRadiusValue = 6;
inline bool Command::has_minradiusvalue() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void Command::set_has_minradiusvalue() {
  _has_bits_[0] |= 0x00000020u;
}
inline void Command::clear_has_minradiusvalue() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void Command::clear_minradiusvalue() {
  minradiusvalue_ = 0;
  clear_has_minradiusvalue();
}
inline double Command::minradiusvalue() const {
  // @@protoc_insertion_point(field_get:comm.Command.minRadiusValue)
  return minradiusvalue_;
}
inline void Command::set_minradiusvalue(double value) {
  set_has_minradiusvalue();
  minradiusvalue_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.minRadiusValue)
}

// optional bool gravity = 7;
inline bool Command::has_gravity() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void Command::set_has_gravity() {
  _has_bits_[0] |= 0x00000040u;
}
inline void Command::clear_has_gravity() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void Command::clear_gravity() {
  gravity_ = false;
  clear_has_gravity();
}
inline bool Command::gravity() const {
  // @@protoc_insertion_point(field_get:comm.Command.gravity)
  return gravity_;
}
inline void Command::set_gravity(bool value) {
  set_has_gravity();
  gravity_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.gravity)
}

// optional .comm.Command.Vector gravityLocation = 8;
inline bool Command::has_gravitylocation() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void Command::set_has_gravitylocation() {
  _has_bits_[0] |= 0x00000080u;
}
inline void Command::clear_has_gravitylocation() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void Command::clear_gravitylocation() {
  if (gravitylocation_ != NULL) gravitylocation_->::comm::Command_Vector::Clear();
  clear_has_gravitylocation();
}
inline const ::comm::Command_Vector& Command::gravitylocation() const {
  // @@protoc_insertion_point(field_get:comm.Command.gravityLocation)
  return gravitylocation_ != NULL ? *gravitylocation_ : *default_instance_->gravitylocation_;
}
inline ::comm::Command_Vector* Command::mutable_gravitylocation() {
  set_has_gravitylocation();
  if (gravitylocation_ == NULL) gravitylocation_ = new ::comm::Command_Vector;
  // @@protoc_insertion_point(field_mutable:comm.Command.gravityLocation)
  return gravitylocation_;
}
inline ::comm::Command_Vector* Command::release_gravitylocation() {
  clear_has_gravitylocation();
  ::comm::Command_Vector* temp = gravitylocation_;
  gravitylocation_ = NULL;
  return temp;
}
inline void Command::set_allocated_gravitylocation(::comm::Command_Vector* gravitylocation) {
  delete gravitylocation_;
  gravitylocation_ = gravitylocation;
  if (gravitylocation) {
    set_has_gravitylocation();
  } else {
    clear_has_gravitylocation();
  }
  // @@protoc_insertion_point(field_set_allocated:comm.Command.gravityLocation)
}

// optional double gravityStrength = 9;
inline bool Command::has_gravitystrength() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void Command::set_has_gravitystrength() {
  _has_bits_[0] |= 0x00000100u;
}
inline void Command::clear_has_gravitystrength() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void Command::clear_gravitystrength() {
  gravitystrength_ = 0;
  clear_has_gravitystrength();
}
inline double Command::gravitystrength() const {
  // @@protoc_insertion_point(field_get:comm.Command.gravityStrength)
  return gravitystrength_;
}
inline void Command::set_gravitystrength(double value) {
  set_has_gravitystrength();
  gravitystrength_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.gravityStrength)
}

// optional double gravitySigma = 10;
inline bool Command::has_gravitysigma() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void Command::set_has_gravitysigma() {
  _has_bits_[0] |= 0x00000200u;
}
inline void Command::clear_has_gravitysigma() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void Command::clear_gravitysigma() {
  gravitysigma_ = 0;
  clear_has_gravitysigma();
}
inline double Command::gravitysigma() const {
  // @@protoc_insertion_point(field_get:comm.Command.gravitySigma)
  return gravitysigma_;
}
inline void Command::set_gravitysigma(double value) {
  set_has_gravitysigma();
  gravitysigma_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.gravitySigma)
}

// optional bool resetGravity = 11;
inline bool Command::has_resetgravity() const {
  return (_has_bits_[0] & 0x00000400u) != 0;
}
inline void Command::set_has_resetgravity() {
  _has_bits_[0] |= 0x00000400u;
}
inline void Command::clear_has_resetgravity() {
  _has_bits_[0] &= ~0x00000400u;
}
inline void Command::clear_resetgravity() {
  resetgravity_ = false;
  clear_has_resetgravity();
}
inline bool Command::resetgravity() const {
  // @@protoc_insertion_point(field_get:comm.Command.resetGravity)
  return resetgravity_;
}
inline void Command::set_resetgravity(bool value) {
  set_has_resetgravity();
  resetgravity_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.resetGravity)
}

// optional bool userForce = 12;
inline bool Command::has_userforce() const {
  return (_has_bits_[0] & 0x00000800u) != 0;
}
inline void Command::set_has_userforce() {
  _has_bits_[0] |= 0x00000800u;
}
inline void Command::clear_has_userforce() {
  _has_bits_[0] &= ~0x00000800u;
}
inline void Command::clear_userforce() {
  userforce_ = false;
  clear_has_userforce();
}
inline bool Command::userforce() const {
  // @@protoc_insertion_point(field_get:comm.Command.userForce)
  return userforce_;
}
inline void Command::set_userforce(bool value) {
  set_has_userforce();
  userforce_ = value;
  // @@protoc_insertion_point(field_set:comm.Command.userForce)
}

// optional .comm.Command.Vector forceVector = 13;
inline bool Command::has_forcevector() const {
  return (_has_bits_[0] & 0x00001000u) != 0;
}
inline void Command::set_has_forcevector() {
  _has_bits_[0] |= 0x00001000u;
}
inline void Command::clear_has_forcevector() {
  _has_bits_[0] &= ~0x00001000u;
}
inline void Command::clear_forcevector() {
  if (forcevector_ != NULL) forcevector_->::comm::Command_Vector::Clear();
  clear_has_forcevector();
}
inline const ::comm::Command_Vector& Command::forcevector() const {
  // @@protoc_insertion_point(field_get:comm.Command.forceVector)
  return forcevector_ != NULL ? *forcevector_ : *default_instance_->forcevector_;
}
inline ::comm::Command_Vector* Command::mutable_forcevector() {
  set_has_forcevector();
  if (forcevector_ == NULL) forcevector_ = new ::comm::Command_Vector;
  // @@protoc_insertion_point(field_mutable:comm.Command.forceVector)
  return forcevector_;
}
inline ::comm::Command_Vector* Command::release_forcevector() {
  clear_has_forcevector();
  ::comm::Command_Vector* temp = forcevector_;
  forcevector_ = NULL;
  return temp;
}
inline void Command::set_allocated_forcevector(::comm::Command_Vector* forcevector) {
  delete forcevector_;
  forcevector_ = forcevector;
  if (forcevector) {
    set_has_forcevector();
  } else {
    clear_has_forcevector();
  }
  // @@protoc_insertion_point(field_set_allocated:comm.Command.forceVector)
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

#endif  // PROTOBUF_agent_2eproto__INCLUDED
