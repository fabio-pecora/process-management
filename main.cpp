#include <iostream>
#include <vector>
#include <climits>
#include <queue>



using namespace std;

class Process {
public:
    string name;
    int arrival_time;
    int priority;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;

    Process(string n, int at, int p, int bt) {
        name = n;
        arrival_time = at;
        priority = p;
        burst_time = bt;
        start_time = 0;
        completion_time = 0;
        turnaround_time = 0;
    }
};

void fcfs(vector<Process>& processes);
void priorityy(vector<Process>& processes);
void sjn(vector<Process>& processes);

int main() {
    int num_processes;
    vector<Process> processes;

    cout << "Enter the number of processes: ";
    cin >> num_processes;

    if (num_processes <= 0) {
        cout << "Number of processes must be greater than 0." << endl;
        return 0;
    }

    for (int i = 0; i < num_processes; i++) {
        string name;
        int arrival_time;
        int priority;
        int burst_time;

        cout << "Enter process name: ";
        cin >> name;

        cout << "Enter arrival time: ";
        cin >> arrival_time;

        cout << "Enter priority: ";
        cin >> priority;

        cout << "Enter burst time: ";
        cin >> burst_time;

        Process p(name, arrival_time, priority, burst_time);

        processes.push_back(p);
    }
    string choice;
        cout << "Which algorithm do you want to use? fcfs --> first come first served, p --> priority or sjn --> shortest job next" << endl;
        cin >> choice;
        if(choice == "fcfs") fcfs(processes);
        else if(choice == "p") priorityy(processes);
        else if(choice == "sjn") sjn(processes);
        else cout << "Wrong selection!" << endl;
        
    return 0;
}

// FCFS Algorithm
void fcfs(vector<Process>& processes) {
    int current_time = 0;

    for (int i = 0; i < processes.size(); i++) {
        // set start time
        processes[i].start_time = max(current_time, processes[i].arrival_time);
        // set completion time
        processes[i].completion_time = processes[i].start_time + processes[i].burst_time;
        // update current time
        current_time = processes[i].completion_time;
        // set turnaround time
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
    }

    // calculate average turnaround time
    float avg_tat = 0;
    for (int i = 0; i < processes.size(); i++) {
        avg_tat += processes[i].turnaround_time;
    }
    avg_tat /= processes.size();

    // print results
    cout << "FCFS Algorithm Results:\n";
    cout << "------------------------\n";
    cout << "Order of execution: ";
    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].name << " ";
    }
    cout << "\n";
    for (int i = 0; i < processes.size(); i++) {
        cout << "Process " << processes[i].name << ":\n";
        cout << "Start Time: " << processes[i].start_time << "\n";
        cout << "Completion Time: " << processes[i].completion_time << "\n";
        cout << "Turnaround Time: " << processes[i].turnaround_time << "\n\n";
    }
    cout << "Average Turnaround Time: " << avg_tat << "\n";
}

// Priority Algorithm
void priorityy(vector<Process>& processes) {
    int current_time = 0;
    bool all_done = false;

    while (!all_done) {
        // get highest priority process that has arrived
        int highest_priority = INT_MAX;
        int idx = -1;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= current_time && processes[i].priority < highest_priority && processes[i].burst_time > 0) {
                highest_priority = processes[i].priority;
                idx = i;
            }
        }

        // if no process can be executed, move time forward
        if (idx == -1) {
            current_time++;
            continue;
        }

        // set start time
        processes[idx].start_time = current_time;
        // execute process for one time unit
        processes[idx].burst_time--;
        // update current time
        current_time++;
        // if process is done, set completion time and turnaround time
        if (processes[idx].burst_time == 0) {
            processes[idx].completion_time = current_time;
            processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
        }

        // check if all processes are done
        all_done = true;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].burst_time > 0) {
                all_done = false;
                break;
            }
        }
    }

    // calculate average turnaround time
    float avg_tat = 0;
    for (int i = 0; i < processes.size(); i++) {
        avg_tat += processes[i].turnaround_time;
    }
    avg_tat /= processes.size();
    
    // output results
    cout << "Priority Algorithm:\n";
    cout << "Process Execution Order: ";
    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].name << " ";
    }
    cout << "\n";
    
    cout << "Process Details:\n";
    for (int i = 0; i < processes.size(); i++) {
        cout << "Process Name: " << processes[i].name << "\n";
        cout << "Arrival Time: " << processes[i].arrival_time << "\n";
        cout << "Priority: " << processes[i].priority << "\n";
        cout << "Burst Time: " << processes[i].burst_time << "\n";
        cout << "Start Time: " << processes[i].start_time << "\n";
        cout << "Completion Time: " << processes[i].completion_time << "\n";
        cout << "Turnaround Time: " << processes[i].turnaround_time << "\n";
        cout << "\n";
    }
    
    cout << "Average Turnaround Time: " << avg_tat << "\n";
}

void sjn(vector<Process>& processes) {
    int current_time = 0;
    bool all_done = false;

    while (!all_done) {
        // get next process with shortest burst time
        int shortest_burst_time = INT_MAX;
        int idx = -1;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival_time <= current_time && processes[i].burst_time < shortest_burst_time && processes[i].burst_time > 0) {
                shortest_burst_time = processes[i].burst_time;
                idx = i;
            }
        }

        // if no process can be executed, move time forward
        if (idx == -1) {
            current_time++;
            continue;
        }

        // set start time
        processes[idx].start_time = current_time;
        // execute process for its full burst time
        current_time += processes[idx].burst_time;
        processes[idx].burst_time = 0;
        // set completion time and turnaround time
        processes[idx].completion_time = current_time;
        processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;

        // check if all processes are done
        all_done = true;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].burst_time > 0) {
                all_done = false;
                break;
            }
        }
    }

    // calculate average turnaround time
    float avg_tat = 0;
    for (int i = 0; i < processes.size(); i++) {
        avg_tat += processes[i].turnaround_time;
    }
    avg_tat /= processes.size();

    // output results
    cout << "Shortest Job Next Algorithm:\n";
    cout << "Process Execution Order: ";
    for (int i = 0; i < processes.size(); i++) {
        cout << processes[i].name << " ";
    }
    cout << "\n";

    cout << "Process Details:\n";
    for (int i = 0; i < processes.size(); i++) {
        cout << "Process Name: " << processes[i].name << "\n";
        cout << "Arrival Time: " << processes[i].arrival_time << "\n";
        cout << "Burst Time: " << processes[i].burst_time << "\n";
        cout << "Start Time: " << processes[i].start_time << "\n";
        cout << "Completion Time: " << processes[i].completion_time << "\n";
        cout << "Turnaround Time: " << processes[i].turnaround_time << "\n";
        cout << "\n";
    }

    cout << "Average Turnaround Time: " << avg_tat << "\n";
}



