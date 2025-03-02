#include <stdio.h>
#include <time.h>
#include <stdbool.h>

int main() {
    struct timespec sleep_time;
    sleep_time.tv_sec = 1;  // Seconds
    sleep_time.tv_nsec = 0;  // Nanoseconds
    
    while (true) {
        // Do whatever work you need to do here
        // printf("Working...\n");
        
        // Sleep properly, consuming virtually no CPU
        // nanosleep(&sleep_time, NULL);
    }
    
    return 0;
}