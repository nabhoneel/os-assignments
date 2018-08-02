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

void sort_by_burst_time(int *pid, int *at, int *bt, int start_process, int end_process) {
  int i, j;
  for(i=start_process; i<=end_process; i++) {
    for(j=start_process; j<=end_process - i - 1; j++) {
      if(bt[j] > bt[j+1]) {
        bt[j] = bt[j] + bt[j+1];
        bt[j+1] = bt[j] - bt[j+1];
        bt[j] = bt[j] - bt[j+1];

        at[j] = at[j] + at[j+1];
        at[j+1] = at[j] - at[j+1];
        at[j] = at[j] - at[j+1];

        pid[j] = pid[j] + pid[j+1];
        pid[j+1] = pid[j] - pid[j+1];
        pid[j] = pid[j] - pid[j+1];
      }
    }
  }
}

int find_latest_process_available(int *arrival_time, int n, int time_elapsed, int current_process) {
  int i;
  for(i=current_process; i<n; i++) {
    if(arrival_time[i] > time_elapsed) {
      return i-1;
    }
  }
  return n-1;
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
  int i;
  int current_process = 1;
  int latest_process_available;

  int time_elapsed = bt[0];
  tat[0] = time_elapsed - at[0];
  wt[0] = tat[0] - bt[0];

  while(1) {
    latest_process_available = find_latest_process_available(at, n, time_elapsed, current_process);
    sort_by_burst_time(pid, at, bt, current_process, latest_process_available);

    for(i=current_process; i<=latest_process_available; i++) {
      time_elapsed += bt[i];
      tat[i] = time_elapsed - at[i];
      wt[i] = tat[i] - bt[i];

      if(at[i] <= time_elapsed) {
        i++;
        break;
      }
    }
    current_process = i;

    if(current_process == n) break;
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

  printf("Enter number of processes in queue: ");
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