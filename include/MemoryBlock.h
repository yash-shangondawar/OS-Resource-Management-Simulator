#pragma once

struct MemoryBlock {
    int id{};
    int size{};
    int allocatedTo{-1};
};
