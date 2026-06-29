//
// Created by facum on 9/06/2026.
//

#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H

#pragma once

#include <vector>
#include "RBNode.h"

class RedBlackTree {

private:

    RBNode* root;

    RBNode* NIL;

    long long visitedNodes;

    std::vector<RBNode*> lastVisitedNodes;

public:

    RedBlackTree();

    ~RedBlackTree();

    void insert(double key,int rowId);

    bool remove(double key,int rowId);

    RBNode* search(double key);

    int rank(double key);

    double select(int k);

    std::vector<int> rangeQuery(double low,double high);

    int countRange(double low,double high);

    double median();

    double percentile(double p);

    int totalSize() const;

    bool contains(double key);

    double max();

    double min();

    std::vector<RBNode*> getNodes();

    RBNode* getRoot()
    {
        return root;
    }

    void clear();

    void resetVisited();

    long long getVisited() const;

    const std::vector<RBNode*>& getVisitedNodes() const
    {
        return lastVisitedNodes;
    }

    RBNode* getNIL() { return NIL; }

private:

    void leftRotate(RBNode* x);

    void rightRotate(RBNode* y);

    void fixInsert(RBNode* z);

    void fixDelete(RBNode* x);

    void transplant(RBNode* u,RBNode* v);

    RBNode* minimum(RBNode* node);

    void updateSize(RBNode* node);

    void updateToRoot(RBNode* node);

    RBNode* selectNode(RBNode* node,int k);

    RBNode* maxNode(RBNode* node);

    void rangeQueryRecursive(RBNode* node,double low,double high,std::vector<int>& result);

    void removeNode(RBNode* z);

    void destroy(RBNode* node);

    void inorderNodes(RBNode* node,std::vector<RBNode*>& result){

        if(node == NIL)
            return;

        inorderNodes(node->left,result);

        result.push_back(node);

        inorderNodes(node->right,result);
    }

    void visitNode(RBNode* node)
    {
        if(node == NIL)
            return;

        visitedNodes++;

        lastVisitedNodes.push_back(node);
    }

};

#endif
