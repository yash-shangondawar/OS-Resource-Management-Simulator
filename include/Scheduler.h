#pragma once
#include <string>
#include <vector>
#include "Process.h"

struct Segment {
    int pid;
    int start;
    int end;
};

struct ScheduleResult {
    std::vector<Process> processes;
    std::vector<Segment> gantt;
    double avgWaiting{};
    double avgTurnaround{};
    double avgResponse{};
    double cpuUtilization{};
};

ScheduleResult fcfs(std::vector<Process> processes);
ScheduleResult sjf(std::vector<Process> processes);
ScheduleResult srtf(std::vector<Process> processes);
ScheduleResult priorityScheduling(std::vector<Process> processes);
ScheduleResult roundRobin(std::vector<Process> processes, int quantum);

void printScheduleResult(const ScheduleResult& result, const std::string& name);
void printGantt(const std::vector<Segment>& gantt);
