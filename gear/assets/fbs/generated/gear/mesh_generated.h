// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_MESH_GEAR_ASSETS_H_
#define FLATBUFFERS_GENERATED_MESH_GEAR_ASSETS_H_

#include "flatbuffers/flatbuffers.h"

#include "common_generated.h"

namespace gear {
namespace assets {

struct Material;
struct MaterialBuilder;

struct MeshletBuffer;
struct MeshletBufferBuilder;

struct MeshPrimitive;
struct MeshPrimitiveBuilder;

struct Mesh;
struct MeshBuilder;

struct Material FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MaterialBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_BASECOLOR = 4,
    VT_NORMAL = 6,
    VT_METALLICROUGHNESS = 8,
    VT_AMBIENTOCCLUSION = 10
  };
  const gear::assets::Ref *baseColor() const {
    return GetPointer<const gear::assets::Ref *>(VT_BASECOLOR);
  }
  gear::assets::Ref *mutable_baseColor() {
    return GetPointer<gear::assets::Ref *>(VT_BASECOLOR);
  }
  const gear::assets::Ref *normal() const {
    return GetPointer<const gear::assets::Ref *>(VT_NORMAL);
  }
  gear::assets::Ref *mutable_normal() {
    return GetPointer<gear::assets::Ref *>(VT_NORMAL);
  }
  const gear::assets::Ref *MetallicRoughness() const {
    return GetPointer<const gear::assets::Ref *>(VT_METALLICROUGHNESS);
  }
  gear::assets::Ref *mutable_MetallicRoughness() {
    return GetPointer<gear::assets::Ref *>(VT_METALLICROUGHNESS);
  }
  const gear::assets::Ref *AmbientOcclusion() const {
    return GetPointer<const gear::assets::Ref *>(VT_AMBIENTOCCLUSION);
  }
  gear::assets::Ref *mutable_AmbientOcclusion() {
    return GetPointer<gear::assets::Ref *>(VT_AMBIENTOCCLUSION);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_BASECOLOR) &&
           verifier.VerifyTable(baseColor()) &&
           VerifyOffset(verifier, VT_NORMAL) &&
           verifier.VerifyTable(normal()) &&
           VerifyOffset(verifier, VT_METALLICROUGHNESS) &&
           verifier.VerifyTable(MetallicRoughness()) &&
           VerifyOffset(verifier, VT_AMBIENTOCCLUSION) &&
           verifier.VerifyTable(AmbientOcclusion()) &&
           verifier.EndTable();
  }
};

struct MaterialBuilder {
  typedef Material Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_baseColor(flatbuffers::Offset<gear::assets::Ref> baseColor) {
    fbb_.AddOffset(Material::VT_BASECOLOR, baseColor);
  }
  void add_normal(flatbuffers::Offset<gear::assets::Ref> normal) {
    fbb_.AddOffset(Material::VT_NORMAL, normal);
  }
  void add_MetallicRoughness(flatbuffers::Offset<gear::assets::Ref> MetallicRoughness) {
    fbb_.AddOffset(Material::VT_METALLICROUGHNESS, MetallicRoughness);
  }
  void add_AmbientOcclusion(flatbuffers::Offset<gear::assets::Ref> AmbientOcclusion) {
    fbb_.AddOffset(Material::VT_AMBIENTOCCLUSION, AmbientOcclusion);
  }
  explicit MaterialBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MaterialBuilder &operator=(const MaterialBuilder &);
  flatbuffers::Offset<Material> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Material>(end);
    return o;
  }
};

inline flatbuffers::Offset<Material> CreateMaterial(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<gear::assets::Ref> baseColor = 0,
    flatbuffers::Offset<gear::assets::Ref> normal = 0,
    flatbuffers::Offset<gear::assets::Ref> MetallicRoughness = 0,
    flatbuffers::Offset<gear::assets::Ref> AmbientOcclusion = 0) {
  MaterialBuilder builder_(_fbb);
  builder_.add_AmbientOcclusion(AmbientOcclusion);
  builder_.add_MetallicRoughness(MetallicRoughness);
  builder_.add_normal(normal);
  builder_.add_baseColor(baseColor);
  return builder_.Finish();
}

struct Material::Traits {
  using type = Material;
  static auto constexpr Create = CreateMaterial;
};

