//
// Created by facum on 9/06/2026.
//

#ifndef TREELAYOUT_H
#define TREELAYOUT_H

#pragma once

#include <SFML/Graphics.hpp>
#include "../rbtree/RBNode.h"

struct VisualNode
{
    RBNode* node;

    sf::Vector2f position;

    int level;

    bool visited=false;
};

class TreeLayout
{
public:

    static void computeLayout(
    RBNode* node,
    RBNode* NIL,
    std::vector<VisualNode>& layout,
    int depth, // Lo dejamos aquí para no romper tu llamada
    int maxDepth);

    static void calculatePositions(
    RBNode* node,
    RBNode* NIL,
    int depth,
    int maxDepth,
    int& xCounter);

    static void fillLayoutVector(
    RBNode* node,
    RBNode* NIL,
    std::vector<VisualNode>& layout,
    int maxDepth,
    int depth);

};



#endif //TREELAYOUT_H
