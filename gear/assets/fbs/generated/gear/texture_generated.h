// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TEXTURE_GEAR_ASSETS_H_
#define FLATBUFFERS_GENERATED_TEXTURE_GEAR_ASSETS_H_

#include "flatbuffers/flatbuffers.h"

namespace gear {
namespace assets {

struct MipData;
struct MipDataBuilder;

struct Texture;
struct TextureBuilder;

enum class PixelFormat : uint8_t {
  Invalid = 0,
  R8 = 1,
  RGB8 = 2,
  RGBA8 = 3,
  BC4 = 4,
  BC5 = 5,
  BC7 = 6,
  MIN = Invalid,
  MAX = BC7
};

inline const PixelFormat (&EnumValuesPixelFormat())[7] {
  static const PixelFormat values[] = {
    PixelFormat::Invalid,
    PixelFormat::R8,
    PixelFormat::RGB8,
    PixelFormat::RGBA8,
    PixelFormat::BC4,
    PixelFormat::BC5,
    PixelFormat::BC7
  };
  return values;
}

inline const char * const *EnumNamesPixelFormat() {
  static const char * const names[8] = {
    "Invalid",
    "R8",
    "RGB8",
    "RGBA8",
    "BC4",
    "BC5",
    "BC7",
    nullptr
  };
  return names;
}

inline const char *EnumNamePixelFormat(PixelFormat e) {
  if (flatbuffers::IsOutRange(e, PixelFormat::Invalid, PixelFormat::BC7)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesPixelFormat()[index];
}

struct MipData FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MipDataBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_DATA = 4
  };
  const flatbuffers::Vector<uint8_t> *data() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  flatbuffers::Vector<uint8_t> *mutable_data() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct MipDataBuilder {
  typedef MipData Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_data(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data) {
    fbb_.AddOffset(MipData::VT_DATA, data);
  }
  explicit MipDataBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MipDataBuilder &operator=(const MipDataBuilder &);
  flatbuffers::Offset<MipData> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MipData>(end);
    return o;
  }
};

inline flatbuffers::Offset<MipData> CreateMipData(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = 0) {
  MipDataBuilder builder_(_fbb);
  builder_.add_data(data);
  return builder_.Finish();
}

struct MipData::Traits {
  using type = MipData;
  static auto constexpr Create = CreateMipData;
};

inline flatbuffers::Offset<MipData> CreateMipDataDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint8_t> *data = nullptr) {
  auto data__ = data ? _fbb.CreateVector<uint8_t>(*data) : 0;
  return gear::assets::CreateMipData(
      _fbb,
      data__);
}

struct Texture FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef TextureBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_WIDTH = 4,
    VT_HEIGHT = 6,
    VT_FORMAT = 8,
    VT_MIPS = 10
  };
  int16_t width() const {
    return GetField<int16_t>(VT_WIDTH, 0);
  }
  bool mutate_width(int16_t _width) {
    return SetField<int16_t>(VT_WIDTH, _width, 0);
  }
  int16_t height() const {
    return GetField<int16_t>(VT_HEIGHT, 0);
  }
  bool mutate_height(int16_t _height) {
    return SetField<int16_t>(VT_HEIGHT, _height, 0);
  }
  gear::assets::PixelFormat format() const {
    return static_cast<gear::assets::PixelFormat>(GetField<uint8_t>(VT_FORMAT, 0));
  }
  bool mutate_format(gear::assets::PixelFormat _format) {
    return SetField<uint8_t>(VT_FORMAT, static_cast<uint8_t>(_format), 0);
  }
  const flatbuffers::Vector<flatbuffers::Offset<gear::assets::MipData>> *mips() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<gear::assets::MipData>> *>(VT_MIPS);
  }
  flatbuffers::Vector<flatbuffers::Offset<gear::assets::MipData>> *mutable_mips() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<gear::assets::MipData>> *>(VT_MIPS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int16_t>(verifier, VT_WIDTH) &&
           VerifyField<int16_t>(verifier, VT_HEIGHT) &&
           VerifyField<uint8_t>(verifier, VT_FORMAT) &&
           VerifyOffset(verifier, VT_MIPS) &&
           verifier.VerifyVector(mips()) &&
           verifier.VerifyVectorOfTables(mips()) &&
           verifier.EndTable();
  }
};

struct TextureBuilder {
  typedef Texture Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_width(int16_t width) {
    fbb_.AddElement<int16_t>(Texture::VT_WIDTH, width, 0);
  }
  void add_height(int16_t height) {
    fbb_.AddElement<int16_t>(Texture::VT_HEIGHT, height, 0);
  }
  void add_format(gear::assets::PixelFormat format) {
    fbb_.AddElement<uint8_t>(Texture::VT_FORMAT, static_cast<uint8_t>(format), 0);
  }
  void add_mips(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::MipData>>> mips) {
    fbb_.AddOffset(Texture::VT_MIPS, mips);
  }
  explicit TextureBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  TextureBuilder &operator=(const TextureBuilder &);
  flatbuffers::Offset<Texture> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Texture>(end);
    return o;
  }
};

inline flatbuffers::Offset<Texture> CreateTexture(
    flatbuffers::FlatBufferBuilder &_fbb,
    int16_t width = 0,
    int16_t height = 0,
    gear::assets::PixelFormat format = gear::assets::PixelFormat::Invalid,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::MipData>>> mips = 0) {
  TextureBuilder builder_(_fbb);
  builder_.add_mips(mips);
  builder_.add_height(height);
  builder_.add_width(width);
  builder_.add_format(format);
  return builder_.Finish();
}

struct Texture::Traits {
  using type = Texture;
  static auto constexpr Create = CreateTexture;
};

inline flatbuffers::Offset<Texture> CreateTextureDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int16_t width = 0,
    int16_t height = 0,
    gear::assets::PixelFormat format = gear::assets::PixelFormat::Invalid,
    const std::vector<flatbuffers::Offset<gear::assets::MipData>> *mips = nullptr) {
  auto mips__ = mips ? _fbb.CreateVector<flatbuffers::Offset<gear::assets::MipData>>(*mips) : 0;
  return gear::assets::CreateTexture(
      _fbb,
      width,
      height,
      format,
      mips__);
}

inline const gear::assets::Texture *GetTexture(const void *buf) {
  return flatbuffers::GetRoot<gear::assets::Texture>(buf);
}

inline const gear::assets::Texture *GetSizePrefixedTexture(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<gear::assets::Texture>(buf);
}

inline Texture *GetMutableTexture(void *buf) {
  return flatbuffers::GetMutableRoot<Texture>(buf);
}

inline bool VerifyTextureBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<gear::assets::Texture>(nullptr);
}

inline bool VerifySizePrefixedTextureBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<gear::assets::Texture>(nullptr);
}

inline void FinishTextureBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Texture> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTextureBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Texture> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace assets
}  // namespace gear

#endif  // FLATBUFFERS_GENERATED_TEXTURE_GEAR_ASSETS_H_