struct MeshletBuffer FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MeshletBufferBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_INDICES = 4,
    VT_POSITIONS = 6,
    VT_TEXCOORDS = 8,
    VT_NORMALS = 10,
    VT_TANGENTS = 12,
    VT_VERTEXCOUNT = 14,
    VT_VERTEXOFFSETS = 16,
    VT_INDEXOFFSETS = 18,
    VT_INDEXCOUNTS = 20
  };
  const flatbuffers::Vector<uint8_t> *indices() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_INDICES);
  }
  flatbuffers::Vector<uint8_t> *mutable_indices() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_INDICES);
  }
  const flatbuffers::Vector<uint8_t> *positions() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_POSITIONS);
  }
  flatbuffers::Vector<uint8_t> *mutable_positions() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_POSITIONS);
  }
  const flatbuffers::Vector<uint8_t> *texcoords() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_TEXCOORDS);
  }
  flatbuffers::Vector<uint8_t> *mutable_texcoords() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_TEXCOORDS);
  }
  const flatbuffers::Vector<uint8_t> *normals() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_NORMALS);
  }
  flatbuffers::Vector<uint8_t> *mutable_normals() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_NORMALS);
  }
  const flatbuffers::Vector<uint8_t> *tangents() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_TANGENTS);
  }
  flatbuffers::Vector<uint8_t> *mutable_tangents() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_TANGENTS);
  }
  uint16_t vertexCount() const {
    return GetField<uint16_t>(VT_VERTEXCOUNT, 0);
  }
  bool mutate_vertexCount(uint16_t _vertexCount) {
    return SetField<uint16_t>(VT_VERTEXCOUNT, _vertexCount, 0);
  }
  const flatbuffers::Vector<int32_t> *vertexOffsets() const {
    return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_VERTEXOFFSETS);
  }
  flatbuffers::Vector<int32_t> *mutable_vertexOffsets() {
    return GetPointer<flatbuffers::Vector<int32_t> *>(VT_VERTEXOFFSETS);
  }
  const flatbuffers::Vector<uint32_t> *indexOffsets() const {
    return GetPointer<const flatbuffers::Vector<uint32_t> *>(VT_INDEXOFFSETS);
  }
  flatbuffers::Vector<uint32_t> *mutable_indexOffsets() {
    return GetPointer<flatbuffers::Vector<uint32_t> *>(VT_INDEXOFFSETS);
  }
  const flatbuffers::Vector<int32_t> *indexCounts() const {
    return GetPointer<const flatbuffers::Vector<int32_t> *>(VT_INDEXCOUNTS);
  }
  flatbuffers::Vector<int32_t> *mutable_indexCounts() {
    return GetPointer<flatbuffers::Vector<int32_t> *>(VT_INDEXCOUNTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_INDICES) &&
           verifier.VerifyVector(indices()) &&
           VerifyOffset(verifier, VT_POSITIONS) &&
           verifier.VerifyVector(positions()) &&
           VerifyOffset(verifier, VT_TEXCOORDS) &&
           verifier.VerifyVector(texcoords()) &&
           VerifyOffset(verifier, VT_NORMALS) &&
           verifier.VerifyVector(normals()) &&
           VerifyOffset(verifier, VT_TANGENTS) &&
           verifier.VerifyVector(tangents()) &&
           VerifyField<uint16_t>(verifier, VT_VERTEXCOUNT) &&
           VerifyOffset(verifier, VT_VERTEXOFFSETS) &&
           verifier.VerifyVector(vertexOffsets()) &&
           VerifyOffset(verifier, VT_INDEXOFFSETS) &&
           verifier.VerifyVector(indexOffsets()) &&
           VerifyOffset(verifier, VT_INDEXCOUNTS) &&
           verifier.VerifyVector(indexCounts()) &&
           verifier.EndTable();
  }
};

struct MeshletBufferBuilder {
  typedef MeshletBuffer Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_indices(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> indices) {
    fbb_.AddOffset(MeshletBuffer::VT_INDICES, indices);
  }
  void add_positions(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> positions) {
    fbb_.AddOffset(MeshletBuffer::VT_POSITIONS, positions);
  }
  void add_texcoords(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> texcoords) {
    fbb_.AddOffset(MeshletBuffer::VT_TEXCOORDS, texcoords);
  }
  void add_normals(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> normals) {
    fbb_.AddOffset(MeshletBuffer::VT_NORMALS, normals);
  }
  void add_tangents(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> tangents) {
    fbb_.AddOffset(MeshletBuffer::VT_TANGENTS, tangents);
  }
  void add_vertexCount(uint16_t vertexCount) {
    fbb_.AddElement<uint16_t>(MeshletBuffer::VT_VERTEXCOUNT, vertexCount, 0);
  }
  void add_vertexOffsets(flatbuffers::Offset<flatbuffers::Vector<int32_t>> vertexOffsets) {
    fbb_.AddOffset(MeshletBuffer::VT_VERTEXOFFSETS, vertexOffsets);
  }
  void add_indexOffsets(flatbuffers::Offset<flatbuffers::Vector<uint32_t>> indexOffsets) {
    fbb_.AddOffset(MeshletBuffer::VT_INDEXOFFSETS, indexOffsets);
  }
  void add_indexCounts(flatbuffers::Offset<flatbuffers::Vector<int32_t>> indexCounts) {
    fbb_.AddOffset(MeshletBuffer::VT_INDEXCOUNTS, indexCounts);
  }
  explicit MeshletBufferBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MeshletBufferBuilder &operator=(const MeshletBufferBuilder &);
  flatbuffers::Offset<MeshletBuffer> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MeshletBuffer>(end);
    return o;
  }
};

