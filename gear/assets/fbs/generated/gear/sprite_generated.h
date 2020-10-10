// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SPRITE_GEAR_ASSETS_H_
#define FLATBUFFERS_GENERATED_SPRITE_GEAR_ASSETS_H_

#include "flatbuffers/flatbuffers.h"

#include "common_generated.h"
#include "texture_generated.h"

namespace gear {
namespace assets {

struct UVs;

struct Rectangle;

struct Circle;

struct Point;

struct Object;
struct ObjectBuilder;

struct Sprite;
struct SpriteBuilder;

enum class Shape : uint8_t {
  NONE = 0,
  rectangle = 1,
  circle = 2,
  point = 3,
  MIN = NONE,
  MAX = point
};

inline const Shape (&EnumValuesShape())[4] {
  static const Shape values[] = {
    Shape::NONE,
    Shape::rectangle,
    Shape::circle,
    Shape::point
  };
  return values;
}

inline const char * const *EnumNamesShape() {
  static const char * const names[5] = {
    "NONE",
    "rectangle",
    "circle",
    "point",
    nullptr
  };
  return names;
}

inline const char *EnumNameShape(Shape e) {
  if (flatbuffers::IsOutRange(e, Shape::NONE, Shape::point)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesShape()[index];
}

template<typename T> struct ShapeTraits {
  static const Shape enum_value = Shape::NONE;
};

template<> struct ShapeTraits<gear::assets::Rectangle> {
  static const Shape enum_value = Shape::rectangle;
};

template<> struct ShapeTraits<gear::assets::Circle> {
  static const Shape enum_value = Shape::circle;
};

template<> struct ShapeTraits<gear::assets::Point> {
  static const Shape enum_value = Shape::point;
};

bool VerifyShape(flatbuffers::Verifier &verifier, const void *obj, Shape type);
bool VerifyShapeVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(2) UVs FLATBUFFERS_FINAL_CLASS {
 private:
  int16_t x0_;
  int16_t y0_;
  int16_t x1_;
  int16_t y1_;

 public:
  UVs() {
    memset(static_cast<void *>(this), 0, sizeof(UVs));
  }
  UVs(int16_t _x0, int16_t _y0, int16_t _x1, int16_t _y1)
      : x0_(flatbuffers::EndianScalar(_x0)),
        y0_(flatbuffers::EndianScalar(_y0)),
        x1_(flatbuffers::EndianScalar(_x1)),
        y1_(flatbuffers::EndianScalar(_y1)) {
  }
  int16_t x0() const {
    return flatbuffers::EndianScalar(x0_);
  }
  void mutate_x0(int16_t _x0) {
    flatbuffers::WriteScalar(&x0_, _x0);
  }
  int16_t y0() const {
    return flatbuffers::EndianScalar(y0_);
  }
  void mutate_y0(int16_t _y0) {
    flatbuffers::WriteScalar(&y0_, _y0);
  }
  int16_t x1() const {
    return flatbuffers::EndianScalar(x1_);
  }
  void mutate_x1(int16_t _x1) {
    flatbuffers::WriteScalar(&x1_, _x1);
  }
  int16_t y1() const {
    return flatbuffers::EndianScalar(y1_);
  }
  void mutate_y1(int16_t _y1) {
    flatbuffers::WriteScalar(&y1_, _y1);
  }
};
FLATBUFFERS_STRUCT_END(UVs, 8);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Rectangle FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float w_;
  float h_;

 public:
  Rectangle() {
    memset(static_cast<void *>(this), 0, sizeof(Rectangle));
  }
  Rectangle(float _x, float _y, float _w, float _h)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        w_(flatbuffers::EndianScalar(_w)),
        h_(flatbuffers::EndianScalar(_h)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  void mutate_x(float _x) {
    flatbuffers::WriteScalar(&x_, _x);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  void mutate_y(float _y) {
    flatbuffers::WriteScalar(&y_, _y);
  }
  float w() const {
    return flatbuffers::EndianScalar(w_);
  }
  void mutate_w(float _w) {
    flatbuffers::WriteScalar(&w_, _w);
  }
  float h() const {
    return flatbuffers::EndianScalar(h_);
  }
  void mutate_h(float _h) {
    flatbuffers::WriteScalar(&h_, _h);
  }
};
FLATBUFFERS_STRUCT_END(Rectangle, 16);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Circle FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float r_;

 public:
  Circle() {
    memset(static_cast<void *>(this), 0, sizeof(Circle));
  }
  Circle(float _x, float _y, float _r)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        r_(flatbuffers::EndianScalar(_r)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  void mutate_x(float _x) {
    flatbuffers::WriteScalar(&x_, _x);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  void mutate_y(float _y) {
    flatbuffers::WriteScalar(&y_, _y);
  }
  float r() const {
    return flatbuffers::EndianScalar(r_);
  }
  void mutate_r(float _r) {
    flatbuffers::WriteScalar(&r_, _r);
  }
};
FLATBUFFERS_STRUCT_END(Circle, 12);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Point FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;

 public:
  Point() {
    memset(static_cast<void *>(this), 0, sizeof(Point));
  }
  Point(float _x, float _y)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  void mutate_x(float _x) {
    flatbuffers::WriteScalar(&x_, _x);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  void mutate_y(float _y) {
    flatbuffers::WriteScalar(&y_, _y);
  }
};
FLATBUFFERS_STRUCT_END(Point, 8);

struct Object FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ObjectBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_SHAPE_TYPE = 6,
    VT_SHAPE = 8
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  flatbuffers::String *mutable_name() {
    return GetPointer<flatbuffers::String *>(VT_NAME);
  }
  bool KeyCompareLessThan(const Object *o) const {
    return *name() < *o->name();
  }
  int KeyCompareWithValue(const char *val) const {
    return strcmp(name()->c_str(), val);
  }
  gear::assets::Shape shape_type() const {
    return static_cast<gear::assets::Shape>(GetField<uint8_t>(VT_SHAPE_TYPE, 0));
  }
  const void *shape() const {
    return GetPointer<const void *>(VT_SHAPE);
  }
  template<typename T> const T *shape_as() const;
  const gear::assets::Rectangle *shape_as_rectangle() const {
    return shape_type() == gear::assets::Shape::rectangle ? static_cast<const gear::assets::Rectangle *>(shape()) : nullptr;
  }
  const gear::assets::Circle *shape_as_circle() const {
    return shape_type() == gear::assets::Shape::circle ? static_cast<const gear::assets::Circle *>(shape()) : nullptr;
  }
  const gear::assets::Point *shape_as_point() const {
    return shape_type() == gear::assets::Shape::point ? static_cast<const gear::assets::Point *>(shape()) : nullptr;
  }
  void *mutable_shape() {
    return GetPointer<void *>(VT_SHAPE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<uint8_t>(verifier, VT_SHAPE_TYPE) &&
           VerifyOffset(verifier, VT_SHAPE) &&
           VerifyShape(verifier, shape(), shape_type()) &&
           verifier.EndTable();
  }
};

template<> inline const gear::assets::Rectangle *Object::shape_as<gear::assets::Rectangle>() const {
  return shape_as_rectangle();
}

template<> inline const gear::assets::Circle *Object::shape_as<gear::assets::Circle>() const {
  return shape_as_circle();
}

template<> inline const gear::assets::Point *Object::shape_as<gear::assets::Point>() const {
  return shape_as_point();
}

struct ObjectBuilder {
  typedef Object Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Object::VT_NAME, name);
  }
  void add_shape_type(gear::assets::Shape shape_type) {
    fbb_.AddElement<uint8_t>(Object::VT_SHAPE_TYPE, static_cast<uint8_t>(shape_type), 0);
  }
  void add_shape(flatbuffers::Offset<void> shape) {
    fbb_.AddOffset(Object::VT_SHAPE, shape);
  }
  explicit ObjectBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ObjectBuilder &operator=(const ObjectBuilder &);
  flatbuffers::Offset<Object> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Object>(end);
    fbb_.Required(o, Object::VT_NAME);
    return o;
  }
};

inline flatbuffers::Offset<Object> CreateObject(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    gear::assets::Shape shape_type = gear::assets::Shape::NONE,
    flatbuffers::Offset<void> shape = 0) {
  ObjectBuilder builder_(_fbb);
  builder_.add_shape(shape);
  builder_.add_name(name);
  builder_.add_shape_type(shape_type);
  return builder_.Finish();
}

struct Object::Traits {
  using type = Object;
  static auto constexpr Create = CreateObject;
};

inline flatbuffers::Offset<Object> CreateObjectDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    gear::assets::Shape shape_type = gear::assets::Shape::NONE,
    flatbuffers::Offset<void> shape = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return gear::assets::CreateObject(
      _fbb,
      name__,
      shape_type,
      shape);
}

struct Sprite FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef SpriteBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEXTURE = 4,
    VT_SIZE = 6,
    VT_IMAGES = 8,
    VT_ORIGIN = 10,
    VT_COLLISIONMASK_TYPE = 12,
    VT_COLLISIONMASK = 14,
    VT_OBJECTS = 16
  };
  const gear::assets::Ref *texture() const {
    return GetPointer<const gear::assets::Ref *>(VT_TEXTURE);
  }
  gear::assets::Ref *mutable_texture() {
    return GetPointer<gear::assets::Ref *>(VT_TEXTURE);
  }
  const gear::assets::fvec2 *size() const {
    return GetStruct<const gear::assets::fvec2 *>(VT_SIZE);
  }
  gear::assets::fvec2 *mutable_size() {
    return GetStruct<gear::assets::fvec2 *>(VT_SIZE);
  }
  const flatbuffers::Vector<const gear::assets::UVs *> *images() const {
    return GetPointer<const flatbuffers::Vector<const gear::assets::UVs *> *>(VT_IMAGES);
  }
  flatbuffers::Vector<const gear::assets::UVs *> *mutable_images() {
    return GetPointer<flatbuffers::Vector<const gear::assets::UVs *> *>(VT_IMAGES);
  }
  const gear::assets::fvec2 *origin() const {
    return GetStruct<const gear::assets::fvec2 *>(VT_ORIGIN);
  }
  gear::assets::fvec2 *mutable_origin() {
    return GetStruct<gear::assets::fvec2 *>(VT_ORIGIN);
  }
  gear::assets::Shape collisionMask_type() const {
    return static_cast<gear::assets::Shape>(GetField<uint8_t>(VT_COLLISIONMASK_TYPE, 0));
  }
  const void *collisionMask() const {
    return GetPointer<const void *>(VT_COLLISIONMASK);
  }
  template<typename T> const T *collisionMask_as() const;
  const gear::assets::Rectangle *collisionMask_as_rectangle() const {
    return collisionMask_type() == gear::assets::Shape::rectangle ? static_cast<const gear::assets::Rectangle *>(collisionMask()) : nullptr;
  }
  const gear::assets::Circle *collisionMask_as_circle() const {
    return collisionMask_type() == gear::assets::Shape::circle ? static_cast<const gear::assets::Circle *>(collisionMask()) : nullptr;
  }
  const gear::assets::Point *collisionMask_as_point() const {
    return collisionMask_type() == gear::assets::Shape::point ? static_cast<const gear::assets::Point *>(collisionMask()) : nullptr;
  }
  void *mutable_collisionMask() {
    return GetPointer<void *>(VT_COLLISIONMASK);
  }
  const flatbuffers::Vector<flatbuffers::Offset<gear::assets::Object>> *objects() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<gear::assets::Object>> *>(VT_OBJECTS);
  }
  flatbuffers::Vector<flatbuffers::Offset<gear::assets::Object>> *mutable_objects() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Object>> *>(VT_OBJECTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TEXTURE) &&
           verifier.VerifyTable(texture()) &&
           VerifyField<gear::assets::fvec2>(verifier, VT_SIZE) &&
           VerifyOffset(verifier, VT_IMAGES) &&
           verifier.VerifyVector(images()) &&
           VerifyField<gear::assets::fvec2>(verifier, VT_ORIGIN) &&
           VerifyField<uint8_t>(verifier, VT_COLLISIONMASK_TYPE) &&
           VerifyOffset(verifier, VT_COLLISIONMASK) &&
           VerifyShape(verifier, collisionMask(), collisionMask_type()) &&
           VerifyOffset(verifier, VT_OBJECTS) &&
           verifier.VerifyVector(objects()) &&
           verifier.VerifyVectorOfTables(objects()) &&
           verifier.EndTable();
  }
};

