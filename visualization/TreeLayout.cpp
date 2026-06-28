#include "TreeLayout.h"
#include <cmath>

static void layoutDFS(
    RBNode* node,
    RBNode* NIL,
    int depth,
    int& xCounter,
    std::vector<VisualNode>& nodes,
    double spacingX,
    double spacingY
)
{
    if(node == nullptr || node == NIL)
        return;

    layoutDFS(
        node->left,
        NIL,
        depth + 1,
        xCounter,
        nodes,
        spacingX,
        spacingY
    );

    VisualNode v;
    v.node = node;
    v.level = depth;

    v.position = sf::Vector2f(700 +
        xCounter * spacingX,
        depth * spacingY
    );

    nodes.push_back(v);

    xCounter++;

    layoutDFS(
        node->right,
        NIL,
        depth + 1,
        xCounter,
        nodes,
        spacingX,
        spacingY
    );
}

void TreeLayout::computeLayout(
RBNode* node,
RBNode* NIL,
std::vector<VisualNode>& layout,
int depth, // Se recibe el 0, pero no lo usamos para el conteo principal
int maxDepth
) {
    if (node == NIL) return;

    // 1. Inicializamos un contador para el eje X
    int xCounter = 0;

    // 2. Calculamos las posiciones de todos los nodos
    calculatePositions(node, NIL, 0, maxDepth, xCounter);

    // 3. Llenamos el layout con un recorrido simple (Preorder o Postorder)
    fillLayoutVector(node, NIL, layout, maxDepth, 0);
}

void TreeLayout::calculatePositions(RBNode* node, RBNode* NIL, int depth, int maxDepth, int& xCounter) {
    if (node == NIL || depth >= maxDepth) return;

    calculatePositions(node->left, NIL, depth + 1, maxDepth, xCounter);

    // Mantenemos la horizontal en un buen tamaño para las etiquetas
    node->x = 50.f + (xCounter * 45.f);

    // --- ALTURA DINÁMICA CRECIENTE PARA EL ÚLTIMO NIVEL ---
    float dynamicY = 0.f;
    float currentGap = 100.f; // Empezamos más compactos arriba (Nivel 0 a 1)

    for (int i = 0; i < depth; ++i) {
        dynamicY += currentGap;

        // Multiplicamos para que cada nivel sea más alto que el anterior
        currentGap *= 1.18f;

        // Ponemos un tope máximo por si el árbol es muy profundo,
        // asegurando que el último nivel tenga un gran tamaño (ej. 190px)
        if (currentGap > 190.f) currentGap = 190.f;
    }

    node->y = dynamicY;

    xCounter++;

    calculatePositions(node->right, NIL, depth + 1, maxDepth, xCounter);
}

// Función auxiliar para pasar los datos al vector de layout
void TreeLayout::fillLayoutVector(
    RBNode* node,
    RBNode* NIL,
    std::vector<VisualNode>& layout,
    int maxDepth,
    int depth
) {
    if (node == NIL || depth >= maxDepth) return;

    layout.push_back({
        node,
        {node->x, node->y},
        false
    });

    fillLayoutVector(node->left, NIL, layout, maxDepth, depth + 1);
    fillLayoutVector(node->right, NIL, layout, maxDepth, depth + 1);
}