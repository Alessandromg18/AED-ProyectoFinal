//
// Created by facum on 9/06/2026.
//

#ifndef RBNODE_H
#define RBNODE_H

#pragma once

#include "../hash/HashSet.h"
#include "Color.h"

struct RBNode {

    double key;

    HashSetInt rowIds;

    Color color;

    RBNode* left;
    RBNode* right;
    RBNode* parent;

    int subtreeSize;

    RBNode(
        double key
    )
        : key(key),
          color(RED),
          left(nullptr),
          right(nullptr),
          parent(nullptr),
          subtreeSize(0)
    {}
};

#endif //RBNODE_H
