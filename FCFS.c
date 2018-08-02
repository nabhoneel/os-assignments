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
void sort(int *at, int *bt, int n) {
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
      }
    }
  }
}

// calculate turn around time and waiting time:
void calc_times(
  int *at, 
  int *bt, 
  int *tat, 
  int *wt, 
  int n
) {
  int i = 0, completion_time = 0;
  for(; i<n; i++) {
    completion_time += bt[i];
    tat[i] = completion_time - at[i];
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
  int *arrival_time, *burst_time, *waiting_time, *turn_around_time;
  int i, n;

  printf("Enter number of processes in queue: ");
  scanf("%d", &n);

  arrival_time = (int*)malloc(sizeof(int) * n);
  burst_time = (int*)malloc(sizeof(int) * n);
  waiting_time = (int*)malloc(sizeof(int) * n);
  turn_around_time = (int*)malloc(sizeof(int) * n);

  printf("Enter the arrival times:\n");
  get_input(arrival_time, n);
  printf("Enter the burst times:\n");
  get_input(burst_time, n);

  sort(arrival_time, burst_time, n);
  calc_times(
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
      (i+1), 
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