inline flatbuffers::Offset<MeshletBuffer> CreateMeshletBuffer(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> indices = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> positions = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> texcoords = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> normals = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> tangents = 0,
    uint16_t vertexCount = 0,
    flatbuffers::Offset<flatbuffers::Vector<int32_t>> vertexOffsets = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint32_t>> indexOffsets = 0,
    flatbuffers::Offset<flatbuffers::Vector<int32_t>> indexCounts = 0) {
  MeshletBufferBuilder builder_(_fbb);
  builder_.add_indexCounts(indexCounts);
  builder_.add_indexOffsets(indexOffsets);
  builder_.add_vertexOffsets(vertexOffsets);
  builder_.add_tangents(tangents);
  builder_.add_normals(normals);
  builder_.add_texcoords(texcoords);
  builder_.add_positions(positions);
  builder_.add_indices(indices);
  builder_.add_vertexCount(vertexCount);
  return builder_.Finish();
}

struct MeshletBuffer::Traits {
  using type = MeshletBuffer;
  static auto constexpr Create = CreateMeshletBuffer;
};

inline flatbuffers::Offset<MeshletBuffer> CreateMeshletBufferDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<uint8_t> *indices = nullptr,
    const std::vector<uint8_t> *positions = nullptr,
    const std::vector<uint8_t> *texcoords = nullptr,
    const std::vector<uint8_t> *normals = nullptr,
    const std::vector<uint8_t> *tangents = nullptr,
    uint16_t vertexCount = 0,
    const std::vector<int32_t> *vertexOffsets = nullptr,
    const std::vector<uint32_t> *indexOffsets = nullptr,
    const std::vector<int32_t> *indexCounts = nullptr) {
  auto indices__ = indices ? _fbb.CreateVector<uint8_t>(*indices) : 0;
  auto positions__ = positions ? _fbb.CreateVector<uint8_t>(*positions) : 0;
  auto texcoords__ = texcoords ? _fbb.CreateVector<uint8_t>(*texcoords) : 0;
  auto normals__ = normals ? _fbb.CreateVector<uint8_t>(*normals) : 0;
  auto tangents__ = tangents ? _fbb.CreateVector<uint8_t>(*tangents) : 0;
  auto vertexOffsets__ = vertexOffsets ? _fbb.CreateVector<int32_t>(*vertexOffsets) : 0;
  auto indexOffsets__ = indexOffsets ? _fbb.CreateVector<uint32_t>(*indexOffsets) : 0;
  auto indexCounts__ = indexCounts ? _fbb.CreateVector<int32_t>(*indexCounts) : 0;
  return gear::assets::CreateMeshletBuffer(
      _fbb,
      indices__,
      positions__,
      texcoords__,
      normals__,
      tangents__,
      vertexCount,
      vertexOffsets__,
      indexOffsets__,
      indexCounts__);
}