template<> inline const gear::assets::Rectangle *Sprite::collisionMask_as<gear::assets::Rectangle>() const {
  return collisionMask_as_rectangle();
}

template<> inline const gear::assets::Circle *Sprite::collisionMask_as<gear::assets::Circle>() const {
  return collisionMask_as_circle();
}

template<> inline const gear::assets::Point *Sprite::collisionMask_as<gear::assets::Point>() const {
  return collisionMask_as_point();
}

struct SpriteBuilder {
  typedef Sprite Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_texture(flatbuffers::Offset<gear::assets::Ref> texture) {
    fbb_.AddOffset(Sprite::VT_TEXTURE, texture);
  }
  void add_size(const gear::assets::fvec2 *size) {
    fbb_.AddStruct(Sprite::VT_SIZE, size);
  }
  void add_images(flatbuffers::Offset<flatbuffers::Vector<const gear::assets::UVs *>> images) {
    fbb_.AddOffset(Sprite::VT_IMAGES, images);
  }
  void add_origin(const gear::assets::fvec2 *origin) {
    fbb_.AddStruct(Sprite::VT_ORIGIN, origin);
  }
  void add_collisionMask_type(gear::assets::Shape collisionMask_type) {
    fbb_.AddElement<uint8_t>(Sprite::VT_COLLISIONMASK_TYPE, static_cast<uint8_t>(collisionMask_type), 0);
  }
  void add_collisionMask(flatbuffers::Offset<void> collisionMask) {
    fbb_.AddOffset(Sprite::VT_COLLISIONMASK, collisionMask);
  }
  void add_objects(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Object>>> objects) {
    fbb_.AddOffset(Sprite::VT_OBJECTS, objects);
  }
  explicit SpriteBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  SpriteBuilder &operator=(const SpriteBuilder &);
  flatbuffers::Offset<Sprite> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Sprite>(end);
    return o;
  }
};

