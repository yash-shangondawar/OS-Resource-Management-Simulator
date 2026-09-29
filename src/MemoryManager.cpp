#include "MemoryManager.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <climits>

using namespace std;

static MemoryResult finish(vector<MemoryBlock> blocks) {
    int total=0, allocated=0;
    for(auto& b:blocks){
        total += b.size;
        if(b.allocatedTo != -1) allocated += b.size;
    }

    MemoryResult r;
    r.blocks=blocks;
    r.allocated=allocated;
    r.freeMemory=total-allocated;
    r.utilization=total ? 100.0*allocated/total : 0.0;
    return r;
}

MemoryResult firstFit(vector<MemoryBlock> blocks,const vector<int>& processes){
    for(int i=0;i<(int)processes.size();++i)
        for(auto& b:blocks)
            if(b.allocatedTo==-1 && b.size>=processes[i]){
                b.allocatedTo=i+1;
                break;
            }
    return finish(blocks);
}

MemoryResult bestFit(vector<MemoryBlock> blocks,const vector<int>& processes){
    for(int i=0;i<(int)processes.size();++i){
        int idx=-1;
        for(int j=0;j<(int)blocks.size();++j)
            if(blocks[j].allocatedTo==-1 && blocks[j].size>=processes[i])
                if(idx==-1 || blocks[j].size<blocks[idx].size) idx=j;
        if(idx!=-1) blocks[idx].allocatedTo=i+1;
    }
    return finish(blocks);
}

MemoryResult worstFit(vector<MemoryBlock> blocks,const vector<int>& processes){
    for(int i=0;i<(int)processes.size();++i){
        int idx=-1;
        for(int j=0;j<(int)blocks.size();++j)
            if(blocks[j].allocatedTo==-1 && blocks[j].size>=processes[i])
                if(idx==-1 || blocks[j].size>blocks[idx].size) idx=j;
        if(idx!=-1) blocks[idx].allocatedTo=i+1;
    }
    return finish(blocks);
}

MemoryResult nextFit(vector<MemoryBlock> blocks,const vector<int>& processes){
    int start=0;
    for(int i=0;i<(int)processes.size();++i){
        bool allocated=false;
        for(int count=0;count<(int)blocks.size();++count){
            int j=(start+count)%blocks.size();
            if(blocks[j].allocatedTo==-1 && blocks[j].size>=processes[i]){
                blocks[j].allocatedTo=i+1;
                start=(j+1)%blocks.size();
                allocated=true;
                break;
            }
        }
        (void)allocated;
    }
    return finish(blocks);
}

void printMemoryResult(const MemoryResult& r,const string& name){
    cout << "\n===== " << name << " =====\n";
    cout << left << setw(10) << "Block" << setw(12) << "Size(KB)"
         << setw(15) << "Allocated To" << "\n";
    for(auto& b:r.blocks){
        string owner=b.allocatedTo==-1 ? "Free" : "P"+to_string(b.allocatedTo);
        cout << left << setw(10) << b.id << setw(12) << b.size
             << setw(15) << owner << "\n";
    }
    cout << fixed << setprecision(2);
    cout << "\nAllocated Memory   : " << r.allocated << " KB";
    cout << "\nFree Memory        : " << r.freeMemory << " KB";
    cout << "\nMemory Utilization : " << r.utilization << "%\n";
}
