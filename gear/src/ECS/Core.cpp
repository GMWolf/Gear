//
// Created by felix on 08/02/2020.
//

#include <gear/ECS/Core.h>
#include <gear/ECS/Component.h>
#include <iostream>
#include <cassert>

namespace gear::ecs {

    ComponentId nextComponentId = 0;

    Chunk* Registry::getFreeChunk(const Archetype &a)  {
        auto s = archetypeChunks.find(a);
        if (s != archetypeChunks.end()) {
            auto& [key, chunks] = *s;
            for(auto& chunk : chunks) {
                if (chunk->size < ChunkSize) {
                    return chunk.get();
                }
            }
        }

        return nullptr;
    }

    Chunk* Registry::createChunk(const Archetype& a) {
        auto&[key, chunks] = *archetypeChunks.try_emplace(a).first;

        Chunk* chunk = chunks.emplace_back(std::make_unique<Chunk>(a)).get();

        for (ComponentId i = 0; i < MaxComponents; i++) {
            if (a[i]) {
                chunk->ptr[i] = malloc(ComponentInfo::component[i].size * ChunkSize);
            }
        }

        return chunk;
    }



    void Registry::emplaceEntity(const Archetype &archetype, Entity* entity) {
        Archetype realArchetype = archetype | Component<EntityRef>::ID();
        auto chunk = getFreeChunk(realArchetype);
        if (!chunk) {
            chunk = createChunk(realArchetype);
        }

        uint16_t eid = chunk->size;

        entity->chunk = chunk;
        entity->index = eid;

        static_cast<EntityRef*>(chunk->get(Component<EntityRef>::ID(), eid))->entity = entity;
        static_cast<EntityRef*>(chunk->get(Component<EntityRef>::ID(), eid))->version = entity->version;

        chunk->size++;
    }

    Entity *EntityPool::getFreeEntity() {
        if (freeEntities.empty()) {
            auto& slab = entitySlabVec.emplace_back(std::make_unique<EntitySlab>());
            for(auto& e : *slab) {
                e.version = 1;
                e.chunk = nullptr;
                e.index = 0;
                freeEntities.push_back(&e);
            }
        }

        auto ret = freeEntities.back();
        freeEntities.pop_back();
        return ret;
    }


    void EntityPool::getFreeEntities(Entity **vec, size_t count) {
        for(int i = 0; i < count; i++) {
            vec[count] = getFreeEntity();
        }
    }

    void EntityPool::free(Entity *entity) {
        freeEntities.push_back(entity);
    }

    size_t EntityPool::getAllocatedEntityCount() {
        return entitySlabSize * entitySlabVec.size();
    }

    void Registry::emplaceComponent(Entity *entity, ComponentId componentId, void * componentPtr) {
        ComponentInfo::component[componentId].functions.emplaceMove(entity->chunk->get(componentId, entity->index), componentPtr);
    }


    static void destroyEntityFromChunk(Chunk* chunk, uint16_t index) {
        //move last index into entity being destroyed.
        //Then destroy last index

        uint16_t fromIndex = chunk->size - 1;
        Entity* entityFrom = static_cast<EntityRef *>(chunk->get(Component<EntityRef>::ID(), fromIndex))->entity;

        for(int i = 0; i < MaxComponents; i++) {
            if (chunk->archetype[i]) {
                void* from = chunk->get(i, fromIndex);
                if(fromIndex != index) {
                    void* to = chunk->get(i, index);
                    ComponentInfo::component[i].functions.move(from, to);
                }
                ComponentInfo::component[i].functions.destroy(from);
            }
        }

        //fixup moved-from entity index
        if (fromIndex != index) {
            entityFrom->index = index;
        }

        chunk->size--;
    }


    void Registry::destroyEntity(Entity *entity) {
        Chunk* chunk = entity->chunk;
        uint16_t index = entity->index;
        destroyEntityFromChunk(chunk, index);

        //make destroyed entity available
        entity->chunk = nullptr;
        entity->version++;
    }

    void Registry::mutateEntity(Entity* entity, const Archetype& archetype) {
        if (entity->chunk->archetype != archetype) {

            auto chunkFrom = entity->chunk;
            auto indexFrom = entity->index;
            //create a new entity for it
            auto newChunk = getFreeChunk(archetype);
            if (!newChunk) {
                newChunk = createChunk(archetype);
            }
            uint16_t eid = newChunk->size;
            newChunk->size++;

            //move components components
            auto sharedArchetype = archetype & entity->chunk->archetype;

            for (int i = 0; i < sharedArchetype.bits.size(); i++) {
                if (sharedArchetype[i]) {
                    void *from = chunkFrom->get(i, indexFrom);
                    void *to = newChunk->get(i, eid);
                    ComponentInfo::component[i].functions.emplaceMove(to, from);
                }
            }

            //Destroy old entity
            destroyEntityFromChunk(chunkFrom, indexFrom);

            //fixup moved entity
            entity->chunk = newChunk;
            entity->index = eid;
        }
    }


    ArrayRange<Chunk *> Registry::queryChunks(const Query &query, Chunk **outChunks, size_t outArraySize) {
        if (outArraySize == 0) {
            return {};
        }
        size_t count = 0;
        for(auto& [arch, chunks] : archetypeChunks) {
            if (testQuery(query, arch)) {
                for(auto& chunkPtr : chunks) {
                    if (count == outArraySize) {
                        std::cerr << "Ran out of chunk buffer space" << std::endl;
                        return {outChunks, outChunks+count};
                    }
                    outChunks[count++] = chunkPtr.get();
                }
            }
        }
        return {outChunks, outChunks+count};
    }

    Registry::View Registry::query(const Query &query) {
        return { Iterator{query,archetypeChunks}, Iterator{ archetypeChunks } };
    }

    void *Chunk::getData(ComponentId id) {
        return ptr[id];
    }

    void *Chunk::get(ComponentId componentId, uint16_t index) {
        return (char*)getData(componentId) + (index * ComponentInfo::component[componentId].size);
    }

    bool EntityRef::alive() const {
        return version == entity->version;
    }

    Registry::Iterator &Registry::Iterator::operator++() {
        vecit++;
        if (vecit == mapit->second.end()) {
            do {
                mapit++;
            } while(mapit != mapend && !testQuery(query, mapit->first));
            vecit = mapit == mapend ? ChunkVec::iterator{} : mapit->second.begin();
        }
        return *this;
    }

    bool Registry::Iterator::operator==(const Registry::Iterator &o) const {
        return o.mapit == mapit && o.vecit == vecit;
    }

    bool Registry::Iterator::operator!=(const Registry::Iterator &o) const {
        return !(*this == o);
    }

    Chunk* Registry::Iterator::operator*() const {
        return vecit->get();
    }

    Registry::Iterator::Iterator(const Query& q, Registry::Store& s) :
    query(q), mapit(s.begin()), mapend(s.end()) {
        if (mapit == mapend) {
            vecit = {};
        } else {
            vecit = mapit->second.begin();
            if (!testQuery(q, mapit->first)) {
                ++*this;
            }
        }
    }

    Registry::Iterator::Iterator(Registry::Store &s) :query({}), mapit(s.end()), mapend(s.end()), vecit({}) {
    }

}