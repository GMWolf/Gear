// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TILESET_GEAR_ASSETS_H_
#define FLATBUFFERS_GENERATED_TILESET_GEAR_ASSETS_H_

#include "flatbuffers/flatbuffers.h"

#include "common_generated.h"

namespace gear {
namespace assets {

struct TileSet;
struct TileSetBuilder;

struct TileSet FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef TileSetBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEXTURE = 4,
    VT_TEXTUREWIDTH = 6,
    VT_TEXTUREHEIGHT = 8,
    VT_TILEWIDTH = 10,
    VT_TILEHEIGHT = 12,
    VT_TILECOUNT = 14,
    VT_COLUMNS = 16
  };
  const gear::assets::Ref *texture() const {
    return GetPointer<const gear::assets::Ref *>(VT_TEXTURE);
  }
  gear::assets::Ref *mutable_texture() {
    return GetPointer<gear::assets::Ref *>(VT_TEXTURE);
  }
  int16_t textureWidth() const {
    return GetField<int16_t>(VT_TEXTUREWIDTH, 0);
  }
  bool mutate_textureWidth(int16_t _textureWidth) {
    return SetField<int16_t>(VT_TEXTUREWIDTH, _textureWidth, 0);
  }
  int16_t textureHeight() const {
    return GetField<int16_t>(VT_TEXTUREHEIGHT, 0);
  }
  bool mutate_textureHeight(int16_t _textureHeight) {
    return SetField<int16_t>(VT_TEXTUREHEIGHT, _textureHeight, 0);
  }
  int16_t tileWidth() const {
    return GetField<int16_t>(VT_TILEWIDTH, 0);
  }
  bool mutate_tileWidth(int16_t _tileWidth) {
    return SetField<int16_t>(VT_TILEWIDTH, _tileWidth, 0);
  }
  int16_t tileHeight() const {
    return GetField<int16_t>(VT_TILEHEIGHT, 0);
  }
  bool mutate_tileHeight(int16_t _tileHeight) {
    return SetField<int16_t>(VT_TILEHEIGHT, _tileHeight, 0);
  }
  int16_t tileCount() const {
    return GetField<int16_t>(VT_TILECOUNT, 0);
  }
  bool mutate_tileCount(int16_t _tileCount) {
    return SetField<int16_t>(VT_TILECOUNT, _tileCount, 0);
  }
  int16_t columns() const {
    return GetField<int16_t>(VT_COLUMNS, 0);
  }
  bool mutate_columns(int16_t _columns) {
    return SetField<int16_t>(VT_COLUMNS, _columns, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TEXTURE) &&
           verifier.VerifyTable(texture()) &&
           VerifyField<int16_t>(verifier, VT_TEXTUREWIDTH) &&
           VerifyField<int16_t>(verifier, VT_TEXTUREHEIGHT) &&
           VerifyField<int16_t>(verifier, VT_TILEWIDTH) &&
           VerifyField<int16_t>(verifier, VT_TILEHEIGHT) &&
           VerifyField<int16_t>(verifier, VT_TILECOUNT) &&
           VerifyField<int16_t>(verifier, VT_COLUMNS) &&
           verifier.EndTable();
  }
};

struct TileSetBuilder {
  typedef TileSet Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_texture(flatbuffers::Offset<gear::assets::Ref> texture) {
    fbb_.AddOffset(TileSet::VT_TEXTURE, texture);
  }
  void add_textureWidth(int16_t textureWidth) {
    fbb_.AddElement<int16_t>(TileSet::VT_TEXTUREWIDTH, textureWidth, 0);
  }
  void add_textureHeight(int16_t textureHeight) {
    fbb_.AddElement<int16_t>(TileSet::VT_TEXTUREHEIGHT, textureHeight, 0);
  }
  void add_tileWidth(int16_t tileWidth) {
    fbb_.AddElement<int16_t>(TileSet::VT_TILEWIDTH, tileWidth, 0);
  }
  void add_tileHeight(int16_t tileHeight) {
    fbb_.AddElement<int16_t>(TileSet::VT_TILEHEIGHT, tileHeight, 0);
  }
  void add_tileCount(int16_t tileCount) {
    fbb_.AddElement<int16_t>(TileSet::VT_TILECOUNT, tileCount, 0);
  }
  void add_columns(int16_t columns) {
    fbb_.AddElement<int16_t>(TileSet::VT_COLUMNS, columns, 0);
  }
  explicit TileSetBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  TileSetBuilder &operator=(const TileSetBuilder &);
  flatbuffers::Offset<TileSet> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<TileSet>(end);
    return o;
  }
};

inline flatbuffers::Offset<TileSet> CreateTileSet(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<gear::assets::Ref> texture = 0,
    int16_t textureWidth = 0,
    int16_t textureHeight = 0,
    int16_t tileWidth = 0,
    int16_t tileHeight = 0,
    int16_t tileCount = 0,
    int16_t columns = 0) {
  TileSetBuilder builder_(_fbb);
  builder_.add_texture(texture);
  builder_.add_columns(columns);
  builder_.add_tileCount(tileCount);
  builder_.add_tileHeight(tileHeight);
  builder_.add_tileWidth(tileWidth);
  builder_.add_textureHeight(textureHeight);
  builder_.add_textureWidth(textureWidth);
  return builder_.Finish();
}

struct TileSet::Traits {
  using type = TileSet;
  static auto constexpr Create = CreateTileSet;
};

inline const gear::assets::TileSet *GetTileSet(const void *buf) {
  return flatbuffers::GetRoot<gear::assets::TileSet>(buf);
}

inline const gear::assets::TileSet *GetSizePrefixedTileSet(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<gear::assets::TileSet>(buf);
}

inline TileSet *GetMutableTileSet(void *buf) {
  return flatbuffers::GetMutableRoot<TileSet>(buf);
}

inline bool VerifyTileSetBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<gear::assets::TileSet>(nullptr);
}

inline bool VerifySizePrefixedTileSetBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<gear::assets::TileSet>(nullptr);
}

inline void FinishTileSetBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::TileSet> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTileSetBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::TileSet> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace assets
}  // namespace gear

#endif  // FLATBUFFERS_GENERATED_TILESET_GEAR_ASSETS_H_