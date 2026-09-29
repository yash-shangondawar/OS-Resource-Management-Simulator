#pragma once

struct Process {
    int pid{};
    int arrival{};
    int burst{};
    int priority{};
    int remaining{};
    int firstStart{-1};
    int completion{0};
    int turnaround{0};
    int waiting{0};
    int response{0};
};
