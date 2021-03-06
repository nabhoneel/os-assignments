/*
Non pre-epmtive version of shortest job first scheduling algorithm
*/

#include <stdio.h>
#include <stdlib.h>

int TIMESTAMP = 1, PROCESS_ID = 0;

// The following will be a queue of a large size:
int *queue, front=0, rear=-1;

void enqueue(int x) {
  queue[++rear] = x;
}

int dequeue() {
  int x;
  if(front <= rear) {
    x = queue[front++];
  } else {
    x = -1;
  }
  return x;
}

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

  int *remaining_time = (int*) malloc(sizeof(int) * n);
  int process_remains;
  int time = 0;

  int time_quantum = 3; 
  int k = time_quantum;

  for(i=0; i<n; i++) {
    remaining_time[i] = bt[i];
  }

  while(1) {
    process_remains = 0;
    for(i=0; i<n; i++) {
      if(remaining_time[i] > 0) {
        process_remains = 1;
        if(remaining_time[i] > time_quantum) {
          k = time_quantum;
          remaining_time[i] -= time_quantum;
        } else {
          k = remaining_time[i];
          wt[i] = time - bt[i];
          remaining_time[i] = 0;
        }
        time += k;
      }
    }
    if(process_remains == 0) {
      break;
    }
  }

  for(i=0; i<n; i++) {
    tat[i] = bt[i] + wt[i];
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

  printf("Round Robin Scheduling\n\nEnter number of processes in queue: ");
  scanf("%d", &n);

  arrival_time = (int*)malloc(sizeof(int) * n);
  burst_time = (int*)malloc(sizeof(int) * n);
  waiting_time = (int*)malloc(sizeof(int) * n);
  turn_around_time = (int*)malloc(sizeof(int) * n);
  process_id = (int*)malloc(sizeof(int) * n);

  for(i=0; i<n; i++) {
    process_id[i] = i;
  }

  queue = (int*)malloc(sizeof(int) * 1000);
  // (int*)malloc(sizeof((int)avg(arrival_time, n) * n));
  for(i=0; i<1000; i++)
    queue[i] = -1;

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