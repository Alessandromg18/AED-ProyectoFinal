#include "TreeLayout.h"

static void layoutDFS(RBNode* node,RBNode* NIL,int depth,int& xCounter,std::vector<VisualNode>& nodes,double spacingX,double spacingY)
{
    if(node == nullptr || node == NIL)
        return;

    layoutDFS(node->left,NIL,depth + 1,xCounter,nodes,spacingX,spacingY);

    VisualNode v;

    v.node = node;
    v.level = depth;

    v.position = sf::Vector2f(700 + xCounter * spacingX,depth * spacingY);

    nodes.push_back(v);

    xCounter++;

    layoutDFS(node->right,NIL,depth + 1,xCounter,nodes,spacingX,spacingY);
}

void TreeLayout::computeLayout(RBNode* node,RBNode* NIL,std::vector<VisualNode>& layout, int depth,int maxDepth)
{
    if (node == NIL) return;

    int xCounter = 0;

    calculatePositions(node, NIL, 0, maxDepth, xCounter);

    fillLayoutVector(node, NIL, layout, maxDepth, 0);
}

void TreeLayout::calculatePositions(RBNode* node, RBNode* NIL, int depth, int maxDepth, int& xCounter) {

    if (node == NIL || depth >= maxDepth) return;

    calculatePositions(node->left, NIL, depth + 1, maxDepth, xCounter);

    node->x = 50.f + (xCounter * 45.f);

    float dynamicY = 0.f;
    float currentGap = 100.f;

    for (int i = 0; i < depth; ++i) {
        dynamicY += currentGap;

        currentGap *= 1.18f;

        if (currentGap > 190.f) currentGap = 190.f;
    }

    node->y = dynamicY;

    xCounter++;

    calculatePositions(node->right, NIL, depth + 1, maxDepth, xCounter);
}

// Función auxiliar para pasar los datos al vector de layout

void TreeLayout::fillLayoutVector(RBNode* node,RBNode* NIL,std::vector<VisualNode>& layout,int maxDepth,int depth) {

    if (node == NIL || depth >= maxDepth) return;

    layout.push_back({node,{node->x, node->y},false});

    fillLayoutVector(node->left, NIL, layout, maxDepth, depth + 1);
    fillLayoutVector(node->right, NIL, layout, maxDepth, depth + 1);
}