#include <math.h>
#include "analysis.h"

double loadVariance(Core *cores, int k) {
    double sum = 0;
    for (int i = 0; i < k; i++)
        sum += cores[i].load;

    double mean = sum / k;

    double variance = 0;
    for (int i = 0; i < k; i++)
        variance += pow(cores[i].load - mean, 2);

    return variance / k;
}
