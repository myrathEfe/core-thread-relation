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
    // Şimdilik least-loaded gibi davransın
    return assignLeastLoaded(cores, k, t);
}

int assignAffinity(Core *cores, int k, Thread t, int *lastCore) {
    // Thread daha önce hiç atanmamışsa 0’a yolla
    if (lastCore[t.id] < 0 || lastCore[t.id] >= k)
        return 0;
    return lastCore[t.id];
}

void balanceWorkStealing(Core *cores, int k) {
    // Şimdilik hiçbir şey yapmasın; sonra doldurursun
}
