#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 5
#define QUANTUM 2       

typedef struct {
    int pid;            
    int arrival_time;   
    int burst_time;     
    int priority;      
    int remaining_time; 
    int start_time;     
    int finish_time;    
} Process;

// Schduling algorithms
void fcfs(Process processes[], int n) {
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) current_time = processes[i].arrival_time;
        processes[i].start_time = current_time;
        processes[i].finish_time = current_time + processes[i].burst_time;
        current_time = processes[i].finish_time;
    }
}

void sjf(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    int min_burst_time;
    int shortest = -1;

    // Run until all processes complete
    while (completed < n) {
	// Find the shortest job
        min_burst_time = 10000;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].finish_time == 0) {
                if (processes[i].burst_time < min_burst_time) {
                    min_burst_time = processes[i].burst_time;
                    shortest = i;
                }
            }
        }

        if (shortest != -1) {
            processes[shortest].start_time = current_time;
            processes[shortest].finish_time = current_time + processes[shortest].burst_time;
            current_time = processes[shortest].finish_time;
            processes[shortest].remaining_time = 0;
            completed++;
        } else current_time++;
    }
}

void priority_scheduling(Process processes[], int n) {
    int current_time = 0;
    int completed = 0;
    int highest_priority;

    // Run until all processes complete
    while (completed < n) {
	// Find the highest priority
        highest_priority = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].finish_time == 0) {
                if (highest_priority == -1 || processes[i].priority < processes[highest_priority].priority) highest_priority = i;
            }
        }

        if (highest_priority != -1) {
            processes[highest_priority].start_time = current_time;
            processes[highest_priority].finish_time = current_time + processes[highest_priority].burst_time;
            current_time = processes[highest_priority].finish_time;
            processes[highest_priority].remaining_time = 0;
            completed++;
        } else current_time++;
    }
}

void round_robin(Process processes[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;
    int i = 0;

    // Run until all processes complete
    while (completed < n) {
        int ready_process_found = 0;

        // Check if there is any process that can run
        for (int j = 0; j < n; j++) {
            if (processes[j].arrival_time <= current_time && processes[j].remaining_time > 0) {
                ready_process_found = 1;
                break;
            }
        }

        if (ready_process_found) {
            // If a process is ready to run
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                int time_slice = (processes[i].remaining_time > quantum) ? quantum : processes[i].remaining_time;
                processes[i].remaining_time -= time_slice;
                current_time += time_slice;

                if (processes[i].remaining_time == 0) {
                    processes[i].finish_time = current_time;
                    completed++;
                }
            }
            i = (i + 1) % n;
        } else {
            // If no process is ready, just increment time
            current_time++;
        }
    }
}

void priority_with_rr(Process processes[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;

    // Run until all processes complete
    while (completed < n) {
        int highest_priority = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (highest_priority == -1 || processes[i].priority < processes[highest_priority].priority) highest_priority = i;
            }
        }

	// Find the highest priority
        if (highest_priority != -1) {
            int time_slice = (processes[highest_priority].remaining_time > quantum) ? quantum : processes[highest_priority].remaining_time;
            processes[highest_priority].remaining_time -= time_slice;
            current_time += time_slice;

            if (processes[highest_priority].remaining_time == 0) {
                processes[highest_priority].finish_time = current_time;
                completed++;
            }
        } else current_time++;
    }
}

// Printing the result with PID, arrival time, burst time, start time and finish time
void print_processes(Process processes[], int n) {
    printf("PID  Arrival  Burst  Start  Finish\n");
    for (int i = 0; i < n; i++) {
        printf("%d    %d       %d     %d     %d\n", processes[i].pid,
               processes[i].arrival_time, processes[i].burst_time,
               processes[i].start_time, processes[i].finish_time);
    }
    printf("\n");
}

int main() {
    // The test data
    Process processes[MAX_PROCESSES] = {
        {1, 0, 6, 2, 6, 0, 0},
        {2, 2, 8, 1, 8, 0, 0},
        {3, 4, 7, 3, 7, 0, 0},
        {4, 6, 3, 4, 3, 0, 0},
        {5, 8, 4, 2, 4, 0, 0}
    };

    int n = 5;

    printf("FCFS Scheduling:\n");
    Process fcfs_processes[MAX_PROCESSES];
    for (int i = 0; i < n; i++) fcfs_processes[i] = processes[i];
    fcfs(fcfs_processes, n);
    print_processes(fcfs_processes, n);

    printf("SJF Scheduling:\n");
    Process sjf_processes[MAX_PROCESSES];
    for (int i = 0; i < n; i++) sjf_processes[i] = processes[i];
    sjf(sjf_processes, n);
    print_processes(sjf_processes, n);

    printf("Priority Scheduling:\n");
    Process priority_processes[MAX_PROCESSES];
    for (int i = 0; i < n; i++) priority_processes[i] = processes[i];
    priority_scheduling(priority_processes, n);
    print_processes(priority_processes, n);

    printf("Round Robin Scheduling:\n");
    Process rr_processes[MAX_PROCESSES];
    for (int i = 0; i < n; i++) rr_processes[i] = processes[i];
    round_robin(rr_processes, n, QUANTUM);
    print_processes(rr_processes, n);

    printf("Priority with Round Robin Scheduling:\n");
    Process prr_processes[MAX_PROCESSES];
    for (int i = 0; i < n; i++) prr_processes[i] = processes[i];
    priority_with_rr(prr_processes, n, QUANTUM);
    print_processes(prr_processes, n);

    return 0;
}

