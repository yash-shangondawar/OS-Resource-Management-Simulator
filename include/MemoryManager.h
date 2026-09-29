#pragma once
#include <string>
#include <vector>
#include "MemoryBlock.h"

struct MemoryResult {
    std::vector<MemoryBlock> blocks;
    int allocated{};
    int freeMemory{};
    double utilization{};
};

MemoryResult firstFit(std::vector<MemoryBlock> blocks, const std::vector<int>& processes);
MemoryResult bestFit(std::vector<MemoryBlock> blocks, const std::vector<int>& processes);
MemoryResult worstFit(std::vector<MemoryBlock> blocks, const std::vector<int>& processes);
MemoryResult nextFit(std::vector<MemoryBlock> blocks, const std::vector<int>& processes);

void printMemoryResult(const MemoryResult& result, const std::string& name);
