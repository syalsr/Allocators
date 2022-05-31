#pragma once

#include <utility>
#include <new>
#include <cstdlib>

template<typename T>
struct Allocator
{
    T* allocate(std::size_t count)
    {
        auto p = malloc(count * sizeof(T));
        if(p == nullptr)
            throw std::bad_alloc{};
        return reinterpret_cast<T*>(p);
    }
    void deallocate(T* p)
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