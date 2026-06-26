//
// Created by facum on 9/06/2026.
//

#include "Benchmark.h"

#include "../database/Database.h"

#include <chrono>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace chrono;

void Benchmark::run(
    const string& csvFile
)
{

    cout
        << "\n==============================\n";

    cout
        << "BENCHMARK\n";

    cout
        << "==============================\n\n";



    auto start =
        high_resolution_clock::now();

    Database db;

    db.loadCSV(csvFile);

    auto end =
        high_resolution_clock::now();

    cout
        << "Carga CSV: "
        << duration_cast<milliseconds>(
               end - start
           ).count()
        << " ms\n\n";

    auto records =
        db.getAllRecords();

    int ITERATIONS = min(1000, (int)records.size());

    cout
        << "Registros: "
        << records.size()
        << "\n\n";
    start =
    high_resolution_clock::now();

    for(int i = 0; i < ITERATIONS; i++)
    {
        const auto& r =
            records[i % records.size()];

        db.findById(r.id);
    }

    end =
        high_resolution_clock::now();

    cout
        << "Busqueda por ID (Hash): "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";
    long long linearVisited = 0;

    start =
        high_resolution_clock::now();

    for(int i = 0; i < ITERATIONS; i++)
    {
        const auto& target =
            records[i % records.size()];

        for(const auto& r : records)
        {
            linearVisited++;

            if(r.id == target.id)
                break;
        }
    }

    end =
        high_resolution_clock::now();

    cout
        << "Busqueda por ID (Lineal): "
        << duration_cast<milliseconds>(
               end - start
           ).count()
        << " ms\n";

    cout
        << "Registros revisados: "
        << linearVisited
        << "\n\n";
    double score =
    records[
        records.size() / 2
    ].score;


    db.getScoreIndex()
.resetVisited();

    start =
        high_resolution_clock::now();

    db.getScoreIndex().search(score);


    end =
        high_resolution_clock::now();

    cout
        << "Equality Query (RBT): "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    cout
        << "Nodos visitados: "
        << db.getScoreIndex()
               .getVisited()
        << "\n";
    linearVisited = 0;

    start =
        high_resolution_clock::now();

    for(const auto& r : records)
    {
        linearVisited++;

        if(r.score == score)
        {
        }
    }

    end =
        high_resolution_clock::now();

    cout
        << "Equality Query (Lineal): "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    cout
        << "Registros revisados: "
        << linearVisited
        << "\n\n";
    db.getScoreIndex()
    .resetVisited();




    auto [minIt, maxIt] = minmax_element(
    records.begin(),
    records.end(),
    [](const auto& a, const auto& b)
    {
        return a.score < b.score;
    }
);

    double minScore = minIt->score;
    double maxScore = maxIt->score;

    double low =
        minScore +
        (maxScore - minScore) * 0.25;

    double high =
        minScore +
        (maxScore - minScore) * 0.75;

    start =
        high_resolution_clock::now();

    auto result = db.findBetween(low, high);

    end =
        high_resolution_clock::now();

    cout
        << "Range Query (RBT): "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    std::cout
        << "Nodos visitados: "
        << db.getScoreIndex()
               .getVisited()
        << "\n";
    linearVisited = 0;

    start =
        high_resolution_clock::now();

    vector<Record> linearResult;

    for(const auto& r : records)
    {
        linearVisited++;

        if(r.score >= low && r.score <= high)
        {
            linearResult.push_back(r);
        }
    }

    end =
        high_resolution_clock::now();

    cout
        << "Range Query (Lineal): "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    cout
        << "Registros revisados: "
        << linearVisited
        << "\n\n";

    db.getScoreIndex()
    .resetVisited();

    start =
    high_resolution_clock::now();

    double median =
        db.medianScore();

    end =
        high_resolution_clock::now();

    cout
        << "Mediana (RBT): "
        << median
        << " | "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";
    vector<double> scores;

    cout
        << "Nodos visitados: "
        << db.getScoreIndex()
            .getVisited()
        << "\n";


    scores.reserve(
        records.size()
    );

    start =
        high_resolution_clock::now();

    for(const auto& r : records)
    {
        scores.push_back(
            r.score
        );
    }

    sort(
        scores.begin(),
        scores.end()
    );

    double medianLinear;

    int n =
        scores.size();

    if(n % 2 == 1)
    {
        medianLinear =
            scores[n/2];
    }
    else
    {
        medianLinear =
            (
                scores[n/2]
                +
                scores[n/2 - 1]
            ) / 2.0;
    }

    end =
        high_resolution_clock::now();

    cout
        << "Mediana (Lineal): "
        << medianLinear
        << " | "
        << duration_cast<milliseconds>(
               end - start
           ).count()
        << " ms\n";

    db.getScoreIndex()
    .resetVisited();

    start =
        high_resolution_clock::now();

    double p90 =
        db.percentileScore(90);

    end =
        high_resolution_clock::now();

    cout
        << "Percentil 90 (RBT): "
        << p90
        << " | "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    cout
        << "Nodos visitados: "
        << db.getScoreIndex()
               .getVisited()
        << "\n";

    vector<double> temp = scores;

    start = high_resolution_clock::now();

    sort(temp.begin(), temp.end());

    int pos = (90 * (temp.size() - 1)) / 100;
    double p90Linear = temp[pos];

    end = high_resolution_clock::now();

    cout
        << "Percentil 90 (Lineal): "
        << p90Linear
        << " | "
        << duration_cast<milliseconds>(
               end - start
           ).count()
        << " ms\n\n";

    db.getScoreIndex()
    .resetVisited();

    start =
        high_resolution_clock::now();

    int countIndex =
        db.getScoreIndex()
            .countRange(
                low,
                high
            );

    end =
        high_resolution_clock::now();

    cout
        << "Count Range (RBT): "
        << countIndex
        << " | "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n";

    cout
        << "Nodos visitados: "
        << db.getScoreIndex()
               .getVisited()
        << "\n";

    int countLinear = 0;

    start =
        high_resolution_clock::now();

    for(const auto& r : records)
    {
        if(
            r.score >= low &&
            r.score <= high
        )
        {
            countLinear++;
        }
    }

    end =
        high_resolution_clock::now();

    cout
        << "Count Range (Lineal): "
        << countLinear
        << " | "
        << duration_cast<microseconds>(
               end - start
           ).count()
        << " us\n\n";


}
