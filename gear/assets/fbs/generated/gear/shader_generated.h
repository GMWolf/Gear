// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SHADER_GEAR_ASSETS_H_
#define FLATBUFFERS_GENERATED_SHADER_GEAR_ASSETS_H_

#include "flatbuffers/flatbuffers.h"

namespace gear {
namespace assets {

struct ShaderText;
struct ShaderTextBuilder;

struct ShaderResource;
struct ShaderResourceBuilder;

struct Shader;
struct ShaderBuilder;

enum class ShaderResourceType : int8_t {
  sampler = 0,
  uniform_buffer = 1,
  MIN = sampler,
  MAX = uniform_buffer
};

inline const ShaderResourceType (&EnumValuesShaderResourceType())[2] {
  static const ShaderResourceType values[] = {
    ShaderResourceType::sampler,
    ShaderResourceType::uniform_buffer
  };
  return values;
}

inline const char * const *EnumNamesShaderResourceType() {
  static const char * const names[3] = {
    "sampler",
    "uniform_buffer",
    nullptr
  };
  return names;
}

inline const char *EnumNameShaderResourceType(ShaderResourceType e) {
  if (flatbuffers::IsOutRange(e, ShaderResourceType::sampler, ShaderResourceType::uniform_buffer)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesShaderResourceType()[index];
}

struct ShaderText FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ShaderTextBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_SOURCE = 4,
    VT_BIN = 6
  };
  const flatbuffers::String *source() const {
    return GetPointer<const flatbuffers::String *>(VT_SOURCE);
  }
  flatbuffers::String *mutable_source() {
    return GetPointer<flatbuffers::String *>(VT_SOURCE);
  }
  const flatbuffers::Vector<uint32_t> *bin() const {
    return GetPointer<const flatbuffers::Vector<uint32_t> *>(VT_BIN);
  }
  flatbuffers::Vector<uint32_t> *mutable_bin() {
    return GetPointer<flatbuffers::Vector<uint32_t> *>(VT_BIN);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_SOURCE) &&
           verifier.VerifyString(source()) &&
           VerifyOffset(verifier, VT_BIN) &&
           verifier.VerifyVector(bin()) &&
           verifier.EndTable();
  }
};

struct ShaderTextBuilder {
  typedef ShaderText Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_source(flatbuffers::Offset<flatbuffers::String> source) {
    fbb_.AddOffset(ShaderText::VT_SOURCE, source);
  }
  void add_bin(flatbuffers::Offset<flatbuffers::Vector<uint32_t>> bin) {
    fbb_.AddOffset(ShaderText::VT_BIN, bin);
  }
  explicit ShaderTextBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ShaderTextBuilder &operator=(const ShaderTextBuilder &);
  flatbuffers::Offset<ShaderText> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ShaderText>(end);
    return o;
  }
};

inline flatbuffers::Offset<ShaderText> CreateShaderText(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> source = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint32_t>> bin = 0) {
  ShaderTextBuilder builder_(_fbb);
  builder_.add_bin(bin);
  builder_.add_source(source);
  return builder_.Finish();
}

struct ShaderText::Traits {
  using type = ShaderText;
  static auto constexpr Create = CreateShaderText;
};

inline flatbuffers::Offset<ShaderText> CreateShaderTextDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *source = nullptr,
    const std::vector<uint32_t> *bin = nullptr) {
  auto source__ = source ? _fbb.CreateString(source) : 0;
  auto bin__ = bin ? _fbb.CreateVector<uint32_t>(*bin) : 0;
  return gear::assets::CreateShaderText(
      _fbb,
      source__,
      bin__);
}

struct ShaderResource FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ShaderResourceBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_BINDING = 6,
    VT_TYPE = 8
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  flatbuffers::String *mutable_name() {
    return GetPointer<flatbuffers::String *>(VT_NAME);
  }
  bool KeyCompareLessThan(const ShaderResource *o) const {
    return *name() < *o->name();
  }
  int KeyCompareWithValue(const char *val) const {
    return strcmp(name()->c_str(), val);
  }
  int32_t binding() const {
    return GetField<int32_t>(VT_BINDING, 0);
  }
  bool mutate_binding(int32_t _binding) {
    return SetField<int32_t>(VT_BINDING, _binding, 0);
  }
  gear::assets::ShaderResourceType type() const {
    return static_cast<gear::assets::ShaderResourceType>(GetField<int8_t>(VT_TYPE, 0));
  }
  bool mutate_type(gear::assets::ShaderResourceType _type) {
    return SetField<int8_t>(VT_TYPE, static_cast<int8_t>(_type), 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffsetRequired(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<int32_t>(verifier, VT_BINDING) &&
           VerifyField<int8_t>(verifier, VT_TYPE) &&
           verifier.EndTable();
  }
};

struct ShaderResourceBuilder {
  typedef ShaderResource Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(ShaderResource::VT_NAME, name);
  }
  void add_binding(int32_t binding) {
    fbb_.AddElement<int32_t>(ShaderResource::VT_BINDING, binding, 0);
  }
  void add_type(gear::assets::ShaderResourceType type) {
    fbb_.AddElement<int8_t>(ShaderResource::VT_TYPE, static_cast<int8_t>(type), 0);
  }
  explicit ShaderResourceBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ShaderResourceBuilder &operator=(const ShaderResourceBuilder &);
  flatbuffers::Offset<ShaderResource> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<ShaderResource>(end);
    fbb_.Required(o, ShaderResource::VT_NAME);
    return o;
  }
};

