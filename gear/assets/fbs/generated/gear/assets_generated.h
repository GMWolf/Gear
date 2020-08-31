// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ASSETS_GEAR_ASSETS_H_
#define FLATBUFFERS_GENERATED_ASSETS_GEAR_ASSETS_H_

#include "flatbuffers/flatbuffers.h"

#include "common_generated.h"
#include "font_generated.h"
#include "map_generated.h"
#include "shader_generated.h"
#include "sprite_generated.h"
#include "texture_generated.h"
#include "tileset_generated.h"

namespace gear {
namespace assets {

struct NestedBundle;
struct NestedBundleBuilder;

struct AssetEntry;
struct AssetEntryBuilder;

struct Bundle;
struct BundleBuilder;

enum class Asset : uint8_t {
  NONE = 0,
  Texture = 1,
  Sprite = 2,
  Font = 3,
  Shader = 4,
  TileSet = 5,
  Map = 6,
  MIN = NONE,
  MAX = Map
};

inline const Asset (&EnumValuesAsset())[7] {
  static const Asset values[] = {
    Asset::NONE,
    Asset::Texture,
    Asset::Sprite,
    Asset::Font,
    Asset::Shader,
    Asset::TileSet,
    Asset::Map
  };
  return values;
}

inline const char * const *EnumNamesAsset() {
  static const char * const names[8] = {
    "NONE",
    "Texture",
    "Sprite",
    "Font",
    "Shader",
    "TileSet",
    "Map",
    nullptr
  };
  return names;
}

inline const char *EnumNameAsset(Asset e) {
  if (flatbuffers::IsOutRange(e, Asset::NONE, Asset::Map)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesAsset()[index];
}

template<typename T> struct AssetTraits {
  static const Asset enum_value = Asset::NONE;
};

template<> struct AssetTraits<gear::assets::Texture> {
  static const Asset enum_value = Asset::Texture;
};

template<> struct AssetTraits<gear::assets::Sprite> {
  static const Asset enum_value = Asset::Sprite;
};

template<> struct AssetTraits<gear::assets::Font> {
  static const Asset enum_value = Asset::Font;
};

template<> struct AssetTraits<gear::assets::Shader> {
  static const Asset enum_value = Asset::Shader;
};

template<> struct AssetTraits<gear::assets::TileSet> {
  static const Asset enum_value = Asset::TileSet;
};

template<> struct AssetTraits<gear::assets::Map> {
  static const Asset enum_value = Asset::Map;
};

bool VerifyAsset(flatbuffers::Verifier &verifier, const void *obj, Asset type);
bool VerifyAssetVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

struct NestedBundle FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef NestedBundleBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_BUNDLE = 4
  };
  const flatbuffers::Vector<uint8_t> *bundle() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_BUNDLE);
  }
  flatbuffers::Vector<uint8_t> *mutable_bundle() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_BUNDLE);
  }
  const gear::assets::Bundle *bundle_nested_root() const {
    return flatbuffers::GetRoot<gear::assets::Bundle>(bundle()->Data());
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_BUNDLE) &&
           verifier.VerifyVector(bundle()) &&
           verifier.EndTable();
  }
};

struct NestedBundleBuilder {
  typedef NestedBundle Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_bundle(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> bundle) {
    fbb_.AddOffset(NestedBundle::VT_BUNDLE, bundle);
  }
  explicit NestedBundleBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  NestedBundleBuilder &operator=(const NestedBundleBuilder &);
  flatbuffers::Offset<NestedBundle> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<NestedBundle>(end);
    return o;
  }
};

inline flatbuffers::Offset<NestedBundle> CreateNestedBundle(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> bundle = 0) {
  NestedBundleBuilder builder_(_fbb);
  builder_.add_bundle(bundle);
  return builder_.Finish();
}

struct NestedBundle::Traits {
  using type = NestedBundle;
  static auto constexpr Create = CreateNestedBundle;
};

inline flatbuffers::Offset<NestedBundle> CreateNestedBundleDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint8_t> *bundle = nullptr) {
  auto bundle__ = bundle ? _fbb.CreateVector<uint8_t>(*bundle) : 0;
  return gear::assets::CreateNestedBundle(
      _fbb,
      bundle__);
}

