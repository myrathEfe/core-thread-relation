#include "scheduler.h"

int assignRR(Core *cores, int k, Thread t, int *rrIndex) {
    int chosen = *rrIndex;
    *rrIndex = (*rrIndex + 1) % k;
    return chosen;
}

int assignLeastLoaded(Core *cores, int k, Thread t) {
    int min = 0;
    for (int i = 1; i < k; i++)
        if (cores[i].load < cores[min].load)
            min = i;
    return min;
}

int assignPriority(Core *cores, int k, Thread t) {
    // Şimdilik least-loaded gibi, sonra gerçek öncelik mantığını ekleyebilirsiniz.
    return assignLeastLoaded(cores, k, t);
}

int assignAffinity(Core *cores, int k, Thread t, int *lastCore) {
    // Aynı thread aynı core'a gitmeye çalışır
    return lastCore[t.id];
}

void balanceWorkStealing(Core *cores, int k) {
    // Work stealing iskeleti; sonra dolduracaksınız
    // örnek: en yüksek yükten en düşük yüke burst aktarma mantığı
}