struct MeshPrimitive FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MeshPrimitiveBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_INDEXCOUNT = 4,
    VT_VERTEXCOUNT = 6,
    VT_INDICES = 8,
    VT_POSITION = 10,
    VT_TEXCOORD = 12,
    VT_NORMALS = 14,
    VT_TANGENTS = 16,
    VT_MATERIAL = 18,
    VT_MESHLETS = 20
  };
  uint32_t indexCount() const {
    return GetField<uint32_t>(VT_INDEXCOUNT, 0);
  }
  bool mutate_indexCount(uint32_t _indexCount) {
    return SetField<uint32_t>(VT_INDEXCOUNT, _indexCount, 0);
  }
  uint32_t vertexCount() const {
    return GetField<uint32_t>(VT_VERTEXCOUNT, 0);
  }
  bool mutate_vertexCount(uint32_t _vertexCount) {
    return SetField<uint32_t>(VT_VERTEXCOUNT, _vertexCount, 0);
  }
  const flatbuffers::Vector<uint8_t> *indices() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_INDICES);
  }
  flatbuffers::Vector<uint8_t> *mutable_indices() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_INDICES);
  }
  const flatbuffers::Vector<uint8_t> *position() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_POSITION);
  }
  flatbuffers::Vector<uint8_t> *mutable_position() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_POSITION);
  }
  const flatbuffers::Vector<uint8_t> *texcoord() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_TEXCOORD);
  }
  flatbuffers::Vector<uint8_t> *mutable_texcoord() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_TEXCOORD);
  }
  const flatbuffers::Vector<uint8_t> *normals() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_NORMALS);
  }
  flatbuffers::Vector<uint8_t> *mutable_normals() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_NORMALS);
  }
  const flatbuffers::Vector<uint8_t> *tangents() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_TANGENTS);
  }
  flatbuffers::Vector<uint8_t> *mutable_tangents() {
    return GetPointer<flatbuffers::Vector<uint8_t> *>(VT_TANGENTS);
  }
  const gear::assets::Ref *material() const {
    return GetPointer<const gear::assets::Ref *>(VT_MATERIAL);
  }
  gear::assets::Ref *mutable_material() {
    return GetPointer<gear::assets::Ref *>(VT_MATERIAL);
  }
  const gear::assets::MeshletBuffer *meshlets() const {
    return GetPointer<const gear::assets::MeshletBuffer *>(VT_MESHLETS);
  }
  gear::assets::MeshletBuffer *mutable_meshlets() {
    return GetPointer<gear::assets::MeshletBuffer *>(VT_MESHLETS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_INDEXCOUNT) &&
           VerifyField<uint32_t>(verifier, VT_VERTEXCOUNT) &&
           VerifyOffset(verifier, VT_INDICES) &&
           verifier.VerifyVector(indices()) &&
           VerifyOffset(verifier, VT_POSITION) &&
           verifier.VerifyVector(position()) &&
           VerifyOffset(verifier, VT_TEXCOORD) &&
           verifier.VerifyVector(texcoord()) &&
           VerifyOffset(verifier, VT_NORMALS) &&
           verifier.VerifyVector(normals()) &&
           VerifyOffset(verifier, VT_TANGENTS) &&
           verifier.VerifyVector(tangents()) &&
           VerifyOffset(verifier, VT_MATERIAL) &&
           verifier.VerifyTable(material()) &&
           VerifyOffset(verifier, VT_MESHLETS) &&
           verifier.VerifyTable(meshlets()) &&
           verifier.EndTable();
  }
};

struct MeshPrimitiveBuilder {
  typedef MeshPrimitive Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_indexCount(uint32_t indexCount) {
    fbb_.AddElement<uint32_t>(MeshPrimitive::VT_INDEXCOUNT, indexCount, 0);
  }
  void add_vertexCount(uint32_t vertexCount) {
    fbb_.AddElement<uint32_t>(MeshPrimitive::VT_VERTEXCOUNT, vertexCount, 0);
  }
  void add_indices(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> indices) {
    fbb_.AddOffset(MeshPrimitive::VT_INDICES, indices);
  }
  void add_position(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> position) {
    fbb_.AddOffset(MeshPrimitive::VT_POSITION, position);
  }
  void add_texcoord(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> texcoord) {
    fbb_.AddOffset(MeshPrimitive::VT_TEXCOORD, texcoord);
  }
  void add_normals(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> normals) {
    fbb_.AddOffset(MeshPrimitive::VT_NORMALS, normals);
  }
  void add_tangents(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> tangents) {
    fbb_.AddOffset(MeshPrimitive::VT_TANGENTS, tangents);
  }
  void add_material(flatbuffers::Offset<gear::assets::Ref> material) {
    fbb_.AddOffset(MeshPrimitive::VT_MATERIAL, material);
  }
  void add_meshlets(flatbuffers::Offset<gear::assets::MeshletBuffer> meshlets) {
    fbb_.AddOffset(MeshPrimitive::VT_MESHLETS, meshlets);
  }
  explicit MeshPrimitiveBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MeshPrimitiveBuilder &operator=(const MeshPrimitiveBuilder &);
  flatbuffers::Offset<MeshPrimitive> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<MeshPrimitive>(end);
    return o;
  }
};

