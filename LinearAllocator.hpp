#pragma once

#include <cstdio>
#include <cstdlib>
#include <new>
#include <algorithm>

class LinearAllocator
{
public:
    explicit LinearAllocator(std::size_t size);
    LinearAllocator(LinearAllocator&& other) noexcept ;
    LinearAllocator& operator=(LinearAllocator&& other) noexcept ;
    ~LinearAllocator();

    LinearAllocator(const LinearAllocator& other)=delete;
    LinearAllocator& operator=(const LinearAllocator& other)=delete;

    void* Allocate(std::size_t size_memory);
    void Deallocate();
    void Init(std::size_t size = 0);
private:
    void* begin{};
    std::size_t size_memory_{};
    std::size_t offset_{};
};

LinearAllocator::LinearAllocator(std::size_t size) : size_memory_{size}
{}

LinearAllocator::LinearAllocator(LinearAllocator &&other) noexcept {
    size_memory_    = other.size_memory_;
    offset_         = other.offset_;
    begin           = other.begin;
    other.begin     = nullptr;
}

LinearAllocator &LinearAllocator::operator=(LinearAllocator &&other) noexcept {
    if(this != &other)
    {
        std::swap(size_memory_, other.size_memory_);
        std::swap(offset_,      other.offset_);
        std::swap(begin,        other.begin);
    }
    return *this;
}

LinearAllocator::~LinearAllocator() {
    Deallocate();
}

void *LinearAllocator::Allocate(std::size_t size_memory) {
    if(offset_ + size_memory > size_memory_)
        return nullptr;

    const std::size_t next_chunk = reinterpret_cast<std::size_t>(begin) + offset_;
    offset_ += size_memory;

    return reinterpret_cast<void*>(next_chunk);
}

void LinearAllocator::Deallocate() {
    size_memory_ = offset_ = 0;
    free(begin);
}

void LinearAllocator::Init(std::size_t size) {
    if(size == 0)
        begin = malloc(size_memory_);
    else
        begin = malloc(size);

    if (begin == nullptr)
        throw std::bad_alloc{};
}

