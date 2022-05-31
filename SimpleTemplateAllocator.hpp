#pragma once

#include <utility>
#include <new>
#include <cstdlib>

template<typename T>
struct Allocator
{
    using value_type    = T;
    using pointer       = T*;
    using const_pointer = const T*;

    T* allocate(std::size_t count)
    {
        auto p = malloc(count * sizeof(T));
        if(p == nullptr)
            throw std::bad_alloc{};
        return reinterpret_cast<T*>(p);
    }
    void deallocate(T* p, std::size_t count)
    {
        free(p);
    }
    template<typename U, typename ... Args>
    void construct(U* p, Args&& ... args)
    {
        new(p) U(std::forward<Args>(args)...);
    }
    void destroy(T* p)
    {
        p->~T();
    }
};
