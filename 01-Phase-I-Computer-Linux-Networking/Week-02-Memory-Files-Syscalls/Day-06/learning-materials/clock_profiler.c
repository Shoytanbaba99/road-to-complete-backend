#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

// Helper function to convert struct timespec to a double for easy math
double timespec_to_double(struct timespec *ts) {
    return (double)ts->tv_sec + ((double)ts->tv_nsec / 1e9);
}

int main() {
    struct timespec wall_start, wall_end;
    struct timespec mono_start, mono_end;

    printf("=== DUAL CLOCK INTERVAL MEASUREMENT ===\n");
    printf("Capturing initial timestamps...\n");

    // 1. Capture the start times from both clocks
    clock_gettime(CLOCK_REALTIME, &wall_start);
    clock_gettime(CLOCK_MONOTONIC, &mono_start);

    printf("Sleeping for exactly 5 seconds. \n");
    printf("--> QUICK! Open a second terminal and change the system time backward by 10 seconds:\n");
    printf("--> Command: sudo date -s \"10 seconds ago\"\n\n");

    // 2. Simulate blocking work (e.g., a database query or network wait)
    sleep(5);

    // 3. Capture the end times
    clock_gettime(CLOCK_REALTIME, &wall_end);
    clock_gettime(CLOCK_MONOTONIC, &mono_end);

    // 4. Calculate Durations
    double wall_duration = timespec_to_double(&wall_end) - timespec_to_double(&wall_start);
    double mono_duration = timespec_to_double(&mono_end) - timespec_to_double(&mono_start);

    printf("=== RESULTS ===\n");
    printf("Monotonic Clock Duration: %f seconds\n", mono_duration);
    printf("Wall Clock Duration:      %f seconds\n", wall_duration);

    if (wall_duration < 0) {
        printf("\nCRITICAL FAILURE: The Wall Clock reported a NEGATIVE duration!\n");
        printf("If this was a database timeout check (e.g., if (duration > 5.0)), it would fail forever.\n");
    }

    return 0;
}