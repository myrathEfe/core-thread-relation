#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_THREADS 500  // başlangıç test ölçeği
#define MAX_CORES 8      // gerçekçi ama yönetilebilir core sayısı

typedef struct {
    int id;
    int load;       // ms cinsinden toplam yük (burst time toplamı)
    int taskCount;  // kaç thread atandı (istatistik için)
} Core;

typedef struct {
    int id;
    int burst;  // thread’in işlem süresi (ms)
} Thread;

int main(int argc, char *argv[]) {
    int k = MAX_CORES;
    int n = MAX_THREADS;

    // Algoritma seçimini al
    int algoChoice;
    if (argc > 1) {
        algoChoice = atoi(argv[1]);
    } else {
        printf("Algoritma Seç (1=RR,2=LeastLoaded,3=Priority,4=Affinity,5=WorkStealing): ");
        scanf("%d", &algoChoice);
    }

    // Core dizisini başlat
    Core cores[MAX_CORES];
    for (int i = 0; i < k; i++) {
        cores[i].id = i;
        cores[i].load = 0;
        cores[i].taskCount = 0;
    }

    // Thread dizisini random burst süreleriyle oluştur
    Thread threads[MAX_THREADS];
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        threads[i].id = i;
        threads[i].burst = (rand() % 20) + 1; // 1–20 ms burst -> gerçekçi RT yükü
    }

    // Simülasyon: Thread’leri seçilen algoritmaya göre core’lara ata
    int rrIndex = 0;  // Round-Robin için sıradaki core
    for (int i = 0; i < n; i++) {
        int targetCore = 0;

        switch (algoChoice) {
            case 1: // Round-Robin (henüz basit ata)
                targetCore = rrIndex;
                rrIndex = (rrIndex + 1) % k;
                break;
            case 2: // Least-Loaded (şimdilik en az loaded core’u seç)
            case 3: // Priority (iskelet için least-loaded gibi seç, sonra değiştirirsiniz)
            case 4: // Affinity (iskelet için 0 ata, sonra değiştirirsiniz)
            case 5: // Work-Stealing (önce least-loaded atama yap, steal’i sonra ekleyeceğiz)
            default:
                // Şimdilik core seçiminde en az yüklü core’u bul
                for (int j = 1; j < k; j++) {
                    if (cores[j].load < cores[targetCore].load)
                        targetCore = j;
                }
                targetCore = targetCore;
                break;
        }

        cores[targetCore].load += threads[i].burst;
        cores[targetCore].taskCount++;
    }

    // Sonuç: Core yüklerini yazdır (baseline kontrol)
    printf("\n--- Core Load Results ---\n");
    for (int i = 0; i < k; i++) {
        printf("Core %d → Total Load: %d ms, Tasks: %d\n",
               cores[i].id, cores[i].load, cores[i].taskCount);
    }

    return 0;
}
