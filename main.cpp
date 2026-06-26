#include "benchmark/Benchmark.h"

int main() {
    Benchmark::run(
        "../data/dataset_100000.csv"
    );
    return 0;
}
