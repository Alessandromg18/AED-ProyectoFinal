#include <iostream>

#include "database/Database.h"

#include "benchmark/Benchmark.h"

int main() {
    Benchmark::run(
        "../data/movies_dataset.csv"
    );
    return 0;
}