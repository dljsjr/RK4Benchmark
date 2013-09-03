#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <mach/mach_time.h>

#define ITERATIONS 10000000
#define BENCHMARK_ITERATIONS 100

#define NANOSECONDS (+1.0E-9)
#define GIGA UINT64_C(1000000000)

static double timebase = 0.0;
static uint64_t timestart = 0;

struct timespec monotonicTime(void) {
  // be more careful in a multithreaded environement
  if (!timestart) {
    mach_timebase_info_data_t tb = { 0 };
    mach_timebase_info(&tb);
    timebase = tb.numer;
    timebase /= tb.denom;
    timestart = mach_absolute_time();
  }
  struct timespec t;
  double diff = (mach_absolute_time() - timestart) * timebase;
  t.tv_sec = diff * NANOSECONDS;
  t.tv_nsec = diff - (t.tv_sec * GIGA);
  return t;
}

void subTime(struct timespec *a, struct timespec *b)
{
  a->tv_nsec = a->tv_nsec - b->tv_nsec;
  if (a->tv_nsec < 0) {
    // borrow.
    a->tv_nsec += 1000000000;
    a->tv_sec --;
  }

  a->tv_sec = a->tv_sec - b->tv_sec;
}

long long timespecToNanos(struct timespec ts)
{
  return (ts.tv_sec / NANOSECONDS + ts.tv_nsec);
}

double rk4(double f(double t, double y), double h, double tn, double yn)
{
  double k1, k2, k3, k4;

  k1 = f(tn, yn);
  k2 = f((tn + h/2), yn + (h/2 * k1));
  k3 = f((tn + h/2), yn + (h/2 * k2));
  k4 = f(tn + h, yn + h * k3);

  return yn + (h/6) * (k1 + 2*k2 + 2*k3 + k4);
}

double expDot(double t, double y)
{
  return -y;
}

int main()
{
  int i, j;
  double t0, y0, tn, yn, h;
  double* results = malloc(sizeof(double) * ITERATIONS);
  long long min, max, mean, stddev, sum, sumSquares;
  long long* times = malloc(sizeof(long long) * BENCHMARK_ITERATIONS);
  struct timespec start, end;

  h = 1e-3;
  y0 = 1.0;
  t0 = 0.0;
  yn = y0;

  printf("\nBeginning C Benchmark...\n");

  for(i = 0; i < BENCHMARK_ITERATIONS; i++)
  {
    start = monotonicTime();

    for(j = 0; j < ITERATIONS; j++)
    {
      results[j] = yn;

      yn = rk4(expDot, h, tn, yn);

      tn += h;
    }

    end = monotonicTime();

    subTime(&end, &start);

    times[i] = timespecToNanos(end);
  }

  max = mean = stddev = sum = sumSquares = 0;
  min = times[0];

  for(i = 0; i < BENCHMARK_ITERATIONS; i++)
  {
    if(times[i] > max)
      max = times[i];
    
    if(times[i] < min)
      min = times[i];

    sum += times[i];
  }

  mean = sum / BENCHMARK_ITERATIONS;

  for(i = 0; i < BENCHMARK_ITERATIONS; i++)
  {
    sumSquares += pow((times[i] - mean), 2);
  }

  stddev = sqrt(sumSquares / BENCHMARK_ITERATIONS);

  printf("Mean: %Lf s\n Min: %Lf s\n Max: %Lf s\n StdDev: %Lf s\n",
      (long double) mean * 1e-9,
      (long double) min * 1e-9,
      (long double) max * 1e-9,
      (long double) stddev * 1e-9);

  return 0;
}