struct AssetEntry FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AssetEntryBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_ASSET_TYPE = 6,
    VT_ASSET = 8
  };
  uint64_t name() const {
    return GetField<uint64_t>(VT_NAME, 0);
  }
  bool mutate_name(uint64_t _name) {
    return SetField<uint64_t>(VT_NAME, _name, 0);
  }
  bool KeyCompareLessThan(const AssetEntry *o) const {
    return name() < o->name();
  }
  int KeyCompareWithValue(uint64_t val) const {
    return static_cast<int>(name() > val) - static_cast<int>(name() < val);
  }
  gear::assets::Asset asset_type() const {
    return static_cast<gear::assets::Asset>(GetField<uint8_t>(VT_ASSET_TYPE, 0));
  }
  const void *asset() const {
    return GetPointer<const void *>(VT_ASSET);
  }
  template<typename T> const T *asset_as() const;
  const gear::assets::Texture *asset_as_Texture() const {
    return asset_type() == gear::assets::Asset::Texture ? static_cast<const gear::assets::Texture *>(asset()) : nullptr;
  }
  const gear::assets::Sprite *asset_as_Sprite() const {
    return asset_type() == gear::assets::Asset::Sprite ? static_cast<const gear::assets::Sprite *>(asset()) : nullptr;
  }
  const gear::assets::Font *asset_as_Font() const {
    return asset_type() == gear::assets::Asset::Font ? static_cast<const gear::assets::Font *>(asset()) : nullptr;
  }
  const gear::assets::Shader *asset_as_Shader() const {
    return asset_type() == gear::assets::Asset::Shader ? static_cast<const gear::assets::Shader *>(asset()) : nullptr;
  }
  const gear::assets::TileSet *asset_as_TileSet() const {
    return asset_type() == gear::assets::Asset::TileSet ? static_cast<const gear::assets::TileSet *>(asset()) : nullptr;
  }
  const gear::assets::Map *asset_as_Map() const {
    return asset_type() == gear::assets::Asset::Map ? static_cast<const gear::assets::Map *>(asset()) : nullptr;
  }
  void *mutable_asset() {
    return GetPointer<void *>(VT_ASSET);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_NAME) &&
           VerifyField<uint8_t>(verifier, VT_ASSET_TYPE) &&
           VerifyOffset(verifier, VT_ASSET) &&
           VerifyAsset(verifier, asset(), asset_type()) &&
           verifier.EndTable();
  }
};

template<> inline const gear::assets::Texture *AssetEntry::asset_as<gear::assets::Texture>() const {
  return asset_as_Texture();
}

template<> inline const gear::assets::Sprite *AssetEntry::asset_as<gear::assets::Sprite>() const {
  return asset_as_Sprite();
}

template<> inline const gear::assets::Font *AssetEntry::asset_as<gear::assets::Font>() const {
  return asset_as_Font();
}

template<> inline const gear::assets::Shader *AssetEntry::asset_as<gear::assets::Shader>() const {
  return asset_as_Shader();
}

template<> inline const gear::assets::TileSet *AssetEntry::asset_as<gear::assets::TileSet>() const {
  return asset_as_TileSet();
}

template<> inline const gear::assets::Map *AssetEntry::asset_as<gear::assets::Map>() const {
  return asset_as_Map();
}

struct AssetEntryBuilder {
  typedef AssetEntry Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(uint64_t name) {
    fbb_.AddElement<uint64_t>(AssetEntry::VT_NAME, name, 0);
  }
  void add_asset_type(gear::assets::Asset asset_type) {
    fbb_.AddElement<uint8_t>(AssetEntry::VT_ASSET_TYPE, static_cast<uint8_t>(asset_type), 0);
  }
  void add_asset(flatbuffers::Offset<void> asset) {
    fbb_.AddOffset(AssetEntry::VT_ASSET, asset);
  }
  explicit AssetEntryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  AssetEntryBuilder &operator=(const AssetEntryBuilder &);
  flatbuffers::Offset<AssetEntry> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AssetEntry>(end);
    return o;
  }
};

inline flatbuffers::Offset<AssetEntry> CreateAssetEntry(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t name = 0,
    gear::assets::Asset asset_type = gear::assets::Asset::NONE,
    flatbuffers::Offset<void> asset = 0) {
  AssetEntryBuilder builder_(_fbb);
  builder_.add_name(name);
  builder_.add_asset(asset);
  builder_.add_asset_type(asset_type);
  return builder_.Finish();
}

struct AssetEntry::Traits {
  using type = AssetEntry;
  static auto constexpr Create = CreateAssetEntry;
};

struct Bundle FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef BundleBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ASSETS = 4,
    VT_NESTEDBUNDLES = 6,
    VT_EXTERNALREFS = 8
  };
  const flatbuffers::Vector<flatbuffers::Offset<gear::assets::AssetEntry>> *assets() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<gear::assets::AssetEntry>> *>(VT_ASSETS);
  }
  flatbuffers::Vector<flatbuffers::Offset<gear::assets::AssetEntry>> *mutable_assets() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<gear::assets::AssetEntry>> *>(VT_ASSETS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<gear::assets::NestedBundle>> *nestedBundles() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<gear::assets::NestedBundle>> *>(VT_NESTEDBUNDLES);
  }
  flatbuffers::Vector<flatbuffers::Offset<gear::assets::NestedBundle>> *mutable_nestedBundles() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<gear::assets::NestedBundle>> *>(VT_NESTEDBUNDLES);
  }
  const flatbuffers::Vector<flatbuffers::Offset<gear::assets::Ref>> *externalRefs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<gear::assets::Ref>> *>(VT_EXTERNALREFS);
  }
  flatbuffers::Vector<flatbuffers::Offset<gear::assets::Ref>> *mutable_externalRefs() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Ref>> *>(VT_EXTERNALREFS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ASSETS) &&
           verifier.VerifyVector(assets()) &&
           verifier.VerifyVectorOfTables(assets()) &&
           VerifyOffset(verifier, VT_NESTEDBUNDLES) &&
           verifier.VerifyVector(nestedBundles()) &&
           verifier.VerifyVectorOfTables(nestedBundles()) &&
           VerifyOffset(verifier, VT_EXTERNALREFS) &&
           verifier.VerifyVector(externalRefs()) &&
           verifier.VerifyVectorOfTables(externalRefs()) &&
           verifier.EndTable();
  }
};

