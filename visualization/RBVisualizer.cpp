//
// Created by facum on 9/06/2026.
//
#include "RBVisualizer.h"
#include "TreeLayout.h"
#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <iostream>



RBVisualizer::RBVisualizer()
    : window(sf::VideoMode({1200, 700}), "Red Black Tree Visualizer"),
      camera(window.getDefaultView())
{
    if(!font.openFromFile("assets/fonts/ARIAL.ttf"))
    {
        std::cout << "Error loading font\n";
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

        // --- RENDERIZADO DEL HUD (CÁMARA DEFAULT) ---
        window.setView(window.getDefaultView());

        // La Base de Datos ahora empieza pegada a la izquierda de la pantalla
        dbView->draw(window);

        // Bloque derecho consolidado
        drawCommandBox();
        drawQueryInfo();
        drawHelpOverlay(); // <-- NUEVO: Se dibuja sobre el HUD fijo

        // --- RENDERIZADO DEL ÁRBOL (CÁMARA DEL VIEWPORT) ---
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
                showHelp = !showHelp; // Alterna entre true y false
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

            // =========================
            // TREE NAVIGATION (WASD)
            // =========================

            if(key->code == sf::Keyboard::Key::A)
            {
                if(currentNode &&
                   currentNode->left &&
                   currentNode->left != treeRef->getNIL())
                {
                    currentNode = currentNode->left;
                }
            }

            else if(key->code == sf::Keyboard::Key::D)
            {
                if(currentNode &&
                   currentNode->right &&
                   currentNode->right != treeRef->getNIL())
                {
                    currentNode = currentNode->right;
                }
            }

            else if(key->code == sf::Keyboard::Key::W)
            {
                if(currentNode &&
                   currentNode->parent &&
                   currentNode != treeRef->getRoot())
                {
                    currentNode = currentNode->parent;
                }
            }

            else if(key->code == sf::Keyboard::Key::R)
            {
                currentNode = treeRef->getRoot();
            }



            // =========================
            // DATABASE PAGINATION (O/P)
            // =========================

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

            // --- MARGEN DE SEGURIDAD PARA LA RAÍZ ---
            // Restamos 45.f al cálculo para empujar el plano visual de la cámara hacia abajo,
            // permitiendo que el nodo de arriba muestre su mitad superior y sus textos sin cortarse.
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
            auto it = std::find_if(
                layout.begin(),
                layout.end(),
                [&](const VisualNode& n){ return n.node == v.node->left; }
            );

            if(it != layout.end())
            {
                sf::Vertex line[] =
                {
                    sf::Vertex(v.position, sf::Color::Black),
                    sf::Vertex(it->position, sf::Color::Black)
                };

                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }

        if(v.node->right)
        {
            auto it = std::find_if(
                layout.begin(),
                layout.end(),
                [&](const VisualNode& n){ return n.node == v.node->right; }
            );

            if(it != layout.end())
            {
                sf::Vertex line[] =
                {
                    sf::Vertex(v.position, sf::Color::Black),
                    sf::Vertex(it->position, sf::Color::Black)
                };

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
    circle.setPosition(
        v.position - sf::Vector2f(17.f, 20.f)
    );

    // =========================
    // OUTLINE
    // =========================
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

    // =========================
    // NODE COLOR
    // =========================
    if(v.node->color == RED)
        circle.setFillColor(
            sf::Color(200, 50, 50)
        );
    else
        circle.setFillColor(
            sf::Color::Black
        );

    window.draw(circle);

    // =========================
    // KEY (CENTERED)
    // =========================
    sf::Text keyText(font);

    std::ostringstream oss;

    // si no tiene decimal significativo
    if(v.node->key == (int)v.node->key)
    {
        oss << (int)v.node->key;
    }
    else
    {
        oss << std::fixed
            << std::setprecision(1)
            << v.node->key;
    }

    keyText.setString(oss.str());

    keyText.setCharacterSize(9);
    keyText.setFillColor(sf::Color::White);

    auto bounds = keyText.getLocalBounds();

    keyText.setOrigin({
        bounds.position.x + bounds.size.x / 3.f,
        bounds.position.y + bounds.size.y / 3.f
    });

    keyText.setPosition(v.position);

    window.draw(keyText);


    // =========================
    // SUBTREE SIZE (TOP RIGHT)
    // =========================
    sf::Text sizeText(font);

    sizeText.setString(
        "s:" +
        std::to_string(v.node->subtreeSize)
    );

    sizeText.setCharacterSize(9);
    sizeText.setFillColor(sf::Color::Black);

    sizeText.setPosition(
        v.position + sf::Vector2f(26.f, -24.f)
    );

    window.draw(sizeText);
}

void RBVisualizer::setDatabase(const Database& db)
{
    dbRef = const_cast<Database*>(&db);
    dbView->setRecords(db.getAllRecords());
}

void RBVisualizer::setQueryResult(
    const QueryResult& result
)
{
    currentQuery = result;
    hasQuery = true;

    if(!result.visitedNodes.empty())
    {
        currentNode =
            result.visitedNodes.front();
    }

    dbView->setRecords(result.records);

    // Opcional: resaltarlos igual
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

    // Sincronizamos la posición X con la caja de comandos de arriba
    sf::Vector2u windowSize = window.getSize();
    float posX = windowSize.x - 400.f - 20.f;
    float posY = 55.f; // Justo debajo de la caja de comandos

    sf::Text text(font);
    text.setCharacterSize(14); // Un toque más compacto para el bloque
    text.setFillColor(sf::Color(50, 50, 50));

    text.setString(
        "RBT Time: " + std::to_string(currentQuery.rbtTimeUs) + " us\n" +
        "Linear Time: " + std::to_string(currentQuery.linearTimeUs) + " us\n" +
        "Visited Nodes: " + std::to_string(currentQuery.visitedRBT) + "\n" +
        "Linear Checked: " + std::to_string(currentQuery.visitedLinear)
    );

    text.setPosition({posX + 5.f, posY});
    window.draw(text);
}

void RBVisualizer::processCommand(const std::string& cmd)
{
    if(!dbRef) return;

    if(cmd == "reset")
    {
        // quitar estado de consulta
        hasQuery = false;

        // limpiar nodos visitados del árbol
        treeRef->resetVisited();

        // restaurar tabla completa
        dbView->clearHighlights();
        dbView->setRecords(
            dbRef->getAllRecords()
        );

        // volver al root
        currentNode =
            treeRef->getRoot();

        // resetear cámara
        camera =
            window.getDefaultView();

        // recalcular layout
        updateLayout(*treeRef);

        return;
    }

    std::istringstream ss(cmd);
    std::string op;
    ss >> op;
    if(op == "topk")
    {
        int k;

        if(!(ss >> k) || k <= 0)
        {
            commandBuffer =
                "ERROR: use -> topk <positive_int>";
            return;
        }

        auto result =
            QueryBenchmark::topK(*dbRef, k);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "bottomk")
    {
        int k;

        if(!(ss >> k) || k <= 0)
        {
            commandBuffer =
                "ERROR: use -> bottomk <positive_int>";
            return;
        }

        auto result =
            QueryBenchmark::bottomK(*dbRef, k);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "id")
    {
        int id;

        if(!(ss >> id))
        {
            commandBuffer =
                "ERROR: use -> id <int>";
            return;
        }

        auto result =
            QueryBenchmark::searchById(*dbRef, id);

        if(result.records.empty())
        {
            commandBuffer =
                "ID not found";
            return;
        }

        setQueryResult(result);
        hasQuery = true;

        // 🔥 mover al nodo del score
        double score =
            result.records[0].score;

        currentNode =
            treeRef->search(score);

        updateLayout(*treeRef);
    }

    else if(op == "range")
    {
        double a, b;

        if(!(ss >> a >> b))
        {
            commandBuffer =
                "ERROR: use -> range <min> <max>";
            return;
        }

        if(a > b)
            std::swap(a, b);

        auto result =
            QueryBenchmark::rangeSearch(*dbRef, a, b);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "percentile")
    {
        double p;

        if(!(ss >> p))
        {
            commandBuffer =
                "ERROR: use -> percentile <0-100>";
            return;
        }

        if(p < 0 || p > 100)
        {
            commandBuffer =
                "ERROR: percentile must be between 0 and 100";
            return;
        }

        auto result =
            QueryBenchmark::percentile(*dbRef, p);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "median")
    {
        auto result =
            QueryBenchmark::median(*dbRef);

        setQueryResult(result);
        hasQuery = true;
    }

    else if(op == "score")
    {
        double score;

        if(!(ss >> score))
        {
            commandBuffer =
                "ERROR: use -> score <number>";
            return;
        }

        auto result =
            QueryBenchmark::equalitySearch(
                *dbRef,
                score
            );

        if(result.records.empty())
        {
            commandBuffer =
                "No records with that score";
            return;
        }

        setQueryResult(result);
        hasQuery = true;

        currentNode =
            treeRef->search(score);

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
            commandBuffer =
                "ERROR: invalid insert format";

            return;
        }

        Record r{
            id,
            name,
            age,
            score,
            category
        };

        bool inserted =
            dbRef->insertRecord(r);

        if(!inserted)
        {
            commandBuffer =
                "ERROR: ID already exists";

            return;
        }

        commandBuffer =
            "Inserted successfully";

        dbView->setRecords(
            dbRef->getAllRecords()
        );

        currentNode =
            treeRef->search(score);

        updateLayout(*treeRef);

        typingCommand = false;
    }

    else if(op == "delete")
    {
        int id;
        ss >> id;

        Record* r =
            dbRef->findById(id);

        // ERROR: no existe
        if(!r)
        {
            commandBuffer =
                "ERROR: ID not found";

            return;
        }

        double score =
            r->score;

        bool deleted =
            dbRef->deleteRecord(id);

        // ERROR interno
        if(!deleted)
        {
            commandBuffer =
                "ERROR deleting record";

            return;
        }

        // SUCCESS
        commandBuffer =
            "Deleted successfully";

        dbView->setRecords(
            dbRef->getAllRecords()
        );

        // mover foco a root si el score eliminado ya no existe
        RBNode* found =
            treeRef->search(score);

        if(found != treeRef->getNIL())
            currentNode = found;
        else
            currentNode = treeRef->getRoot();

        updateLayout(*treeRef);

        typingCommand = false;
    }


}

void RBVisualizer::drawCommandBox()
{
    // Obtenemos el tamaño actual de la ventana para posicionamiento relativo
    sf::Vector2u windowSize = window.getSize();

    // Dimensiones de la caja
    float boxWidth = 400.f;
    float boxHeight = 30.f;

    // Posición: alineado a la derecha (Ancho ventana - Ancho caja - Margen de 20px)
    float posX = windowSize.x - boxWidth - 40.f;
    float posY = 10.f;

    sf::RectangleShape box({boxWidth, boxHeight});
    box.setPosition({posX, posY});
    box.setFillColor(sf::Color(230, 230, 230));

    // Añadimos un pequeño borde para que resalte más al escribir
    box.setOutlineThickness(typingCommand ? 2.f : 0.f);
    box.setOutlineColor(sf::Color(200, 50, 50)); // Rojo si estás escribiendo

    window.draw(box);

    sf::Text text(font);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Black);

    // Si no está escribiendo, ponemos un texto guía elegante
    if (commandBuffer.empty() && !typingCommand) {
        text.setString("Presiona 'B' para escribir un comando...");
        text.setFillColor(sf::Color(150, 150, 150));
    } else {
        text.setString(commandBuffer);
    }

    text.setPosition({posX + 10.f, posY + 5.f});
    window.draw(text);
}

// Añade este método a tu clase RBVisualizer
void RBVisualizer::drawHelpOverlay()
{
    if (!showHelp) return;

    sf::Vector2u windowSize = window.getSize();
    float boxWidth = 400.f;
    float posX = windowSize.x - boxWidth - 20.f;
    float posY = 150.f; // Posicionado abajo de las estadísticas

    // Fondo del panel de ayuda
    sf::RectangleShape helpBox({boxWidth, 260.f});
    helpBox.setPosition({posX, posY});
    helpBox.setFillColor(sf::Color(245, 245, 245));
    helpBox.setOutlineThickness(1.f);
    helpBox.setOutlineColor(sf::Color(200, 200, 200));
    window.draw(helpBox);

    sf::Text title(font, "GUIA DE CONTROLES (F1 para cerrar)");
    title.setCharacterSize(11);
    title.setStyle(sf::Text::Style::Bold);
    title.setFillColor(sf::Color::White);

    sf::RectangleShape titleBar({boxWidth, 20.f});
    titleBar.setPosition({posX, posY});
    titleBar.setFillColor(sf::Color(80, 80, 80));
    window.draw(titleBar);
    title.setPosition({posX + 5.f, posY + 2.f});
    window.draw(title);

    // Contenido de los comandos
    sf::Text body(font);
    body.setCharacterSize(11);
    body.setFillColor(sf::Color::Black);
    body.setString(
        " CONTROLES DEL VISUALIZADOR:\n"
        "  B : Activar barra / Flechas : Mover camara\n"
        "  W, A, D : Moverse en Arbol (Padre, Izq, Der)\n"
        "  R : Ir a Raiz / MouseWheel : Zoom\n"
        "  O, P : Pagina Anterior / Siguiente DB\n\n"
        " COMANDOS DISPONIBLES (Escribir tras pulsar B):\n"
        "  topk <n>         : Muestra los 'n' mayores scores\n"
        "  bottomk <n>      : Muestra los 'n' menores scores\n"
        "  id <id>          : Busca por ID y enfoca en el Arbol\n"
        "  score <valor>    : Busca score exacto y lo enfoca\n"
        "  range <min> <max>: Registros en rango de scores\n"
        "  percentile <p>   : Registros en percentil (0-100)\n"
        "  median           : Encuentra la mediana de los datos\n"
        "  insert <id> <name> <age> <score> <cat>\n"
        "  delete <id>      : Elimina registro por ID\n"
        "  reset            : Restaura y limpia la vista"
    );
    body.setPosition({posX + 5.f, posY + 25.f});
    window.draw(body);
}