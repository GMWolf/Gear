//
// Created by felix on 08/02/2020.
//

#include <gear/ECS/ECS.h>
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
            if (a[i] || i == Component<Entity>::ID()) {
                chunk->ptr[i] = malloc(ComponentInfo::component[i].size * ChunkSize);
            }
        }

        return chunk;
    }

    std::pair<Chunk *, uint16_t> Registry::emplaceEntity(const Archetype &archetype) {
        auto chunk = getFreeChunk(archetype);
        if (!chunk) {
            chunk = createChunk(archetype);
        }

        chunk->size++;

        return std::make_pair(chunk, chunk->size - 1);
    }

    void CommandBuffer::destroyEntity(const Entity &entity) {
        commands.emplace_back(DestroyCommand{entity.id});
    }


    void World::execute(const CreateCommand &createCommand) {
        auto [chunk, eid] = registry.emplaceEntity(createCommand.archetype);
        for(auto& [componentId, componentPointer] : createCommand.components) {
            ComponentInfo::component[componentId].functions.emplace(chunk->get(componentId, eid), componentPointer);
        }
        Entity entity;
        entity.id = getFreeEntityId();
        entity.archetype = createCommand.archetype;
        if (entities.size() <= entity.id)
            entities.resize(entity.id + 1);

        entities[entity.id] = std::make_pair(chunk, eid);

        Component<Entity>::Functions::emplace(chunk->get(Component<Entity>::ID(), eid), &entity);
    }

    void World::execute(const DestroyCommand& command) {
        auto [chunk, index] = entities[command.entityId];


        if (chunk && (chunk->size > 0)) {
            uint16_t idFrom = chunk->size - 1;

            EntityId entityIdFrom = static_cast<Entity *>(chunk->get(Component<Entity>::ID(), idFrom))->id;

            for (int i = 0; i < MaxComponents; i++) {
                if (chunk->archetype[i]) {
                    void *from = chunk->get(i, idFrom);
                    void *to = chunk->get(i, index);
                    if (from != to) {
                        ComponentInfo::component[i].functions.move(from, to);
                    }
                    ComponentInfo::component[i].functions.destroy(from);
                }
            }
            chunk->size--;

            static_cast<Entity*>(chunk->get(Component<Entity>::ID(), index))->id = entityIdFrom;

            entities[entityIdFrom].second = index;
            entities[command.entityId].first = nullptr;
        }

    }

    void World::executeCommandBuffer(const CommandBuffer &commandBuffer) {
        for(auto& c : commandBuffer.commands) {
            //std::visit([this](auto c){execute(c);}, c);
            if (std::holds_alternative<DestroyCommand>(c)) {
                execute(std::get<DestroyCommand>(c));
            } else if (std::holds_alternative<CreateCommand>(c)) {
                execute(std::get<CreateCommand>(c));
            }
        }

    }

    EntityId World::getFreeEntityId() {
        for(int i = 0; i < entities.size(); i++) {
            if (entities[i].first == nullptr) {
                return i;
            }
        }
        return nextEntityId++;
    }

    RegistryView World::getChunks(const Archetype &archetype) {
        return RegistryView(registry, archetype);
    }

    void *Chunk::getData(ComponentId id) {
        return ptr[id];
    }

    void *Chunk::get(ComponentId componentId, uint16_t index) {
        return (char*)getData(componentId) + (index * ComponentInfo::component[componentId].size);
    }

    CreateCommand::~CreateCommand() {
        for(auto [id, ptr] : components) {
            ComponentInfo::component[id].functions.destroy(ptr);
        }
    }

    CreateCommand::CreateCommand(CreateCommand && o) noexcept :
    archetype(o.archetype),
    components(std::move(o.components)){
        o.components.clear();
    }

    bool RegistryIterator::operator==(const RegistryIterator &o) const {
        return chunkIt == o.chunkIt;
    }

    bool RegistryIterator::operator!=(const RegistryIterator &o) const {
        return !(*this == o);
    }

    Chunk &RegistryIterator::operator*() {
        assert(archetypeIt->first.matches(archetype));
        return **chunkIt;
    }

    RegistryIterator &RegistryIterator::operator++() {
        chunkIt++;
        if (chunkIt == archetypeIt->second.end()) {
            do {
                archetypeIt++;
            } while(
                    (archetypeIt != archetypeEnd) &&
                    (!archetypeIt->first.matches(archetype))
                    );
            if (archetypeIt != archetypeEnd) {
                chunkIt = archetypeIt->second.begin();
            } else {
                chunkIt = {};
            }
        }

        return *this;
    }

    RegistryIterator RegistryIterator::operator++(int) &{
        auto temp = *this;
        ++*this;
        return temp;
    }

    RegistryIterator::RegistryIterator(Registry::Store::iterator archetypeIt, Registry::Store::iterator archetypeEnd,
                                       Registry::ChunkVec::iterator chunkIt, const Archetype &archetype) :
            archetypeIt(archetypeIt),
            archetypeEnd(archetypeEnd),
            chunkIt(chunkIt),
            archetype(archetype)
    {
    }

    RegistryIterator RegistryView::begin() {

        auto ait = registry.archetypeChunks.begin();
        auto aend = registry.archetypeChunks.end();
        while(ait != aend && !ait->first.matches(archetype)) {
            ++ait;
        }

        return RegistryIterator(ait, aend,ait != aend ? ait->second.begin() : Registry::ChunkVec::iterator{nullptr}, archetype);
    }

    RegistryIterator RegistryView::end() {
        return RegistryIterator(registry.archetypeChunks.end(), registry.archetypeChunks.end(), Registry::ChunkVec::iterator{nullptr}, archetype);
    }

    RegistryView::RegistryView(Registry &registry, const Archetype &archetype) : registry(registry), archetype(archetype) {
    }
}