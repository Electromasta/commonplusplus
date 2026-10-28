#pragma once

namespace common    {
    template<typename T>
    class singleton   {
        public:
            static T & instance() {
                static T instance;
                return instance;
            }
        protected:
            singleton() = default;
            virtual ~singleton() = default;
        private:
            singleton(const singleton&) = delete;
            singleton& operator=(const singleton&) = delete;
            singleton(singleton&&) = delete;
            singleton& operator=(singleton&&) = delete;
    };
}

/// Meyer's Singleton - thread-safe, single-instance pattern.
///
/// Derived classes must add to private:
/// - friend Singleton<MySingleton>;
/// - MySingleton() = default;
/// - ~MySingleton() = default;
///
/// The friend declaration takes no `class` keyword: Singleton is an alias
/// template, and an elaborated-type-specifier (`friend class X<T>;`) may only
/// name a real class. The extended friend form below accepts the alias.
template<typename T>
using Singleton = common::singleton<T>;