inline flatbuffers::Offset<ShaderResource> CreateShaderResource(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    int32_t binding = 0,
    gear::assets::ShaderResourceType type = gear::assets::ShaderResourceType::sampler) {
  ShaderResourceBuilder builder_(_fbb);
  builder_.add_binding(binding);
  builder_.add_name(name);
  builder_.add_type(type);
  return builder_.Finish();
}

struct ShaderResource::Traits {
  using type = ShaderResource;
  static auto constexpr Create = CreateShaderResource;
};

inline flatbuffers::Offset<ShaderResource> CreateShaderResourceDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    int32_t binding = 0,
    gear::assets::ShaderResourceType type = gear::assets::ShaderResourceType::sampler) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return gear::assets::CreateShaderResource(
      _fbb,
      name__,
      binding,
      type);
}

struct Shader FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ShaderBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ISBINARY = 4,
    VT_VERTEXTEXT = 6,
    VT_FRAGMENTTEXT = 8,
    VT_RESOURCES = 10
  };
  bool isBinary() const {
    return GetField<uint8_t>(VT_ISBINARY, 0) != 0;
  }
  bool mutate_isBinary(bool _isBinary) {
    return SetField<uint8_t>(VT_ISBINARY, static_cast<uint8_t>(_isBinary), 0);
  }
  const gear::assets::ShaderText *vertexText() const {
    return GetPointer<const gear::assets::ShaderText *>(VT_VERTEXTEXT);
  }
  gear::assets::ShaderText *mutable_vertexText() {
    return GetPointer<gear::assets::ShaderText *>(VT_VERTEXTEXT);
  }
  const gear::assets::ShaderText *fragmentText() const {
    return GetPointer<const gear::assets::ShaderText *>(VT_FRAGMENTTEXT);
  }
  gear::assets::ShaderText *mutable_fragmentText() {
    return GetPointer<gear::assets::ShaderText *>(VT_FRAGMENTTEXT);
  }
  const flatbuffers::Vector<flatbuffers::Offset<gear::assets::ShaderResource>> *resources() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<gear::assets::ShaderResource>> *>(VT_RESOURCES);
  }
  flatbuffers::Vector<flatbuffers::Offset<gear::assets::ShaderResource>> *mutable_resources() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<gear::assets::ShaderResource>> *>(VT_RESOURCES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_ISBINARY) &&
           VerifyOffset(verifier, VT_VERTEXTEXT) &&
           verifier.VerifyTable(vertexText()) &&
           VerifyOffset(verifier, VT_FRAGMENTTEXT) &&
           verifier.VerifyTable(fragmentText()) &&
           VerifyOffset(verifier, VT_RESOURCES) &&
           verifier.VerifyVector(resources()) &&
           verifier.VerifyVectorOfTables(resources()) &&
           verifier.EndTable();
  }
};

struct ShaderBuilder {
  typedef Shader Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_isBinary(bool isBinary) {
    fbb_.AddElement<uint8_t>(Shader::VT_ISBINARY, static_cast<uint8_t>(isBinary), 0);
  }
  void add_vertexText(flatbuffers::Offset<gear::assets::ShaderText> vertexText) {
    fbb_.AddOffset(Shader::VT_VERTEXTEXT, vertexText);
  }
  void add_fragmentText(flatbuffers::Offset<gear::assets::ShaderText> fragmentText) {
    fbb_.AddOffset(Shader::VT_FRAGMENTTEXT, fragmentText);
  }
  void add_resources(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::ShaderResource>>> resources) {
    fbb_.AddOffset(Shader::VT_RESOURCES, resources);
  }
  explicit ShaderBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ShaderBuilder &operator=(const ShaderBuilder &);
  flatbuffers::Offset<Shader> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Shader>(end);
    return o;
  }
};

inline flatbuffers::Offset<Shader> CreateShader(
    flatbuffers::FlatBufferBuilder &_fbb,
    bool isBinary = false,
    flatbuffers::Offset<gear::assets::ShaderText> vertexText = 0,
    flatbuffers::Offset<gear::assets::ShaderText> fragmentText = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::ShaderResource>>> resources = 0) {
  ShaderBuilder builder_(_fbb);
  builder_.add_resources(resources);
  builder_.add_fragmentText(fragmentText);
  builder_.add_vertexText(vertexText);
  builder_.add_isBinary(isBinary);
  return builder_.Finish();
}

struct Shader::Traits {
  using type = Shader;
  static auto constexpr Create = CreateShader;
};

inline flatbuffers::Offset<Shader> CreateShaderDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    bool isBinary = false,
    flatbuffers::Offset<gear::assets::ShaderText> vertexText = 0,
    flatbuffers::Offset<gear::assets::ShaderText> fragmentText = 0,
    std::vector<flatbuffers::Offset<gear::assets::ShaderResource>> *resources = nullptr) {
  auto resources__ = resources ? _fbb.CreateVectorOfSortedTables<gear::assets::ShaderResource>(resources) : 0;
  return gear::assets::CreateShader(
      _fbb,
      isBinary,
      vertexText,
      fragmentText,
      resources__);
}

inline const gear::assets::Shader *GetShader(const void *buf) {
  return flatbuffers::GetRoot<gear::assets::Shader>(buf);
}

inline const gear::assets::Shader *GetSizePrefixedShader(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<gear::assets::Shader>(buf);
}

inline Shader *GetMutableShader(void *buf) {
  return flatbuffers::GetMutableRoot<Shader>(buf);
}

inline bool VerifyShaderBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<gear::assets::Shader>(nullptr);
}

inline bool VerifySizePrefixedShaderBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<gear::assets::Shader>(nullptr);
}

inline void FinishShaderBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Shader> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedShaderBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Shader> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace assets
}  // namespace gear

#endif  // FLATBUFFERS_GENERATED_SHADER_GEAR_ASSETS_H_
