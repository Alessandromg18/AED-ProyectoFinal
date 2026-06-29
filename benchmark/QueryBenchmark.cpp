//
// Created by facum on 27/06/2026.
//

#include "QueryBenchmark.h"
#include <chrono>

QueryResult QueryBenchmark::searchById(Database& db,int id)
{
    QueryResult result;

    // PARA METODO RBT :

    auto start = std::chrono::high_resolution_clock::now();

    Record* rbtRecord = db.findById(id);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast <std::chrono::microseconds>(end - start).count();

    if(rbtRecord) result.records.push_back(*rbtRecord);

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    // PARA METODO LINEAL :

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords = db.linearFindById(id);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::rangeSearch(Database& db,double min,double max)
{
    QueryResult result;

    // PARA METODO RBT :

    auto start = std::chrono::high_resolution_clock::now();

    auto rbtRecords = db.findBetween(min, max);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.records = rbtRecords;

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    // PARA METODO LINEAL :

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords = db.linearFindBetween(min, max);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.getAllRecords().size();

    return result;
}


QueryResult QueryBenchmark::topK(Database& db,int k)
{
    QueryResult result;

    // PARA METODO RBT :

    auto start = std::chrono::high_resolution_clock::now();

    auto rbtRecords = db.topK(k);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.records = rbtRecords;

    if(result.records.size() > k)
    {
        result.records.resize(k);
    }

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    // PARA METODO LINEAL

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords = db.linearTopK(k);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::median(Database& db)
{
    QueryResult result;

    // PARA METODO RBT :
    auto start = std::chrono::high_resolution_clock::now();

    double medianValue = db.medianScore();

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    // Lo hemos guardado como un Record falso para visualizar

    Record medianRecord;
    medianRecord.id = -1;
    medianRecord.name = "Resultado Mediana";
    medianRecord.age = 0;
    medianRecord.score = medianValue;
    medianRecord.category = "Mediana";

    result.records.push_back(medianRecord);

    // PARA METODO LINEAL

    start = std::chrono::high_resolution_clock::now();

    double linearMedian = db.linearMedianScore();

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::equalitySearch(Database& db,double score)
{
    QueryResult result;

    // PARA METODO RBT :

    auto start = std::chrono::high_resolution_clock::now();

    auto rbtRecords = db.findBetween(score, score);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.records = rbtRecords;

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    // PARA METODO LINEAL

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords = db.linearFindBetween(score, score);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::bottomK(Database& db,int k)
{
    QueryResult result;

    // PARA METODO RBT

    auto start = std::chrono::high_resolution_clock::now();

    auto rbtRecords = db.bottomK(k);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.records = rbtRecords;

    if(result.records.size() > k)
    {
        result.records.resize(k);
    }

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    // PARA METODO LINEAL :

    start = std::chrono::high_resolution_clock::now();

    auto linearRecords = db.linearBottomK(k);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::percentile(Database& db,double p)
{
    QueryResult result;

    // PARA METODO RBT :

    auto start = std::chrono::high_resolution_clock::now();

    double percentileValue = db.percentileScore(p);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    Record percentileRecord;
    percentileRecord.id = -1;
    percentileRecord.name = "Percentile Result";
    percentileRecord.age = 0;
    percentileRecord.score = percentileValue;
    percentileRecord.category = "Percentile";

    result.records.push_back(percentileRecord);

    // PARA METODO LINEAL :

    start = std::chrono::high_resolution_clock::now();

    double linearPercentile = db.linearPercentileScore(p);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.getAllRecords().size();

    return result;
}

QueryResult QueryBenchmark::countRange(Database& db,double min,double max)
{
    QueryResult result;

    // Para metodo RBT :

    auto start = std::chrono::high_resolution_clock::now();

    int count = db.countBetween(min, max);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    Record r;
    r.id = -1;
    r.name = "COUNT";
    r.age = 0;
    r.score = count;
    r.category = "-";

    result.records.push_back(r);

    // Para metodo lineal

    start = std::chrono::high_resolution_clock::now();

    db.linearCountBetween(min,max);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.totalRecords();

    return result;
}

QueryResult QueryBenchmark::rank(
    Database& db,
    double key
)
{
    QueryResult result;

    // Para metodo con RBT :

    auto start = std::chrono::high_resolution_clock::now();

    int rankValue = db.rankScore(key);

    auto end = std::chrono::high_resolution_clock::now();

    result.rbtTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedNodes = db.getScoreIndex().getVisitedNodes();

    result.visitedRBT = result.visitedNodes.size();

    Record r;
    r.id = -1;
    r.name = "RANK";
    r.age = 0;
    r.score = rankValue;
    r.category = "-";

    result.records.push_back(r);

    // Para metodo con lineal :

    start = std::chrono::high_resolution_clock::now();

    db.linearRankScore(key);

    end = std::chrono::high_resolution_clock::now();

    result.linearTimeUs = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    result.visitedLinear = db.totalRecords();

    return result;
}