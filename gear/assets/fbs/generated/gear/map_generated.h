// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MAP_GEAR_ASSETS_H_
#define FLATBUFFERS_GENERATED_MAP_GEAR_ASSETS_H_

#include "flatbuffers/flatbuffers.h"

#include "common_generated.h"

namespace gear {
namespace assets {

struct Layer;
struct LayerBuilder;

struct Map;
struct MapBuilder;

struct Layer FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef LayerBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_TILESET = 6,
    VT_WIDTH = 8,
    VT_HEIGHT = 10,
    VT_DATA = 12
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  flatbuffers::String *mutable_name() {
    return GetPointer<flatbuffers::String *>(VT_NAME);
  }
  const gear::assets::Ref *tileSet() const {
    return GetPointer<const gear::assets::Ref *>(VT_TILESET);
  }
  gear::assets::Ref *mutable_tileSet() {
    return GetPointer<gear::assets::Ref *>(VT_TILESET);
  }
  uint16_t width() const {
    return GetField<uint16_t>(VT_WIDTH, 0);
  }
  bool mutate_width(uint16_t _width) {
    return SetField<uint16_t>(VT_WIDTH, _width, 0);
  }
  uint16_t height() const {
    return GetField<uint16_t>(VT_HEIGHT, 0);
  }
  bool mutate_height(uint16_t _height) {
    return SetField<uint16_t>(VT_HEIGHT, _height, 0);
  }
  const flatbuffers::Vector<uint32_t> *data() const {
    return GetPointer<const flatbuffers::Vector<uint32_t> *>(VT_DATA);
  }
  flatbuffers::Vector<uint32_t> *mutable_data() {
    return GetPointer<flatbuffers::Vector<uint32_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_TILESET) &&
           verifier.VerifyTable(tileSet()) &&
           VerifyField<uint16_t>(verifier, VT_WIDTH) &&
           VerifyField<uint16_t>(verifier, VT_HEIGHT) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct LayerBuilder {
  typedef Layer Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Layer::VT_NAME, name);
  }
  void add_tileSet(flatbuffers::Offset<gear::assets::Ref> tileSet) {
    fbb_.AddOffset(Layer::VT_TILESET, tileSet);
  }
  void add_width(uint16_t width) {
    fbb_.AddElement<uint16_t>(Layer::VT_WIDTH, width, 0);
  }
  void add_height(uint16_t height) {
    fbb_.AddElement<uint16_t>(Layer::VT_HEIGHT, height, 0);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<uint32_t>> data) {
    fbb_.AddOffset(Layer::VT_DATA, data);
  }
  explicit LayerBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  LayerBuilder &operator=(const LayerBuilder &);
  flatbuffers::Offset<Layer> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Layer>(end);
    return o;
  }
};

inline flatbuffers::Offset<Layer> CreateLayer(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<gear::assets::Ref> tileSet = 0,
    uint16_t width = 0,
    uint16_t height = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint32_t>> data = 0) {
  LayerBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_tileSet(tileSet);
  builder_.add_name(name);
  builder_.add_height(height);
  builder_.add_width(width);
  return builder_.Finish();
}

struct Layer::Traits {
  using type = Layer;
  static auto constexpr Create = CreateLayer;
};

inline flatbuffers::Offset<Layer> CreateLayerDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    flatbuffers::Offset<gear::assets::Ref> tileSet = 0,
    uint16_t width = 0,
    uint16_t height = 0,
    const std::vector<uint32_t> *data = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto data__ = data ? _fbb.CreateVector<uint32_t>(*data) : 0;
  return gear::assets::CreateLayer(
      _fbb,
      name__,
      tileSet,
      width,
      height,
      data__);
}

struct Map FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MapBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_LAYERS = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<gear::assets::Layer>> *layers() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<gear::assets::Layer>> *>(VT_LAYERS);
  }
  flatbuffers::Vector<flatbuffers::Offset<gear::assets::Layer>> *mutable_layers() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Layer>> *>(VT_LAYERS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_LAYERS) &&
           verifier.VerifyVector(layers()) &&
           verifier.VerifyVectorOfTables(layers()) &&
           verifier.EndTable();
  }
};

struct MapBuilder {
  typedef Map Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_layers(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Layer>>> layers) {
    fbb_.AddOffset(Map::VT_LAYERS, layers);
  }
  explicit MapBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MapBuilder &operator=(const MapBuilder &);
  flatbuffers::Offset<Map> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Map>(end);
    return o;
  }
};

inline flatbuffers::Offset<Map> CreateMap(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Layer>>> layers = 0) {
  MapBuilder builder_(_fbb);
  builder_.add_layers(layers);
  return builder_.Finish();
}

struct Map::Traits {
  using type = Map;
  static auto constexpr Create = CreateMap;
};

inline flatbuffers::Offset<Map> CreateMapDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<gear::assets::Layer>> *layers = nullptr) {
  auto layers__ = layers ? _fbb.CreateVector<flatbuffers::Offset<gear::assets::Layer>>(*layers) : 0;
  return gear::assets::CreateMap(
      _fbb,
      layers__);
}

inline const gear::assets::Map *GetMap(const void *buf) {
  return flatbuffers::GetRoot<gear::assets::Map>(buf);
}

inline const gear::assets::Map *GetSizePrefixedMap(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<gear::assets::Map>(buf);
}

inline Map *GetMutableMap(void *buf) {
  return flatbuffers::GetMutableRoot<Map>(buf);
}

inline bool VerifyMapBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<gear::assets::Map>(nullptr);
}

inline bool VerifySizePrefixedMapBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<gear::assets::Map>(nullptr);
}

inline void FinishMapBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Map> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMapBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Map> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace assets
}  // namespace gear

#endif  // FLATBUFFERS_GENERATED_MAP_GEAR_ASSETS_H_