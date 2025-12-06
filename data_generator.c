#include <stdlib.h>
#include <time.h>
#include "data_generator.h"

void generateThreads(Thread *threads, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        threads[i].id = i;
        threads[i].burst = (rand() % 20) + 1; // 1–20 ms
        threads[i].priority = rand() % 5;     // opsiyonel
    }
}