inline flatbuffers::Offset<Sprite> CreateSprite(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<gear::assets::Ref> texture = 0,
    const gear::assets::fvec2 *size = 0,
    flatbuffers::Offset<flatbuffers::Vector<const gear::assets::UVs *>> images = 0,
    const gear::assets::fvec2 *origin = 0,
    gear::assets::Shape collisionMask_type = gear::assets::Shape::NONE,
    flatbuffers::Offset<void> collisionMask = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Object>>> objects = 0) {
  SpriteBuilder builder_(_fbb);
  builder_.add_objects(objects);
  builder_.add_collisionMask(collisionMask);
  builder_.add_origin(origin);
  builder_.add_images(images);
  builder_.add_size(size);
  builder_.add_texture(texture);
  builder_.add_collisionMask_type(collisionMask_type);
  return builder_.Finish();
}

struct Sprite::Traits {
  using type = Sprite;
  static auto constexpr Create = CreateSprite;
};

inline flatbuffers::Offset<Sprite> CreateSpriteDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<gear::assets::Ref> texture = 0,
    const gear::assets::fvec2 *size = 0,
    const std::vector<gear::assets::UVs> *images = nullptr,
    const gear::assets::fvec2 *origin = 0,
    gear::assets::Shape collisionMask_type = gear::assets::Shape::NONE,
    flatbuffers::Offset<void> collisionMask = 0,
    std::vector<flatbuffers::Offset<gear::assets::Object>> *objects = nullptr) {
  auto images__ = images ? _fbb.CreateVectorOfStructs<gear::assets::UVs>(*images) : 0;
  auto objects__ = objects ? _fbb.CreateVectorOfSortedTables<gear::assets::Object>(objects) : 0;
  return gear::assets::CreateSprite(
      _fbb,
      texture,
      size,
      images__,
      origin,
      collisionMask_type,
      collisionMask,
      objects__);
}

