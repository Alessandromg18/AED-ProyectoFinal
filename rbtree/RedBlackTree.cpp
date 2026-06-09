//
// Created by facum on 9/06/2026.
//

#include "RedBlackTree.h"

#include <stdexcept>

RedBlackTree::RedBlackTree() {

    visitedNodes = 0;

    NIL = new RBNode(0);

    NIL->color = BLACK;

    NIL->subtreeSize = 0;

    NIL->left = NIL;
    NIL->right = NIL;
    NIL->parent = NIL;

    root = NIL;
}

void RedBlackTree::updateSize(
    RBNode* node
) {

    if(node == NIL)
        return;

    node->subtreeSize =
        node->left->subtreeSize +
        node->rowIds.size() +
        node->right->subtreeSize;
}

void RedBlackTree::updateToRoot(
    RBNode* node
) {

    while(node != NIL) {

        updateSize(node);

        node = node->parent;
    }
}

RBNode* RedBlackTree::search(
    double key
) {

    RBNode* current = root;

    while(current != NIL) {

        visitedNodes++;

        if(key == current->key)
            return current;

        if(key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    return nullptr;
}

void RedBlackTree::leftRotate(RBNode* x)
{
    RBNode* y = x->right;

    x->right = y->left;

    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        root = y;

    else if (x == x->parent->left)
        x->parent->left = y;

    else
        x->parent->right = y;

    y->left = x;

    x->parent = y;

    updateSize(x);
    updateSize(y);
}

void RedBlackTree::rightRotate(RBNode* y)
{
    RBNode* x = y->left;

    y->left = x->right;

    if (x->right != NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NIL)
        root = x;

    else if (y == y->parent->left)
        y->parent->left = x;

    else
        y->parent->right = x;

    x->right = y;

    y->parent = x;

    updateSize(y);
    updateSize(x);
}

void RedBlackTree::insert(
    double key,
    int rowId
)
{
    RBNode* current = root;
    RBNode* parent = NIL;

    while(current != NIL)
    {
        parent = current;

        if(key == current->key)
        {
            current->rowIds.insert(rowId);

            updateToRoot(current);

            return;
        }

        if(key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    RBNode* z = new RBNode(key);

    z->rowIds.insert(rowId);

    z->left = NIL;
    z->right = NIL;
    z->parent = parent;

    z->subtreeSize = 1;

    if(parent == NIL)
        root = z;

    else if(key < parent->key)
        parent->left = z;

    else
        parent->right = z;

    updateToRoot(parent);

    fixInsert(z);
}

void RedBlackTree::fixInsert(RBNode* z)
{
    while(z->parent->color == RED)
    {
        if(z->parent == z->parent->parent->left)
        {
            RBNode* y =
                z->parent->parent->right;

            if(y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;

                z->parent->parent->color =
                    RED;

                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->right)
                {
                    z = z->parent;

                    leftRotate(z);
                }

                z->parent->color = BLACK;

                z->parent->parent->color =
                    RED;

                rightRotate(
                    z->parent->parent
                );
            }
        }
        else
        {
            RBNode* y =
                z->parent->parent->left;

            if(y->color == RED)
            {
                z->parent->color = BLACK;

                y->color = BLACK;

                z->parent->parent->color =
                    RED;

                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->left)
                {
                    z = z->parent;

                    rightRotate(z);
                }

                z->parent->color = BLACK;

                z->parent->parent->color =
                    RED;

                leftRotate(
                    z->parent->parent
                );
            }
        }
    }

    root->color = BLACK;
}

RBNode* RedBlackTree::minimum(
    RBNode* node
)
{
    while(node->left != NIL)
        node = node->left;

    return node;
}

void RedBlackTree::transplant(
    RBNode* u,
    RBNode* v
)
{
    if(u->parent == NIL)
        root = v;

    else if(u == u->parent->left)
        u->parent->left = v;

    else
        u->parent->right = v;

    v->parent = u->parent;
}

RBNode* RedBlackTree::selectNode(
    RBNode* node,
    int k
)
{
    if(node == NIL)
        return NIL;

    visitedNodes++;

    int leftSize =
        node->left->subtreeSize;

    int currentCount =
        node->rowIds.size();

    if(k <= leftSize)
    {
        return selectNode(
            node->left,
            k
        );
    }

    if(
        k <= leftSize + currentCount
    )
    {
        return node;
    }

    return selectNode(
        node->right,
        k - leftSize - currentCount
    );
}

double RedBlackTree::select(
    int k
)
{
    if(
        root == NIL ||
        k <= 0 ||
        k > root->subtreeSize
    )
    {
        throw std::out_of_range(
            "k invalido"
        );
    }

    RBNode* result =
        selectNode(root, k);

    return result->key;
}

int RedBlackTree::rank(
    double key
)
{
    RBNode* current = root;

    int rankValue = 0;

    while(current != NIL)
    {
        visitedNodes++;

        if(key < current->key)
        {
            current =
                current->left;
        }
        else if(key > current->key)
        {
            rankValue +=
                current->left->subtreeSize;

            rankValue +=
                current->rowIds.size();

            current =
                current->right;
        }
        else
        {
            rankValue +=
                current->left->subtreeSize;

            return rankValue;
        }
    }

    return rankValue;
}

double RedBlackTree::median()
{
    if(root == NIL)
    {
        throw std::runtime_error(
            "Arbol vacio"
        );
    }

    int n =
        root->subtreeSize;

    if(n % 2 == 1)
    {
        return select(
            (n + 1) / 2
        );
    }

    double left =
        select(n / 2);

    double right =
        select(n / 2 + 1);

    return
        (left + right) / 2.0;
}

double RedBlackTree::percentile(double p)
{
    if (p < 0 || p > 100)
    {
        throw std::invalid_argument("Percentil invalido");
    }

    int n = root->subtreeSize;

    double pos = p * n / 100.0;
    int k = (int)pos + (pos > (int)pos);

    if (k == 0)
    {
        k = 1;
    }

    return select(k);
}

int RedBlackTree::countRange(
    double low,
    double high
)
{
    const double EPS =
        0.0000001;

    return
        rank(high + EPS)
        - rank(low);
}

void RedBlackTree::rangeQueryRecursive(
    RBNode* node,
    double low,
    double high,
    std::vector<int>& result
)
{
    if(node == NIL)
        return;

    visitedNodes++;

    if(node->key > low)
    {
        rangeQueryRecursive(
            node->left,
            low,
            high,
            result
        );
    }

    if(
        node->key >= low &&
        node->key <= high
    )
    {
        std::vector<int> ids =
            node->rowIds.toVector();

        for(int id : ids)
        {
            result.push_back(id);
        }
    }

    if(node->key < high)
    {
        rangeQueryRecursive(
            node->right,
            low,
            high,
            result
        );
    }
}

std::vector<int> RedBlackTree::rangeQuery(
    double low,
    double high
)
{
    std::vector<int> result;

    rangeQueryRecursive(
        root,
        low,
        high,
        result
    );

    return result;
}

bool RedBlackTree::remove(
    double key,
    int rowId
)
{
    RBNode* z = search(key);

    if(z == nullptr)
        return false;

    if(!z->rowIds.contains(rowId))
        return false;

    z->rowIds.remove(rowId);

    if(!z->rowIds.empty())
    {
        updateToRoot(z);
        return true;
    }

    removeNode(z);

    return true;
}

void RedBlackTree::removeNode(
    RBNode* z
)
{
    RBNode* y = z;

    RBNode* x;

    Color originalColor =
        y->color;

    if(z->left == NIL)
    {
        x = z->right;

        transplant(
            z,
            z->right
        );
    }
    else if(z->right == NIL)
    {
        x = z->left;

        transplant(
            z,
            z->left
        );
    }
    else
    {
        y =
            minimum(z->right);

        originalColor =
            y->color;

        x =
            y->right;

        if(y->parent == z)
        {
            x->parent = y;
        }
        else
        {
            transplant(
                y,
                y->right
            );

            y->right =
                z->right;

            y->right->parent =
                y;
        }

        transplant(z,y);

        y->left =
            z->left;

        y->left->parent =
            y;

        y->color =
            z->color;

        updateSize(y);
    }
    delete z;

    updateToRoot(
        x->parent
    );

    if(
        originalColor ==
        BLACK
    )
    {
        fixDelete(x);
    }
}

void RedBlackTree::fixDelete(
    RBNode* x
)
{
    while(
        x != root &&
        x->color == BLACK
    )
    {
        if(
            x == x->parent->left
        )
        {
            RBNode* w =
                x->parent->right;

            // CASO 1
            if(
                w->color == RED
            )
            {
                w->color = BLACK;

                x->parent->color = RED;

                leftRotate(
                    x->parent
                );

                w =
                    x->parent->right;
            }

            // CASO 2
            if(
                w->left->color == BLACK &&
                w->right->color == BLACK
            )
            {
                w->color = RED;

                x =
                    x->parent;
            }

            else
            {
                // CASO 3
                if(
                    w->right->color == BLACK
                )
                {
                    w->left->color =
                        BLACK;

                    w->color =
                        RED;

                    rightRotate(w);

                    w =
                        x->parent->right;
                }

                // CASO 4
                w->color =
                    x->parent->color;

                x->parent->color =
                    BLACK;

                w->right->color =
                    BLACK;

                leftRotate(
                    x->parent
                );

                x = root;
            }
        }
        else
        {
            RBNode* w =
                x->parent->left;

            // CASO 1 ESPEJO

            if(
                w->color == RED
            )
            {
                w->color = BLACK;

                x->parent->color =
                    RED;

                rightRotate(
                    x->parent
                );

                w =
                    x->parent->left;
            }

            // CASO 2 ESPEJO

            if(
                w->right->color == BLACK &&
                w->left->color == BLACK
            )
            {
                w->color = RED;

                x =
                    x->parent;
            }

            else
            {
                // CASO 3 ESPEJO

                if(
                    w->left->color == BLACK
                )
                {
                    w->right->color =
                        BLACK;

                    w->color =
                        RED;

                    leftRotate(w);

                    w =
                        x->parent->left;
                }

                // CASO 4 ESPEJO

                w->color =
                    x->parent->color;

                x->parent->color =
                    BLACK;

                w->left->color =
                    BLACK;

                rightRotate(
                    x->parent
                );

                x = root;
            }
        }
    }

    x->color = BLACK;
}

void RedBlackTree::destroy(
    RBNode* node
)
{
    if(
        node == NIL
    )
        return;

    destroy(node->left);

    destroy(node->right);

    delete node;
}

RedBlackTree::~RedBlackTree()
{
    destroy(root);

    delete NIL;
}

int RedBlackTree::totalSize() const
{
    return root->subtreeSize;
}

bool RedBlackTree::contains(
    double key
)
{
    return search(key)
        != nullptr;
}

RBNode* RedBlackTree::maxNode(
    RBNode* node
)
{
    while(
        node->right != NIL
    )
    {
        node =
            node->right;
    }

    return node;
}

double RedBlackTree::max()
{
    if(root == NIL)
        throw std::runtime_error(
            "Arbol vacio"
        );

    return maxNode(root)->key;
}

double RedBlackTree::min()
{
    if(root == NIL)
        throw std::runtime_error(
            "Arbol vacio"
        );

    return minimum(root)->key;
}

std::vector<RBNode*> RedBlackTree::getNodes()
{
    std::vector<RBNode*> result;

    inorderNodes(
        root,
        result
    );

    return result;
}

void RedBlackTree::clear()
{
    destroy(root);

    root = NIL;
}

void RedBlackTree::resetVisited()
{
    visitedNodes = 0;
}

long long RedBlackTree::getVisited() const
{
    return visitedNodes;
}