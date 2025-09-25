#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int process;
    int burst_time;
    int arrival_time;
    int start_time;
    int finish_time;
    int tat; // Turnaround Time
    int wt;  // Waiting Time
    int rt;  // Response Time
    struct node *next;
} Node;

Node *start = NULL;

// Insert process in arrival time order
void push(int p, int bt, int at) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->process = p;
    newNode->burst_time = bt;
    newNode->arrival_time = at;
    newNode->next = NULL;

    if (start == NULL || at < start->arrival_time) {
        newNode->next = start;
        start = newNode;
        return;
    }

    Node *ptr = start;
    while (ptr->next != NULL && ptr->next->arrival_time <= at) {
        ptr = ptr->next;
    }
    newNode->next = ptr->next;
    ptr->next = newNode;
}

// Calculate start, finish, TAT, WT, RT
void calculateTimes() {
    Node *ptr = start;
    int current_time = 0;

    while (ptr != NULL) {
        if (ptr->arrival_time > current_time)
            current_time = ptr->arrival_time; // CPU idle until process arrives

        ptr->start_time = current_time;
        ptr->finish_time = current_time + ptr->burst_time;

        ptr->tat = ptr->finish_time - ptr->arrival_time;
        ptr->wt = ptr->tat - ptr->burst_time;
        ptr->rt = ptr->start_time - ptr->arrival_time;

        current_time = ptr->finish_time;
        ptr = ptr->next;
    }
}

// Display table with all metrics
void displayTable(int n) {
    Node *ptr = start;
    float total_tat = 0, total_wt = 0, total_rt = 0;

    printf("\nProcess Execution Table:\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("| Process | Arrival | Burst | Start | Finish |   TAT   |   WT   |   RT   |\n");
    printf("-------------------------------------------------------------------------------\n");

    while (ptr != NULL) {
        printf("|   P%-4d |   %-6d| %-5d| %-5d| %-6d| %-7d| %-7d| %-7d|\n",
               ptr->process, ptr->arrival_time, ptr->burst_time,
               ptr->start_time, ptr->finish_time,
               ptr->tat, ptr->wt, ptr->rt);

        total_tat += ptr->tat;
        total_wt += ptr->wt;
        total_rt += ptr->rt;

        ptr = ptr->next;
    }
    printf("-------------------------------------------------------------------------------\n");

    printf("\nAverage Turnaround Time = %.2f\n", total_tat / n);
    printf("Average Waiting Time    = %.2f\n", total_wt / n);
    printf("Average Response Time   = %.2f\n", total_rt / n);
}

// Display Gantt chart with indexes and idle times
void displayGantt() {
    Node *ptr = start;
    int current_time = 0;

    printf("\nGantt Chart:\n");
    printf("-------------------------------------------------\n");

    // Print processes including IDLE
    ptr = start;
    while (ptr != NULL) {
        if (ptr->start_time > current_time) {
            printf("| IDLE ");
            current_time = ptr->start_time;
        }
        printf("| P%d  ", ptr->process);
        current_time = ptr->finish_time;
        ptr = ptr->next;
    }
    printf("|\n");
    printf("-------------------------------------------------\n");

    // Print timeline indexes
    ptr = start;
    current_time = 0;
    printf("%-5d", current_time);
    while (ptr != NULL) {
        if (ptr->start_time > current_time) {
            printf("%-6d", ptr->start_time);
        }
        printf("%-6d", ptr->finish_time);
        current_time = ptr->finish_time;
        ptr = ptr->next;
    }
    printf("\n");
}

// Create linked list from input arrays
void create(int p, int bt[], int at[]) {
    for (int i = 0; i < p; i++) {
        push(i + 1, bt[i], at[i]);
    }
}

int main() {
    printf("\nHey User! Welcome to FCFS Scheduler\n");

    int p;
    printf("\nEnter number of processes: ");
    scanf("%d", &p);

    int bt[p], at[p];
    printf("Enter burst time and arrival time for each process:\n");
    for (int i = 0; i < p; i++) {
        printf("P%d -> Burst time & Arrival time: ", i + 1);
        scanf("%d %d", &bt[i], &at[i]);
    }

    printf("\nProcess Table (Input):\n");
    printf("------------------------------------------------\n");
    printf("| Process | Burst time | Arrival time |\n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < p; i++)
        printf("|  P%-6d|     %-7d|      %-7d|\n", i + 1, bt[i], at[i]);
    printf("------------------------------------------------\n");

    create(p, bt, at);
    calculateTimes();
    displayTable(p);
    displayGantt();

    return 0;
}
