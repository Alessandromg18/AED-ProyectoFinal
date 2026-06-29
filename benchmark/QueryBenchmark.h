//
// Created by facum on 27/06/2026.
//

#ifndef QUERYBENCHMARK_H
#define QUERYBENCHMARK_H
#pragma once

#include "../database/Database.h"
#include "QueryResult.h"

class QueryBenchmark
{
public:

    static QueryResult searchById(Database& db,int id);

    static QueryResult rangeSearch(Database& db,double min,double max);

    static QueryResult topK(Database& db,int k);

    static QueryResult median(Database& db);

    static QueryResult equalitySearch(Database& db,double score);

    static QueryResult percentile(Database& db,double p);

    static QueryResult bottomK(Database& db,int k);

    static QueryResult countRange(Database& db,double min,double max);

    static QueryResult rank(Database& db,double key);

};

#endif //QUERYBENCHMARK_H
