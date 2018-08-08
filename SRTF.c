/*
Non pre-epmtive version of shortest job first scheduling algorithm
*/

#include <stdio.h>
#include <stdlib.h>

void get_input(int *x, int l) {
  int i=0;
  for(; i<l; i++) {
    printf("Process #%d: ", (i+1));
    scanf("%d", &x[i]);
  }
}

// sort the burst times according to the arrival times:
void sort_by_arrival_time(int *pid, int *at, int *bt, int n) {
  int i, j;
  for(i=0; i<n; i++) {
    for(j=0; j<n - i - 1; j++) {
      if(at[j] > at[j+1]) {
        at[j] = at[j] + at[j+1];
        at[j+1] = at[j] - at[j+1];
        at[j] = at[j] - at[j+1];

        bt[j] = bt[j] + bt[j+1];
        bt[j+1] = bt[j] - bt[j+1];
        bt[j] = bt[j] - bt[j+1];

        pid[j] = pid[j] + pid[j+1];
        pid[j+1] = pid[j] - pid[j+1];
        pid[j] = pid[j] - pid[j+1];
      }
    }
  }
}

// calculate turn around time and waiting time:
void calc_times(
  int *pid,
  int *at, 
  int *bt, 
  int *tat, 
  int *wt, 
  int n
) {
  int i, j;

  int *completion_time = (int*)calloc(n, sizeof(int));
  int *remaining_time = (int*) malloc(sizeof(int) * n);
  int chosen_process;
  int process_remains;

  for(i=0; i<n; i++) {
    remaining_time[i] = bt[i];
  }

  for(i=0; ; i++) {
    chosen_process = 0;
    process_remains = 0;

    // Find the process with the minimum remaining burst time:
    for(j=0; j<n; j++) {
      if(remaining_time[j] > 0) {
        if(process_remains == 0) {
          // Choosing the first process (among the ones which remain to be computed):
          chosen_process = j;
          process_remains = 1;
        }
        if(at[j] <= i && remaining_time[chosen_process] > remaining_time[j]) {
          chosen_process = j;
        }
      }
    }

    // All processes have been computed:
    if(process_remains == 0) break;

    // Reduce remaining time for the chosen process:
    remaining_time[chosen_process]--;

    // If the chosen process has been computed, log its completion time:
    if(remaining_time[chosen_process] == 0) {
      completion_time[chosen_process] = i+1;
    }
  }

  // Calculate other times:
  for(i=0; i<n; i++) {
    tat[i] = completion_time[i] - at[i];
    wt[i] = tat[i] - bt[i];
  }
}

float avg(int *arr, int n) {
  int i;
  float sum = 0;
  for(i=0; i<n; i++) {
    sum += arr[i];
  }
  return (sum/(float)n);
}

int main() {
  int *arrival_time, *burst_time, *waiting_time, *turn_around_time, *process_id;
  int i, n;

  printf("SHORTEST REMAINING TIME FIRST (SRTF)\n\nEnter number of processes in queue: ");
  scanf("%d", &n);

  arrival_time = (int*)malloc(sizeof(int) * n);
  burst_time = (int*)malloc(sizeof(int) * n);
  waiting_time = (int*)malloc(sizeof(int) * n);
  turn_around_time = (int*)malloc(sizeof(int) * n);
  process_id = (int*)malloc(sizeof(int) * n);

  for(i=0; i<n; i++) {
    process_id[i] = i;
  }

  printf("Enter the arrival times:\n");
  get_input(arrival_time, n);
  printf("Enter the burst times:\n");
  get_input(burst_time, n);

  // First, sort by arrival times of processes:
  sort_by_arrival_time(process_id, arrival_time, burst_time, n);
  // Calculate other parameters:
  calc_times(
    process_id,
    arrival_time, 
    burst_time, 
    turn_around_time, 
    waiting_time, 
    n
  );
  
  printf("\nProcess\t\tArrival time\tBurst time\tTurn-around time\tWaiting time\n");

  for(i=0; i<n; i++) {
    printf(
      "P%d\t\t%d\t\t%d\t\t%d\t\t\t%d\n", 
      process_id[i], 
      arrival_time[i], 
      burst_time[i], 
      turn_around_time[i], 
      waiting_time[i]
    );
  }

  printf(
    "Average turn-around time: %g\nAverage waiting time: %g", 
    avg(turn_around_time, n), 
    avg(waiting_time, n)
  );

  return -1;
}