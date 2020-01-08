//
// Created by felix on 08/01/2020.
//

#ifndef GEAR_DEPENDENCYINJECTION_H
#define GEAR_DEPENDENCYINJECTION_H

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <functional>
#include <type_traits>

namespace gear {

    class World {
    public:

        template<class T, class... Args>
        void emplace(Args&&... args);

        template<class T>
        T& get();

        template<class R, class... Args,
                std::enable_if_t<!std::is_void<R>::value, int> = 0>
        R invoke(std::function<R(Args...)> f);

        template<class R, class... Args,
                std::enable_if_t<std::is_void<R>::value, int> = 0>
        void invoke(std::function<R(Args...)> f);

        template<class R, class... Args,
                std::enable_if_t<!std::is_void<R>::value, int> = 0>
        R invoke(R(*f)(Args...));

        template<class R, class... Args,
                std::enable_if_t<std::is_void<R>::value, int> = 0>
        void invoke(R(*f)(Args...));

        void reset();

    private:
        std::unordered_map<std::type_index, std::shared_ptr<void>> store;
    };

    template<class T, class... Args>
    void World::emplace(Args &&... args) {
        store.emplace(std::make_pair(
                std::type_index(typeid(T)),
                std::make_shared<T>(args...))
        );
    }

    template<class T>
    T& World::get() {
        auto f = store.find(std::type_index(typeid(T)));
        if (f != store.end()) {
            return *std::static_pointer_cast<T>( f->second );
        } else {
            throw std::exception();
        }
    }

    void World::reset() {
        store.clear();
    }

    template<class R, class... Args,
            std::enable_if_t<!std::is_void<R>::value, int>>
    R World::invoke(std::function<R(Args...)> f) {
        return f(get<std::remove_reference_t<Args>>()...);
    }

    template<class R, class... Args, std::enable_if_t<!std::is_void<R>::value, int>>
    R World::invoke(R (*f)(Args...)) {
        return f(get<std::remove_reference_t<Args>>()...);
    }

    template<class R, class... Args,
            std::enable_if_t<std::is_void<R>::value, int>>
    void World::invoke(std::function<R(Args...)> f) {
        f(get<std::remove_reference_t<Args>>()...);
    }

    template<class R, class... Args, std::enable_if_t<std::is_void<R>::value, int>>
    void World::invoke(R (*f)(Args...)) {
        f(get<std::remove_reference_t<Args>>()...);
    }

}

#endif //GEAR_DEPENDENCYINJECTION_H
