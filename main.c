#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "thread.h"
#include "scheduler.h"
#include "data_generator.h"
#include "analysis.h"

#define K 8
#define N 500

const char* algoName(int algo) {
    switch (algo) {
        case 1: return "Round-Robin";
        case 2: return "Least-Loaded";
        case 3: return "Priority-Based";
        case 4: return "Affinity-Based";
        case 5: return "Least-Loaded + Work-Stealing";
        default: return "Unknown";
    }
}

void printResults(Core *cores, int k, int algo) {
    int totalLoad = 0;
    int maxLoad = cores[0].load;
    int minLoad = cores[0].load;

    for (int i = 0; i < k; i++) {
        totalLoad += cores[i].load;
        if (cores[i].load > maxLoad) maxLoad = cores[i].load;
        if (cores[i].load < minLoad) minLoad = cores[i].load;
    }

    double avgLoad = (double)totalLoad / k;
    double var = loadVariance(cores, k);

    printf("\n================= SONUÇLAR =================\n");
    printf(" Algoritma      : %s\n", algoName(algo));
    printf(" Çekirdek sayısı: %d\n", k);
    printf(" Thread sayısı  : %d\n", N);
    printf("--------------------------------------------\n");

    for (int i = 0; i < k; i++) {
        printf(" Core %d -> Load: %4d  | Tasks: %3d\n",
               cores[i].id, cores[i].load, cores[i].taskCount);
    }

    printf("--------------------------------------------\n");
    printf(" Toplam yük         : %d\n", totalLoad);
    printf(" Ortalama yük       : %.2f\n", avgLoad);
    printf(" Min / Max yük      : %d / %d\n", minLoad, maxLoad);
    printf(" Yük varyansı       : %.2f\n", var);
    printf("============================================\n\n");
}

int main() {

    while (1) {

        int algo;
        printf("\nAlgoritma Seç:\n");
        printf(" 1 = Round-Robin\n");
        printf(" 2 = Least-Loaded\n");
        printf(" 3 = Priority-Based\n");
        printf(" 4 = Affinity-Based\n");
        printf(" 5 = Work-Stealing\n");
        printf(" 0 = Çıkış\n");
        printf("Seçiminiz: ");
        scanf("%d", &algo);

        if (algo == 0) {
            printf("Programdan çıkılıyor...\n");
            break;
        }

        // yapılar
        Core cores[K];
        Thread threads[N];
        int rrIndex = 0;
        int lastCore[N];

        // lastCore başlangıcı
        for (int i = 0; i < N; i++) lastCore[i] =  -1;

        // veri üretme ve core başlangıcı
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

                default:
                    printf("Geçersiz seçim, Least-Loaded kullanıldı.\n");
                    target = assignLeastLoaded(cores, K, threads[i]);
                    break;
            }

            cores[target].load += threads[i].burst;
            cores[target].taskCount++;
            lastCore[threads[i].id] = target;
        }

        // Work Stealing
        if (algo == 5) {
            balanceWorkStealing(cores, K);
        }

        // sonuçlar
        printResults(cores, K, algo);
    }

    return 0;
}