struct BundleBuilder {
  typedef Bundle Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_assets(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::AssetEntry>>> assets) {
    fbb_.AddOffset(Bundle::VT_ASSETS, assets);
  }
  void add_nestedBundles(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::NestedBundle>>> nestedBundles) {
    fbb_.AddOffset(Bundle::VT_NESTEDBUNDLES, nestedBundles);
  }
  void add_externalRefs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Ref>>> externalRefs) {
    fbb_.AddOffset(Bundle::VT_EXTERNALREFS, externalRefs);
  }
  explicit BundleBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  BundleBuilder &operator=(const BundleBuilder &);
  flatbuffers::Offset<Bundle> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Bundle>(end);
    return o;
  }
};

inline flatbuffers::Offset<Bundle> CreateBundle(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::AssetEntry>>> assets = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::NestedBundle>>> nestedBundles = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::Ref>>> externalRefs = 0) {
  BundleBuilder builder_(_fbb);
  builder_.add_externalRefs(externalRefs);
  builder_.add_nestedBundles(nestedBundles);
  builder_.add_assets(assets);
  return builder_.Finish();
}

struct Bundle::Traits {
  using type = Bundle;
  static auto constexpr Create = CreateBundle;
};

inline flatbuffers::Offset<Bundle> CreateBundleDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    std::vector<flatbuffers::Offset<gear::assets::AssetEntry>> *assets = nullptr,
    const std::vector<flatbuffers::Offset<gear::assets::NestedBundle>> *nestedBundles = nullptr,
    const std::vector<flatbuffers::Offset<gear::assets::Ref>> *externalRefs = nullptr) {
  auto assets__ = assets ? _fbb.CreateVectorOfSortedTables<gear::assets::AssetEntry>(assets) : 0;
  auto nestedBundles__ = nestedBundles ? _fbb.CreateVector<flatbuffers::Offset<gear::assets::NestedBundle>>(*nestedBundles) : 0;
  auto externalRefs__ = externalRefs ? _fbb.CreateVector<flatbuffers::Offset<gear::assets::Ref>>(*externalRefs) : 0;
  return gear::assets::CreateBundle(
      _fbb,
      assets__,
      nestedBundles__,
      externalRefs__);
}

inline bool VerifyAsset(flatbuffers::Verifier &verifier, const void *obj, Asset type) {
  switch (type) {
    case Asset::NONE: {
      return true;
    }
    case Asset::Texture: {
      auto ptr = reinterpret_cast<const gear::assets::Texture *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Asset::Sprite: {
      auto ptr = reinterpret_cast<const gear::assets::Sprite *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Asset::Font: {
      auto ptr = reinterpret_cast<const gear::assets::Font *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Asset::Shader: {
      auto ptr = reinterpret_cast<const gear::assets::Shader *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Asset::TileSet: {
      auto ptr = reinterpret_cast<const gear::assets::TileSet *>(obj);
      return verifier.VerifyTable(ptr);
    }
    case Asset::Map: {
      auto ptr = reinterpret_cast<const gear::assets::Map *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return true;
  }
}

inline bool VerifyAssetVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyAsset(
        verifier,  values->Get(i), types->GetEnum<Asset>(i))) {
      return false;
    }
  }
  return true;
}

inline const gear::assets::Bundle *GetBundle(const void *buf) {
  return flatbuffers::GetRoot<gear::assets::Bundle>(buf);
}

inline const gear::assets::Bundle *GetSizePrefixedBundle(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<gear::assets::Bundle>(buf);
}

inline Bundle *GetMutableBundle(void *buf) {
  return flatbuffers::GetMutableRoot<Bundle>(buf);
}

inline bool VerifyBundleBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<gear::assets::Bundle>(nullptr);
}

inline bool VerifySizePrefixedBundleBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<gear::assets::Bundle>(nullptr);
}

inline void FinishBundleBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Bundle> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedBundleBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Bundle> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace assets
}  // namespace gear

#endif  // FLATBUFFERS_GENERATED_ASSETS_GEAR_ASSETS_H_
