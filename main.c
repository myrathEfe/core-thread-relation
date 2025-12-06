#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "thread.h"
#include "scheduler.h"
#include "data_generator.h"
#include "analysis.h"

#define K 8
#define N 500

int main(int argc, char *argv[]) {

    int algo;
    if (argc > 1) algo = atoi(argv[1]);
    else {
        printf("Algoritma Seç (1=RR,2=LL,3=Priority,4=Affinity,5=Steal): ");
        scanf("%d", &algo);
    }

    Core cores[K];
    Thread threads[N];
    int rrIndex = 0;        // RR için güncel core pointer’ı
    int lastCore[N];        // affinity için

    // init
    initCores(cores, K);
    generateThreads(threads, N);

    // simülasyon
    for (int i = 0; i < N; i++) {
        int target = 0;

        switch (algo) {
            case 1:
                target = assignRR(cores, K, threads[i], &rrIndex);
            break;

            case 2:
                target = assignLeastLoaded(cores, K, threads[i]);
            break;

            case 3:
                target = assignPriority(cores, K, threads[i]);
            break;

            case 4:
                target = assignAffinity(cores, K, threads[i], lastCore);
            break;

            case 5:
                target = assignLeastLoaded(cores, K, threads[i]);
            break;
        }

        cores[target].load += threads[i].burst;
        cores[target].taskCount++;
        lastCore[threads[i].id] = target;
    }

    if (algo == 5)
        balanceWorkStealing(cores, K);

    printf("\n--- Core Loads ---\n");
    for (int i = 0; i < K; i++) {
        printf("Core %d: Load = %d, Tasks = %d\n",
               cores[i].id, cores[i].load, cores[i].taskCount);
    }

    printf("\nLoad Variance = %.2f\n", loadVariance(cores, K));

    return 0;
}
