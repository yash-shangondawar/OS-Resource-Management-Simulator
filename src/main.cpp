#include <iostream>
#include <vector>
#include "Scheduler.h"
#include "MemoryManager.h"

using namespace std;

vector<Process> readProcesses() {
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> p(n);
    for(int i=0;i<n;++i){
        p[i].pid=i+1;
        cout << "P" << i+1 << " - Arrival Burst Priority: ";
        cin >> p[i].arrival >> p[i].burst >> p[i].priority;
        p[i].remaining=p[i].burst;
    }
    return p;
}

vector<MemoryBlock> readBlocks() {
    int n;
    cout << "Enter number of memory blocks: ";
    cin >> n;
    vector<MemoryBlock> blocks(n);
    for(int i=0;i<n;++i){
        blocks[i].id=i+1;
        cout << "Block " << i+1 << " size (KB): ";
        cin >> blocks[i].size;
    }
    return blocks;
}

vector<int> readMemoryProcesses() {
    int n;
    cout << "Enter number of memory requests: ";
    cin >> n;
    vector<int> p(n);
    for(int i=0;i<n;++i){
        cout << "Request P" << i+1 << " size (KB): ";
        cin >> p[i];
    }
    return p;
}

int main(){
    cout << "============================================\n";
    cout << "       OS RESOURCE MANAGEMENT SIMULATOR\n";
    cout << "============================================\n";

    while(true){
        cout << "\n1. CPU Scheduling\n";
        cout << "2. Memory Management\n";
        cout << "3. Compare Scheduling Algorithms\n";
        cout << "4. Compare Memory Allocation Algorithms\n";
        cout << "5. Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        if(choice==5) break;

        if(choice==1){
            auto p=readProcesses();
            cout << "\n1.FCFS  2.SJF  3.SRTF  4.Priority  5.Round Robin\nChoice: ";
            int c; cin>>c;
            if(c==1) printScheduleResult(fcfs(p),"FCFS");
            else if(c==2) printScheduleResult(sjf(p),"SJF");
            else if(c==3) printScheduleResult(srtf(p),"SRTF");
            else if(c==4) printScheduleResult(priorityScheduling(p),"Priority");
            else if(c==5){
                int q; cout<<"Time Quantum: "; cin>>q;
                printScheduleResult(roundRobin(p,q),"Round Robin");
            }
        }
        else if(choice==2){
            auto blocks=readBlocks();
            auto req=readMemoryProcesses();
            cout << "\n1.First Fit  2.Best Fit  3.Worst Fit  4.Next Fit\nChoice: ";
            int c; cin>>c;
            if(c==1) printMemoryResult(firstFit(blocks,req),"First Fit");
            else if(c==2) printMemoryResult(bestFit(blocks,req),"Best Fit");
            else if(c==3) printMemoryResult(worstFit(blocks,req),"Worst Fit");
            else if(c==4) printMemoryResult(nextFit(blocks,req),"Next Fit");
        }
        else if(choice==3){
            auto p=readProcesses();
            auto a=fcfs(p), b=sjf(p), c=srtf(p), d=priorityScheduling(p);
            int q; cout<<"Time Quantum for Round Robin: "; cin>>q;
            auto e=roundRobin(p,q);

            cout << "\nAlgorithm Comparison\n";
            cout << "FCFS       WT="<<a.avgWaiting<<" TAT="<<a.avgTurnaround<<" RT="<<a.avgResponse<<"\n";
            cout << "SJF        WT="<<b.avgWaiting<<" TAT="<<b.avgTurnaround<<" RT="<<b.avgResponse<<"\n";
            cout << "SRTF       WT="<<c.avgWaiting<<" TAT="<<c.avgTurnaround<<" RT="<<c.avgResponse<<"\n";
            cout << "Priority    WT="<<d.avgWaiting<<" TAT="<<d.avgTurnaround<<" RT="<<d.avgResponse<<"\n";
            cout << "Round Robin WT="<<e.avgWaiting<<" TAT="<<e.avgTurnaround<<" RT="<<e.avgResponse<<"\n";
        }
        else if(choice==4){
            auto blocks=readBlocks();
            auto req=readMemoryProcesses();
            auto a=firstFit(blocks,req), b=bestFit(blocks,req);
            auto c=worstFit(blocks,req), d=nextFit(blocks,req);

            cout << "\nMemory Comparison\n";
            cout << "First Fit  : "<<a.utilization<<"% utilization\n";
            cout << "Best Fit   : "<<b.utilization<<"% utilization\n";
            cout << "Worst Fit  : "<<c.utilization<<"% utilization\n";
            cout << "Next Fit   : "<<d.utilization<<"% utilization\n";
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    cout << "\nGoodbye!\n";
    return 0;
}
