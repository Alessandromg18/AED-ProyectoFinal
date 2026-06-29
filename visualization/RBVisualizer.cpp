//
// Created by facum on 9/06/2026.
//
#include "RBVisualizer.h"
#include "TreeLayout.h"
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <iostream>

RBVisualizer::RBVisualizer() : window(sf::VideoMode({1200, 700}), "Red Black Tree Frontend"),
      camera(window.getDefaultView())
{
    sf::Image icon;

    if(icon.loadFromFile("assets/utec_logo.png"))
    {
        window.setIcon(icon.getSize(),icon.getPixelsPtr());
    }
    else
    {
        std::cout << "Error cargando icono\n";
    }

    if(!font.openFromFile("assets/fonts/ARIAL.TTF"))
    {
        std::cout << "Error cargando fuente\n";
    }

    dbView = std::make_unique<DatabaseVisualizer>(font);
}

void RBVisualizer::show(RedBlackTree& tree)
{
    updateLayout(tree);

    treeRef = &tree;
    currentNode = tree.getRoot();

    while(window.isOpen())
    {
        processEvents();
        window.clear(sf::Color::White);
        window.setView(window.getDefaultView());
        dbView->draw(window);

        drawCommandBox();
        drawQueryInfo();
        drawHelpOverlay();

        window.setView(camera);
        drawConnections();
        drawNodes();

        window.display();
    }
}

void RBVisualizer::processEvents()
{
    while(auto event = window.pollEvent())
    {
        if(event->is<sf::Event::Closed>())
            window.close();

        if(auto mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            if(mouseWheel->delta > 0)
                camera.zoom(0.9f);
            else
                camera.zoom(1.1f);
        }

        if(auto key = event->getIf<sf::Event::KeyPressed>())
        {
            if(key->code == sf::Keyboard::Key::F1)
            {
                showHelp = !showHelp;
            }

            if(key->code == sf::Keyboard::Key::B)
            {
                typingCommand = true;
                commandBuffer.clear();
            }

            if(typingCommand)
            {
                return;
            }

            // Moverse en el arbol

            if(key->code == sf::Keyboard::Key::A)
            {
                if(currentNode && currentNode->left && currentNode->left != treeRef->getNIL())
                {
                    currentNode = currentNode->left;
                }
            }

            else if(key->code == sf::Keyboard::Key::D)
            {
                if(currentNode && currentNode->right && currentNode->right != treeRef->getNIL())
                {
                    currentNode = currentNode->right;
                }
            }

            else if(key->code == sf::Keyboard::Key::W)
            {
                if(currentNode && currentNode->parent && currentNode != treeRef->getRoot())
                {
                    currentNode = currentNode->parent;
                }
            }

            else if(key->code == sf::Keyboard::Key::R)
            {
                currentNode = treeRef->getRoot();
            }

            // Moverse de pagina en el dataset

            else if(key->code == sf::Keyboard::Key::O)
            {
                dbView->prevPage();
            }

            else if(key->code == sf::Keyboard::Key::P)
            {
                dbView->nextPage();
            }

            updateLayout(*treeRef);
        }

        if(auto text = event->getIf<sf::Event::TextEntered>())
        {
            if(typingCommand)
            {
                if(text->unicode == '\b')
                {
                    if(!commandBuffer.empty())
                        commandBuffer.pop_back();
                }
                else if(text->unicode == '\r')
                {
                    processCommand(commandBuffer);
                    typingCommand = false;
                }
                else
                {
                    commandBuffer += static_cast<char>(text->unicode);
                }
            }
        }

    }

    float speed = 2.f;

    // Mover la camara :

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        camera.move({-speed, 0});

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        camera.move({speed, 0});

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        camera.move({0, -speed});

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        camera.move({0, speed});

}

void RBVisualizer::updateLayout(RedBlackTree& tree)
{
    layout.clear();

    if(!currentNode) currentNode = tree.getRoot();

    TreeLayout::computeLayout(currentNode, tree.getNIL(), layout, 0, 5);

    std::unordered_set<RBNode*> visited(tree.getVisitedNodes().begin(), tree.getVisitedNodes().end());

    for(auto& v : layout) {
        v.visited = visited.count(v.node) > 0;
    }

    if (currentNode && currentNode != tree.getNIL()) {
        static RBNode* lastNode = nullptr;

        if (currentNode != lastNode) {
            sf::Vector2f viewSize = camera.getSize();
            camera.setCenter({currentNode->x, currentNode->y + (viewSize.y / 2.f) - 45.f});
            lastNode = currentNode;
        }

        camera.setViewport(sf::FloatRect({0.f, 0.45f}, {1.f, 0.55f}));
    }
}

