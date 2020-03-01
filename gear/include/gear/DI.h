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

    class DI {
    public:

        template<class T, class... Args>
        T& emplace(Args&&... args);

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

        template<class... T>
        void inject(std::tuple<T...>& tuple);

        template<class T>
        void inject(T& t);
        void reset();

    private:
        std::unordered_map<std::type_index, std::shared_ptr<void>> store;
    };

    template<class T, class... Args>
    T& DI::emplace(Args &&... args) {
        auto ptr = store.emplace(std::make_pair(
                std::type_index(typeid(T)),
                std::make_shared<T>(args...))
        ).first;
        return *static_cast<T*>(ptr->second.get());
    }

    template<class T>
    T& DI::get() {
        auto f = store.find(std::type_index(typeid(T)));
        if (f != store.end()) {
            return *std::static_pointer_cast<T>( f->second );
        } else {
            throw std::exception();
        }
    }

    void DI::reset() {
        store.clear();
    }

    template<class R, class... Args,
            std::enable_if_t<!std::is_void<R>::value, int>>
    R DI::invoke(std::function<R(Args...)> f) {
        return f(get<std::remove_reference_t<Args>>()...);
    }

    template<class R, class... Args, std::enable_if_t<!std::is_void<R>::value, int>>
    R DI::invoke(R (*f)(Args...)) {
        return f(get<std::remove_reference_t<Args>>()...);
    }

    template<class... T>
    void DI::inject(std::tuple<T...> &tuple) {
        ((std::get<T>(tuple) = get<std::remove_reference_t<T>>()), ...);
    }

    template<class T>
    void DI::inject(T &t) {
        inject(t.di);
    }

    template<class R, class... Args,
            std::enable_if_t<std::is_void<R>::value, int>>
    void DI::invoke(std::function<R(Args...)> f) {
        f(get<std::remove_reference_t<Args>>()...);
    }

    template<class R, class... Args, std::enable_if_t<std::is_void<R>::value, int>>
    void DI::invoke(R (*f)(Args...)) {
        f(get<std::remove_reference_t<Args>>()...);
    }

}

#endif //GEAR_DEPENDENCYINJECTION_H
