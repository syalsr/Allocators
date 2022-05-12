#pragma once

#include <cstdio>
#include <cstdlib>
#include <new>
#include <algorithm>

class StackAllocator
{
public:
    explicit StackAllocator(std::size_t size);
    StackAllocator(StackAllocator&& other) noexcept ;
    StackAllocator& operator=(StackAllocator&& other) noexcept ;
    ~StackAllocator();

    StackAllocator(const StackAllocator& other)=delete;
    StackAllocator& operator=(const StackAllocator& other)=delete;

    void* Allocate(std::size_t size_memory);
    void Deallocate();
    void Init(std::size_t size = 0);
private:
    void* begin{};
    std::size_t size_memory_{};
    std::size_t offset_{};
};

StackAllocator::StackAllocator(std::size_t size) :size_memory_{size}
{}

StackAllocator::StackAllocator(StackAllocator &&other) noexcept {
    size_memory_    = other.size_memory_;
    offset_         = other.offset_;
    begin           = other.begin;
    other.begin     = nullptr;
}

StackAllocator &StackAllocator::operator=(StackAllocator &&other) noexcept {
    if(this != &other)
    {
        std::swap(size_memory_, other.size_memory_);
        std::swap(offset_,      other.offset_);
        std::swap(begin,        other.begin);
    }
    return *this;
}

StackAllocator::~StackAllocator() {
    Deallocate();
}

void *StackAllocator::Allocate(std::size_t size_memory) {
    if(offset_ + size_memory + sizeof(std::size_t) > size_memory_)
        return nullptr;

    const std::size_t header = reinterpret_cast<std::size_t>(begin) + offset_;
    *(reinterpret_cast<size_t*>(header)) = size_memory;
    const std::size_t next_chunk = reinterpret_cast<std::size_t>(begin) + offset_;
    offset_ +=size_memory;

    return reinterpret_cast<void*>(next_chunk);
}

void StackAllocator::Deallocate() {
    size_memory_ = offset_ = 0;
    free(begin);
}

void StackAllocator::Init(std::size_t size) {
    if(size == 0)
        begin = malloc(size_memory_);
    else {
        begin = malloc(size);
        size_memory_ = size;
        offset_ = 0;
    }

    if (begin == nullptr)
        throw std::bad_alloc{};
}