inline bool VerifyShape(flatbuffers::Verifier &verifier, const void *obj, Shape type) {
  switch (type) {
    case Shape::NONE: {
      return true;
    }
    case Shape::rectangle: {
      return verifier.Verify<gear::assets::Rectangle>(static_cast<const uint8_t *>(obj), 0);
    }
    case Shape::circle: {
      return verifier.Verify<gear::assets::Circle>(static_cast<const uint8_t *>(obj), 0);
    }
    case Shape::point: {
      return verifier.Verify<gear::assets::Point>(static_cast<const uint8_t *>(obj), 0);
    }
    default: return true;
  }
}

inline bool VerifyShapeVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyShape(
        verifier,  values->Get(i), types->GetEnum<Shape>(i))) {
      return false;
    }
  }
  return true;
}

inline const gear::assets::Sprite *GetSprite(const void *buf) {
  return flatbuffers::GetRoot<gear::assets::Sprite>(buf);
}

inline const gear::assets::Sprite *GetSizePrefixedSprite(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<gear::assets::Sprite>(buf);
}

inline Sprite *GetMutableSprite(void *buf) {
  return flatbuffers::GetMutableRoot<Sprite>(buf);
}

inline bool VerifySpriteBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<gear::assets::Sprite>(nullptr);
}

inline bool VerifySizePrefixedSpriteBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<gear::assets::Sprite>(nullptr);
}

inline void FinishSpriteBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Sprite> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedSpriteBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Sprite> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace assets
}  // namespace gear

#endif  // FLATBUFFERS_GENERATED_SPRITE_GEAR_ASSETS_H_
