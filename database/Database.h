//
// Created by facum on 9/06/2026.
//

#ifndef DATABASE_H
#define DATABASE_H

#pragma once

#include "../models/Record.h"
#include "../hash/HashTable.h"
#include "../rbtree/RedBlackTree.h"

#include <vector>

class Database {

private:

    HashTable<int, Record> records;
    RedBlackTree scoreIndex;

public:

    Database();

    bool insertRecord(const Record& record);

    bool deleteRecord(int id);

    bool updateScore(int id,double newScore);

    Record* findById(int id);

    std::vector<Record> findBetween(double low,double high);

    double medianScore();

    double percentileScore(double p);

    int totalRecords();

    bool contains(int id);

    std::vector<Record> topK(int k);

    std::vector<Record> bottomK(int k);

    RedBlackTree& getScoreIndex()
    {
        return scoreIndex;
    }

    void loadCSV(const std::string& filename);

    void clear()
    {
        records.clear();
        scoreIndex.clear();
    }

    std::vector<Record> getAllRecords() const;

    std::vector<Record> linearFindById(int id);

    std::vector<Record> linearFindBetween(double low,double high);

    std::vector<Record> linearTopK(int k);

    double linearMedianScore();

    std::vector<Record> linearBottomK(int k);

    double linearPercentileScore(double p);

    int countBetween(double low,double high);

    int linearCountBetween(double low,double high);

    int rankScore(double score);

    int linearRankScore(double score);
};

#endif //DATABASE_H