void RBVisualizer::drawConnections()
{
    for(const auto& v : layout)
    {
        if(v.node->left)
        {
            auto it = std::find_if(layout.begin(),layout.end(),
                [&](const VisualNode& n){ return n.node == v.node->left; });

            if(it != layout.end())
            {
                sf::Vertex line[] = {sf::Vertex(v.position, sf::Color::Black), sf::Vertex(it->position, sf::Color::Black)};

                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }

        if(v.node->right)
        {
            auto it = std::find_if(layout.begin(), layout.end(),
                [&](const VisualNode& n){ return n.node == v.node->right; } );

            if(it != layout.end())
            {
                sf::Vertex line[] = {sf::Vertex(v.position, sf::Color::Black), sf::Vertex(it->position, sf::Color::Black)};

                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }
    }
}

void RBVisualizer::drawNodes()
{
    for(const auto& v : layout)
        drawNode(v);
}

void RBVisualizer::drawNode(const VisualNode& v)
{
    sf::CircleShape circle(24.f);
    circle.setPosition(v.position - sf::Vector2f(17.f, 20.f));

    sf::Color outlineColor;
    float thickness = 1.f;

    if(v.node == currentNode)
    {
        outlineColor = sf::Color::Yellow;
        thickness = 4.f;
    }

    else if(v.visited)
    {
        outlineColor = sf::Color::Blue;
        thickness = 3.f;
    }

    else
    {
        outlineColor = sf::Color::White;
        thickness = 1.f;
    }

    circle.setOutlineThickness(thickness);
    circle.setOutlineColor(outlineColor);

    // COLOREAMOS LOS NODOS :

    if(v.node->color == RED)
        circle.setFillColor(sf::Color(200, 50, 50));
    else
        circle.setFillColor(sf::Color::Black);
    window.draw(circle);

    // Colocamos el score en el centro del nodo :

    sf::Text keyText(font);

    std::ostringstream oss;

    if(v.node->key == (int)v.node->key)
    {
        oss << (int)v.node->key;
    }

    else
    {
        oss << std::fixed << std::setprecision(1) << v.node->key;
    }

    keyText.setString(oss.str());
    keyText.setCharacterSize(9);
    keyText.setFillColor(sf::Color::White);

    auto bounds = keyText.getLocalBounds();

    keyText.setOrigin({bounds.position.x + bounds.size.x / 3.f, bounds.position.y + bounds.size.y / 3.f });

    keyText.setPosition(v.position);

    window.draw(keyText);

    // Colocamos su SUBTREE SIZE en el superior derecho

    sf::Text sizeText(font);

    sizeText.setString("s:" +std::to_string(v.node->subtreeSize));

    sizeText.setCharacterSize(9);
    sizeText.setFillColor(sf::Color::Black);

    sizeText.setPosition(v.position + sf::Vector2f(26.f, -24.f));

    window.draw(sizeText);
}

void RBVisualizer::setDatabase(const Database& db)
{
    dbRef = const_cast<Database*>(&db);
    dbView->setRecords(db.getAllRecords());
}

void RBVisualizer::setQueryResult(const QueryResult& result)
{
    currentQuery = result;
    hasQuery = true;

    if(!result.visitedNodes.empty())
    {
        currentNode = result.visitedNodes.front();
    }

    dbView->setRecords(result.records);

    // Resaltamos los registros

    std::vector<int> ids;

    for(const auto& r : result.records)
    {
        ids.push_back(r.id);
    }

    dbView->highlight(ids);
}

void RBVisualizer::drawQueryInfo()
{
    if(!hasQuery)
        return;

    sf::Vector2u windowSize = window.getSize();
    float posX = windowSize.x - 400.f - 20.f;
    float posY = 55.f;

    sf::Text text(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color(50, 50, 50));

    text.setString(
        "Tiempo RBT: " + std::to_string(currentQuery.rbtTimeUs) + " us\n" +
        "Tiempo Lineal: " + std::to_string(currentQuery.linearTimeUs) + " us\n" +
        "Nodes Visitados en RBT: " + std::to_string(currentQuery.visitedRBT) + "\n" +
        "Nodos Visitados en Lineal: " + std::to_string(currentQuery.visitedLinear)
    );

    text.setPosition({posX + 5.f, posY});
    window.draw(text);
}

void RBVisualizer::processCommand(const std::string& cmd)
{
    if(!dbRef) return;

    if(cmd == "reset")
    {
        hasQuery = false;

        // Limpiamos los nodos visitados del árbol :

        treeRef->resetVisited();

        // Restauramos la tabla completa :

        dbView->clearHighlights();
        dbView->setRecords(dbRef->getAllRecords());

        // Volvemos a la raiz :

        currentNode = treeRef->getRoot();

        // Reseteamos cámara :

        camera = window.getDefaultView();

        updateLayout(*treeRef);

        return;
    }

    std::istringstream ss(cmd);
    std::string op;
    ss >> op;

    if(op == "top")
    {
        int k;

        if(!(ss >> k) || k <= 0)
        {
            commandBuffer = "ERROR: Debes usar -> top <entero positivo>";
            return;
        }

        auto result = QueryBenchmark::topK(*dbRef, k);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "bottom")
    {
        int k;

        if(!(ss >> k) || k <= 0)
        {
            commandBuffer = "ERROR: Debes usar -> bottom <entero positivo>";
            return;
        }

        auto result = QueryBenchmark::bottomK(*dbRef, k);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "id")
    {
        int id;

        if(!(ss >> id))
        {
            commandBuffer = "ERROR: Debes usar -> id <entero>";
            return;
        }

        auto result = QueryBenchmark::searchById(*dbRef, id);

        if(result.records.empty())
        {
            commandBuffer = "ID no identificado";
            return;
        }

        setQueryResult(result);
        hasQuery = true;

        double score = result.records[0].score;

        currentNode = treeRef->search(score);

        updateLayout(*treeRef);
    }

    else if(op == "rango")
    {
        double a, b;

        if(!(ss >> a >> b))
        {
            commandBuffer = "ERROR: Debes usar -> rango <min> <max>";
            return;
        }

        if(a > b)
            std::swap(a, b);

        auto result = QueryBenchmark::rangeSearch(*dbRef, a, b);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "percentil")
    {
        double p;

        if(!(ss >> p))
        {
            commandBuffer = "ERROR: Debes usar -> percentil <0-100>";
            return;
        }

        if(p < 0 || p > 100)
        {
            commandBuffer =
                "ERROR: Percentil no esta entre 0 o 100";
            return;
        }

        auto result = QueryBenchmark::percentile(*dbRef, p);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "mediana")
    {
        auto result = QueryBenchmark::median(*dbRef);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "score")
    {
        double score;

        if(!(ss >> score))
        {
            commandBuffer = "ERROR: Debes usar -> score <numero>";
            return;
        }

        auto result = QueryBenchmark::equalitySearch(*dbRef,score);

        if(result.records.empty())
        {
            commandBuffer = "No hay registros con ese score";
            return;
        }

        setQueryResult(result);
        hasQuery = true;

        currentNode = treeRef->search(score);

        updateLayout(*treeRef);
    }

    else if(op == "insert")
    {
        int id;
        std::string name;
        int age;
        double score;
        std::string category;

        if(!(ss >> id >> name >> age >> score >> category))
        {
            commandBuffer = "ERROR: Formato de insert invalido";
            return;
        }

        Record r{id,name,age,score,category};

        bool inserted = dbRef->insertRecord(r);

        if(!inserted)
        {
            commandBuffer = "ERROR: Ese ID ya existe";

            return;
        }

        commandBuffer = "Se inserto correctamente";

        dbView->setRecords(dbRef->getAllRecords());

        currentNode = treeRef->search(score);

        updateLayout(*treeRef);

        typingCommand = false;
    }

    else if(op == "delete")
    {
        int id;
        ss >> id;

        Record* r = dbRef->findById(id);

        if(!r)
        {
            commandBuffer = "ERROR: ID no encontrado";
            return;
        }

        double score = r->score;

        bool deleted = dbRef->deleteRecord(id);

        if(!deleted)
        {
            commandBuffer = "ERROR elimando ese registro";

            return;
        }

        commandBuffer = "Se elimino correctamente";

        dbView->setRecords(dbRef->getAllRecords());

        RBNode* found = treeRef->search(score);

        if(found != treeRef->getNIL())
            currentNode = found;
        else
            currentNode = treeRef->getRoot();

        updateLayout(*treeRef);

        typingCommand = false;
    }

    else if(op == "count")
    {
        double a, b;

        if(!(ss >> a >> b))
        {
            commandBuffer = "ERROR: Debes usar -> count <min> <max>";
            return;
        }

        auto result =QueryBenchmark::countRange(*dbRef,a,b);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "rank")
    {
        double key;

        if(!(ss >> key))
        {
            commandBuffer = "ERROR: Debes usar -> rank <score>";
            return;
        }

        auto result = QueryBenchmark::rank(*dbRef,key);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "update")
    {
        int id;
        double newScore;

        if(!(ss >> id >> newScore))
        {
            commandBuffer = "ERROR: Debes usar -> update <id> <score>";
            return;
        }

        Record* r = dbRef->findById(id);

        if(!r)
        {
            commandBuffer = "ERROR: ID no encontrado";
            return;
        }

        bool updated = dbRef->updateScore(id,newScore);

        if(!updated)
        {
            commandBuffer ="ERROR al actualizar el registro";
            return;
        }

        commandBuffer = "Actualizacion completada";

        dbView->setRecords(dbRef->getAllRecords());

        currentNode = treeRef->search(newScore);

        updateLayout(*treeRef);
    }

}

void RBVisualizer::drawCommandBox()
{
    sf::Vector2u windowSize = window.getSize();

    float boxWidth = 400.f;
    float boxHeight = 30.f;

    float posX = windowSize.x - boxWidth - 40.f;
    float posY = 10.f;

    sf::RectangleShape box({boxWidth, boxHeight});
    box.setPosition({posX, posY});
    box.setFillColor(sf::Color(230, 230, 230));

    box.setOutlineThickness(typingCommand ? 2.f : 0.f);

    // Si estamos escribiendo se pone de rojo

    box.setOutlineColor(sf::Color(50, 200, 255));

    window.draw(box);

    sf::Text text(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Black);

    // Si no está escribiendo, ponemos un texto guía

    if (commandBuffer.empty() && !typingCommand) {
        text.setString("Presiona 'B' para escribir un comando...");
        text.setFillColor(sf::Color(150, 150, 150));
    } else {
        text.setString(commandBuffer);
    }

    text.setPosition({posX + 10.f, posY + 5.f});
    window.draw(text);
}

void RBVisualizer::drawHelpOverlay()
{
    if(!showHelp) return;

    sf::Vector2u windowSize = window.getSize();

    float boxWidth = 430.f;
    float posX = windowSize.x - boxWidth - 20.f;
    float posY = 150.f;

    // CONTENIDO

    sf::Text body(font);
    body.setCharacterSize(11);
    body.setFillColor(sf::Color::Black);

    body.setString(
        " CONTROLES DEL VISUALIZADOR:\n"
        "  B : Activar barra / Flechas : Mover camara\n"
        "  W, A, D : Moverse en Arbol (Padre, Izq, Der)\n"
        "  R : Ir a Raiz / MouseWheel : Zoom\n"
        "  O, P : Pagina Anterior / Siguiente DB\n\n"

        " COMANDOS DISPONIBLES:\n"
        "  top <n>            : Top K scores\n"
        "  bottom <n>         : Bottom K scores\n"
        "  id <id>            : Buscar por ID\n"
        "  score <valor>      : Buscar score exacto\n"
        "  rango <min> <max>  : Buscar por rango\n"
        "  rank <score>       : Cantidad menores al score\n"
        "  percentil <p>     : Buscar percentil\n"
        "  mediana             : Obtener mediana\n"
        "  count <min> <max>  : Contar rango\n"
        "  insert <id> <name> <age> <score> <cat>\n"
        "  delete <id>        : Eliminar registro\n"
        "  update <id> <score>: Actualizar score\n"
        "  reset              : Restaurar vista"
    );

    body.setPosition({posX + 8.f, posY + 28.f});

    // Altura :
    auto bodyBounds = body.getLocalBounds();

    float helpHeight = bodyBounds.size.y + 45.f;

    // Fondo :
    sf::RectangleShape helpBox({boxWidth, helpHeight});

    helpBox.setPosition({posX, posY});
    helpBox.setFillColor(sf::Color(245, 245, 245));
    helpBox.setOutlineThickness(1.f);
    helpBox.setOutlineColor(sf::Color(200, 200, 200));

    window.draw(helpBox);

    sf::RectangleShape titleBar({boxWidth, 20.f});

    titleBar.setPosition({posX, posY});
    titleBar.setFillColor(sf::Color(80, 80, 80));

    window.draw(titleBar);

    sf::Text title(font,"GUIA DE CONTROLES (F1 para cerrar)");

    title.setCharacterSize(11);
    title.setStyle(sf::Text::Style::Bold);
    title.setFillColor(sf::Color::White);
    title.setPosition({posX + 5.f, posY + 2.f});

    window.draw(title);

    window.draw(body);
}