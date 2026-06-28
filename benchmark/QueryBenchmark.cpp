//
// Created by facum on 27/06/2026.
//

#include "QueryBenchmark.h"
#include <chrono>

QueryResult QueryBenchmark::searchById(
    Database& db,
    int id
)
{
    QueryResult result;

    auto start = std::chrono::high_resolution_clock::now();

    Record* rbtRecord = db.findById(id);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    if(rbtRecord)
        result.records.push_back(*rbtRecord);

    result.visitedNodes =
        db.getScoreIndex().getVisitedNodes();

    result.visitedRBT =
        result.visitedNodes.size();

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords =
        db.linearFindById(id);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedLinear =
        db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::rangeSearch(
    Database& db,
    double min,
    double max
)
{
    QueryResult result;

    auto start = std::chrono::high_resolution_clock::now();

    auto rbtRecords =
        db.findBetween(min, max);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.records = rbtRecords;

    result.visitedNodes =
        db.getScoreIndex().getVisitedNodes();

    result.visitedRBT =
        result.visitedNodes.size();

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords =
        db.linearFindBetween(min, max);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedLinear =
        db.getAllRecords().size();

    return result;
}


QueryResult QueryBenchmark::topK(
    Database& db,
    int k
)
{
    QueryResult result;

    // =========================
    // RBT
    // =========================
    auto start = std::chrono::high_resolution_clock::now();

    auto rbtRecords =
        db.topK(k);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.records = rbtRecords;

    if(result.records.size() > k)
    {
        result.records.resize(k);
    }


    result.visitedNodes =
        db.getScoreIndex().getVisitedNodes();

    result.visitedRBT =
        result.visitedNodes.size();

    // =========================
    // LINEAR
    // =========================
    start = std::chrono::high_resolution_clock::now();

    auto linearRecords =
        db.linearTopK(k);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedLinear =
        db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::median(
    Database& db
)
{
    QueryResult result;

    // =========================
    // RBT
    // =========================
    auto start = std::chrono::high_resolution_clock::now();

    double medianValue =
        db.medianScore();

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedNodes =
        db.getScoreIndex().getVisitedNodes();

    result.visitedRBT =
        result.visitedNodes.size();

    // Guardamos como Record fake para visualizar
    Record medianRecord;
    medianRecord.id = -1;
    medianRecord.name = "Median Result";
    medianRecord.age = 0;
    medianRecord.score = medianValue;
    medianRecord.category = "Median";

    result.records.push_back(medianRecord);

    // =========================
    // LINEAR
    // =========================
    start = std::chrono::high_resolution_clock::now();

    double linearMedian =
        db.linearMedianScore();

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedLinear =
        db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::equalitySearch(
    Database& db,
    double score
)
{
    QueryResult result;

    auto start = std::chrono::high_resolution_clock::now();

    auto rbtRecords =
        db.findBetween(score, score);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.records = rbtRecords;

    result.visitedNodes =
        db.getScoreIndex().getVisitedNodes();

    result.visitedRBT =
        result.visitedNodes.size();

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords =
        db.linearFindBetween(score, score);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedLinear =
        db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::bottomK(
    Database& db,
    int k
)
{
    QueryResult result;

    auto start = std::chrono::high_resolution_clock::now();

    auto rbtRecords =
        db.bottomK(k);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.records = rbtRecords;

    if(result.records.size() > k)
    {
        result.records.resize(k);
    }

    result.visitedNodes =
        db.getScoreIndex().getVisitedNodes();

    result.visitedRBT =
        result.visitedNodes.size();

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords =
        db.linearBottomK(k);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedLinear =
        db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::percentile(
    Database& db,
    double p
)
{
    QueryResult result;

    auto start = std::chrono::high_resolution_clock::now();

    double percentileValue =
        db.percentileScore(p);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedNodes =
        db.getScoreIndex().getVisitedNodes();

    result.visitedRBT =
        result.visitedNodes.size();

    Record percentileRecord;
    percentileRecord.id = -1;
    percentileRecord.name = "Percentile Result";
    percentileRecord.age = 0;
    percentileRecord.score = percentileValue;
    percentileRecord.category = "Percentile";

    result.records.push_back(
        percentileRecord
    );

    start = std::chrono::high_resolution_clock::now();

    double linearPercentile =
        db.linearPercentileScore(p);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start).count();

    result.visitedLinear =
        db.getAllRecords().size();

    return result;
}