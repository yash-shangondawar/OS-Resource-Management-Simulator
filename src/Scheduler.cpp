#include "Scheduler.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <climits>

using namespace std;

static void addSegment(vector<Segment>& g, int pid, int start, int end) {
    if (start >= end) return;
    if (!g.empty() && g.back().pid == pid && g.back().end == start)
        g.back().end = end;
    else
        g.push_back({pid, start, end});
}

static ScheduleResult finalize(vector<Process> p, vector<Segment> g) {
    double wt=0, tat=0, rt=0;
    int busy=0, endTime=0;

    for (auto& x : p) {
        x.turnaround = x.completion - x.arrival;
        x.waiting = x.turnaround - x.burst;
        x.response = x.firstStart - x.arrival;
        wt += x.waiting;
        tat += x.turnaround;
        rt += x.response;
        busy += x.burst;
        endTime = max(endTime, x.completion);
    }

    sort(p.begin(), p.end(), [](const Process& a, const Process& b) {
        return a.pid < b.pid;
    });

    ScheduleResult r;
    r.processes = p;
    r.gantt = g;
    if (!p.empty()) {
        r.avgWaiting = wt / p.size();
        r.avgTurnaround = tat / p.size();
        r.avgResponse = rt / p.size();
    }
    r.cpuUtilization = endTime ? (100.0 * busy / endTime) : 0.0;
    return r;
}

ScheduleResult fcfs(vector<Process> p) {
    sort(p.begin(), p.end(), [](const Process& a, const Process& b) {
        if (a.arrival != b.arrival) return a.arrival < b.arrival;
        return a.pid < b.pid;
    });

    vector<Segment> g;
    int time = 0;

    for (auto& x : p) {
        if (time < x.arrival) {
            addSegment(g, -1, time, x.arrival);
            time = x.arrival;
        }
        x.firstStart = time;
        addSegment(g, x.pid, time, time + x.burst);
        time += x.burst;
        x.completion = time;
    }
    return finalize(p, g);
}

ScheduleResult sjf(vector<Process> p) {
    vector<Segment> g;
    vector<bool> done(p.size(), false);
    int completed=0, time=0;

    while (completed < (int)p.size()) {
        int idx=-1;
        for (int i=0;i<(int)p.size();++i) {
            if (!done[i] && p[i].arrival <= time) {
                if (idx==-1 || p[i].burst < p[idx].burst ||
                    (p[i].burst == p[idx].burst && p[i].arrival < p[idx].arrival))
                    idx=i;
            }
        }

        if (idx==-1) {
            int next=INT_MAX;
            for (int i=0;i<(int)p.size();++i)
                if (!done[i]) next=min(next,p[i].arrival);
            addSegment(g,-1,time,next);
            time=next;
            continue;
        }

        p[idx].firstStart=time;
        addSegment(g,p[idx].pid,time,time+p[idx].burst);
        time += p[idx].burst;
        p[idx].completion=time;
        done[idx]=true;
        ++completed;
    }
    return finalize(p,g);
}

ScheduleResult priorityScheduling(vector<Process> p) {
    vector<Segment> g;
    vector<bool> done(p.size(), false);
    int completed=0, time=0;

    while (completed < (int)p.size()) {
        int idx=-1;
        for (int i=0;i<(int)p.size();++i) {
            if (!done[i] && p[i].arrival <= time) {
                if (idx==-1 || p[i].priority < p[idx].priority ||
                    (p[i].priority == p[idx].priority && p[i].arrival < p[idx].arrival))
                    idx=i;
            }
        }

        if (idx==-1) {
            int next=INT_MAX;
            for (int i=0;i<(int)p.size();++i)
                if (!done[i]) next=min(next,p[i].arrival);
            addSegment(g,-1,time,next);
            time=next;
            continue;
        }

        p[idx].firstStart=time;
        addSegment(g,p[idx].pid,time,time+p[idx].burst);
        time += p[idx].burst;
        p[idx].completion=time;
        done[idx]=true;
        ++completed;
    }
    return finalize(p,g);
}

ScheduleResult srtf(vector<Process> p) {
    vector<Segment> g;
    for (auto& x : p) x.remaining=x.burst;

    int completed=0, time=0;
    while (completed < (int)p.size()) {
        int idx=-1;
        for (int i=0;i<(int)p.size();++i) {
            if (p[i].arrival <= time && p[i].remaining > 0) {
                if (idx==-1 || p[i].remaining < p[idx].remaining ||
                    (p[i].remaining == p[idx].remaining && p[i].arrival < p[idx].arrival))
                    idx=i;
            }
        }

        if (idx==-1) {
            int next=INT_MAX;
            for (auto& x:p) if (x.remaining>0) next=min(next,x.arrival);
            addSegment(g,-1,time,next);
            time=next;
            continue;
        }

        if (p[idx].firstStart==-1) p[idx].firstStart=time;
        addSegment(g,p[idx].pid,time,time+1);
        ++time;
        --p[idx].remaining;

        if (p[idx].remaining==0) {
            p[idx].completion=time;
            ++completed;
        }
    }
    return finalize(p,g);
}

ScheduleResult roundRobin(vector<Process> p, int quantum) {
    for (auto& x:p) x.remaining=x.burst;

    sort(p.begin(),p.end(),[](const Process& a,const Process& b){
        if(a.arrival!=b.arrival) return a.arrival<b.arrival;
        return a.pid<b.pid;
    });

    vector<Segment> g;
    queue<int> q;
    int time=0, next=0, completed=0;

    while(completed<(int)p.size()){
        if(q.empty() && next<(int)p.size() && time<p[next].arrival){
            addSegment(g,-1,time,p[next].arrival);
            time=p[next].arrival;
        }

        while(next<(int)p.size() && p[next].arrival<=time) q.push(next++);

        if(q.empty()) continue;

        int i=q.front(); q.pop();
        if(p[i].firstStart==-1) p[i].firstStart=time;

        int run=min(quantum,p[i].remaining);
        addSegment(g,p[i].pid,time,time+run);
        time+=run;
        p[i].remaining-=run;

        while(next<(int)p.size() && p[next].arrival<=time) q.push(next++);

        if(p[i].remaining>0) q.push(i);
        else {
            p[i].completion=time;
            ++completed;
        }
    }

    return finalize(p,g);
}

void printGantt(const vector<Segment>& g) {
    cout << "\nGantt Chart:\n";
    for(auto& s:g) cout << "| " << (s.pid==-1 ? "IDLE" : "P"+to_string(s.pid)) << " ";
    cout << "|\n";
    if(!g.empty()){
        cout << g.front().start;
        for(auto& s:g) cout << setw(7) << s.end;
        cout << "\n";
    }
}

void printScheduleResult(const ScheduleResult& r,const string& name){
    cout << "\n===== " << name << " =====\n";
    cout << left << setw(6) << "PID" << setw(6) << "AT" << setw(6) << "BT"
         << setw(6) << "CT" << setw(7) << "TAT" << setw(6) << "WT"
         << setw(6) << "RT" << "\n";

    for(auto& x:r.processes)
        cout << left << setw(6) << x.pid << setw(6) << x.arrival
             << setw(6) << x.burst << setw(6) << x.completion
             << setw(7) << x.turnaround << setw(6) << x.waiting
             << setw(6) << x.response << "\n";

    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time    : " << r.avgWaiting;
    cout << "\nAverage Turnaround Time : " << r.avgTurnaround;
    cout << "\nAverage Response Time   : " << r.avgResponse;
    cout << "\nCPU Utilization         : " << r.cpuUtilization << "%\n";
    printGantt(r.gantt);
}
