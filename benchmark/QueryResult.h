//
// Created by facum on 27/06/2026.
//

#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#pragma once

#include "../models/Record.h"
#include "../rbtree/RBNode.h"
#include <vector>

struct QueryResult
{
    std::vector<Record> records;

    long long rbtTimeUs;
    long long linearTimeUs;

    int visitedRBT;
    int visitedLinear;

    std::vector<RBNode*> visitedNodes;
};


#endif //QUERYRESULT_H
