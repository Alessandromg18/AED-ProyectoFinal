//
// Created by facum on 9/06/2026.
//

#ifndef RBVISUALIZER_H
#define RBVISUALIZER_H
#include <SFML/Graphics.hpp>

#include "TreeLayout.h"
#include "../rbtree/RedBlackTree.h"
#include "DatabaseVisualizer.h"
#include "../database/Database.h"
#include "../benchmark/QueryResult.h"
#include "../benchmark/QueryBenchmark.h"

class RBVisualizer
{
private:

    sf::RenderWindow window;

    sf::Font font;

    std::unique_ptr<DatabaseVisualizer> dbView;

    RBNode* currentNode = nullptr;

    std::vector<VisualNode> layout;

    sf::View camera;

    RedBlackTree* treeRef = nullptr;

    QueryResult currentQuery;

    bool hasQuery = false;

    std::string commandBuffer;

    bool typingCommand = false;

    Database* dbRef = nullptr;

    bool showHelp = false;

public:

    RBVisualizer();

    void show(RedBlackTree& tree);

    void setDatabase(const Database& db);

    void setQueryResult(const QueryResult& result);

private:

    void processEvents();

    void updateLayout(RedBlackTree& tree);

    void draw();

    void drawConnections();

    void drawNodes();

    void drawNode(const VisualNode&);

    void drawQueryInfo();

    void drawCommandBox();

    void processCommand(const std::string& cmd);

    void drawHelpOverlay();
};


#endif //RBVISUALIZER_H