inline flatbuffers::Offset<MeshPrimitive> CreateMeshPrimitive(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t indexCount = 0,
    uint32_t vertexCount = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> indices = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> position = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> texcoord = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> normals = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> tangents = 0,
    flatbuffers::Offset<gear::assets::Ref> material = 0,
    flatbuffers::Offset<gear::assets::MeshletBuffer> meshlets = 0) {
  MeshPrimitiveBuilder builder_(_fbb);
  builder_.add_meshlets(meshlets);
  builder_.add_material(material);
  builder_.add_tangents(tangents);
  builder_.add_normals(normals);
  builder_.add_texcoord(texcoord);
  builder_.add_position(position);
  builder_.add_indices(indices);
  builder_.add_vertexCount(vertexCount);
  builder_.add_indexCount(indexCount);
  return builder_.Finish();
}

struct MeshPrimitive::Traits {
  using type = MeshPrimitive;
  static auto constexpr Create = CreateMeshPrimitive;
};

inline flatbuffers::Offset<MeshPrimitive> CreateMeshPrimitiveDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t indexCount = 0,
    uint32_t vertexCount = 0,
    const std::vector<uint8_t> *indices = nullptr,
    const std::vector<uint8_t> *position = nullptr,
    const std::vector<uint8_t> *texcoord = nullptr,
    const std::vector<uint8_t> *normals = nullptr,
    const std::vector<uint8_t> *tangents = nullptr,
    flatbuffers::Offset<gear::assets::Ref> material = 0,
    flatbuffers::Offset<gear::assets::MeshletBuffer> meshlets = 0) {
  auto indices__ = indices ? _fbb.CreateVector<uint8_t>(*indices) : 0;
  auto position__ = position ? _fbb.CreateVector<uint8_t>(*position) : 0;
  auto texcoord__ = texcoord ? _fbb.CreateVector<uint8_t>(*texcoord) : 0;
  auto normals__ = normals ? _fbb.CreateVector<uint8_t>(*normals) : 0;
  auto tangents__ = tangents ? _fbb.CreateVector<uint8_t>(*tangents) : 0;
  return gear::assets::CreateMeshPrimitive(
      _fbb,
      indexCount,
      vertexCount,
      indices__,
      position__,
      texcoord__,
      normals__,
      tangents__,
      material,
      meshlets);
}

struct Mesh FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef MeshBuilder Builder;
  struct Traits;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PRIMITIVES = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<gear::assets::MeshPrimitive>> *primitives() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<gear::assets::MeshPrimitive>> *>(VT_PRIMITIVES);
  }
  flatbuffers::Vector<flatbuffers::Offset<gear::assets::MeshPrimitive>> *mutable_primitives() {
    return GetPointer<flatbuffers::Vector<flatbuffers::Offset<gear::assets::MeshPrimitive>> *>(VT_PRIMITIVES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_PRIMITIVES) &&
           verifier.VerifyVector(primitives()) &&
           verifier.VerifyVectorOfTables(primitives()) &&
           verifier.EndTable();
  }
};

struct MeshBuilder {
  typedef Mesh Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_primitives(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::MeshPrimitive>>> primitives) {
    fbb_.AddOffset(Mesh::VT_PRIMITIVES, primitives);
  }
  explicit MeshBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  MeshBuilder &operator=(const MeshBuilder &);
  flatbuffers::Offset<Mesh> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Mesh>(end);
    return o;
  }
};

inline flatbuffers::Offset<Mesh> CreateMesh(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<gear::assets::MeshPrimitive>>> primitives = 0) {
  MeshBuilder builder_(_fbb);
  builder_.add_primitives(primitives);
  return builder_.Finish();
}

struct Mesh::Traits {
  using type = Mesh;
  static auto constexpr Create = CreateMesh;
};

inline flatbuffers::Offset<Mesh> CreateMeshDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<gear::assets::MeshPrimitive>> *primitives = nullptr) {
  auto primitives__ = primitives ? _fbb.CreateVector<flatbuffers::Offset<gear::assets::MeshPrimitive>>(*primitives) : 0;
  return gear::assets::CreateMesh(
      _fbb,
      primitives__);
}

inline const gear::assets::Mesh *GetMesh(const void *buf) {
  return flatbuffers::GetRoot<gear::assets::Mesh>(buf);
}

inline const gear::assets::Mesh *GetSizePrefixedMesh(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<gear::assets::Mesh>(buf);
}

inline Mesh *GetMutableMesh(void *buf) {
  return flatbuffers::GetMutableRoot<Mesh>(buf);
}

inline bool VerifyMeshBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<gear::assets::Mesh>(nullptr);
}

inline bool VerifySizePrefixedMeshBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<gear::assets::Mesh>(nullptr);
}

inline void FinishMeshBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Mesh> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMeshBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<gear::assets::Mesh> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace assets
}  // namespace gear

#endif  // FLATBUFFERS_GENERATED_MESH_GEAR_ASSETS_H_
