#include "Test.hpp"
#include <cstdlib>
#include <algorithm>
#include <limits.h>
#include <queue>
#include <utility>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <ctime>
#include "OptionsMenu.hpp"
#include "mainMenu.hpp"
#include "DifficultyMenu.hpp"
#include "Constants.hpp"

using namespace std;

// Structure pour stocker les informations d'un nœud dans la file de priorité
struct Node {
    int index;
    int distance;
    bool operator<(const Node& other) const {
        return distance > other.distance; // Pour avoir une file de priorité min
    }
};

// Variables globales
vector<vector<int>> adjacencyMatrix;
map<pair<int, int>, int> positionToIndex;
map<int, pair<int, int>> indexToPosition;

// Fonction pour lire les mots depuis un fichier
vector<string> readWordsFromFile(const string& filename) {
    ifstream file(filename.c_str());
    vector<string> words;
    string word;
    while (file >> word) {
        words.push_back(word);
    }
    return words;
}

// Fonction pour mélanger les directions
void shuffleDirections(int directions[8][2]) {
    for (int i = 7; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(directions[i], directions[j]);
    }
}

// Fonction pour placer les mots dans le labyrinthe
void placeWords(vector<vector<Cell>>& maze, const vector<string>& words) {
    int rows = maze.size(), cols = maze[0].size();
    int directions[8][2] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (const string& word : words) {
        bool placed = false;
        for (int attempt = 0; attempt < 100 && !placed; attempt++) {
            int x = rand() % rows;
            int y = rand() % cols;
            if (maze[x][y].type != PATH || maze[x][y].letter != ' ') continue;

            shuffleDirections(directions);
            for (int i = 0; i < 8 && !placed; i++) {
                int nx = x, ny = y;
                bool canPlace = true;
                for (char c : word) {
                    if (nx < 0 || ny < 0 || nx >= rows || ny >= cols || maze[nx][ny].type != PATH || (maze[nx][ny].letter != ' ' && maze[nx][ny].letter != c)) {
                        canPlace = false;
                        break;
                    }
                    nx += directions[i][0];
                    ny += directions[i][1];
                }
                if (canPlace) {
                    nx = x;
                    ny = y;
                    for (char c : word) {
                        maze[nx][ny].letter = c;
                        nx += directions[i][0];
                        ny += directions[i][1];
                    }
                    placed = true;
                }
            }
        }
    }
}

// Fonction pour générer le labyrinthe
void generateMaze(vector<vector<Cell>>& maze, int x, int y) {
    int directions[8][2] = {
        {0, -2}, {0, 2}, {-2, 0}, {2, 0},
        {-2, -2}, {-2, 2}, {2, -2}, {2, 2}
    };
    shuffleDirections(directions);

    for (int i = 0; i < 8; ++i) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        if (nx > 0 && nx < maze.size() - 1 && ny > 0 && ny < maze[0].size() - 1 && maze[nx][ny].type == WALL) {
            maze[nx][ny].type = PATH;
            maze[x + directions[i][0] / 2][y + directions[i][1] / 2].type = PATH;
            generateMaze(maze, nx, ny);
        }
    }
}

// Fonction pour construire le graphe
void buildGraph(const vector<vector<Cell>>& maze) {
    int rows = maze.size();
    int cols = maze[0].size();
    int nodeCount = 0;
    adjacencyMatrix = vector<vector<int>>(rows * cols, vector<int>(rows * cols, 0));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (maze[i][j].type == PATH) {
                positionToIndex[{i, j}] = nodeCount;
                indexToPosition[nodeCount] = { i, j };
                nodeCount++;
            }
        }
    }

    int directions[8][2] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (auto& entry : positionToIndex) {
        int x = entry.first.first;
        int y = entry.first.second;
        int index = entry.second;

        for (int d = 0; d < 8; ++d) {
            int nx = x + directions[d][0];
            int ny = y + directions[d][1];

            if (positionToIndex.count({ nx, ny })) {
                int neighborIndex = positionToIndex[{nx, ny}];
                adjacencyMatrix[index][neighborIndex] = 1;
            }
        }
    }
}

// Fonction pour trouver le point de départ
pair<int, int> findStartPoint(const vector<vector<Cell>>& maze) {
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze[0].size(); ++j) {
            if (maze[i][j].type == PATH) {
                return { i, j };
            }
        }
    }
    return { -1, -1 };
}

// Fonction pour trouver le point d'arrivée
pair<int, int> findEndPoint(const vector<vector<Cell>>& maze) {
    for (int i = maze.size() - 1; i >= 0; --i) {
        for (int j = maze[0].size() - 1; j >= 0; --j) {
            if (maze[i][j].type == PATH) {
                return { i, j };
            }
        }
    }
    return { -1, -1 };
}

// Structure pour stocker les mots trouvés
struct FoundWord {
    string word;
    vector<pair<int, int>> path;
};

// Fonction pour vérifier si un mot est dans la liste des mots valides
bool isValidWord(const string& word, const vector<string>& validWords) {
    return find(validWords.begin(), validWords.end(), word) != validWords.end();
}

// Fonction pour obtenir le mot formé par un chemin
string getWordFromPath(const vector<pair<int, int>>& path, const vector<vector<Cell>>& maze) {
    string word;
    for (const auto& pos : path) {
        word += maze[pos.first][pos.second].letter;
    }
    return word;
}

// Fonction pour rechercher les mots pendant le parcours
vector<FoundWord> findWords(const pair<int, int>& currentPos,
    const vector<vector<Cell>>& maze,
    const vector<string>& validWords) {
    vector<FoundWord> foundWords;
    vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));

    // Directions possibles (8 directions)
    int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    // File pour le BFS
    queue<pair<pair<int, int>, vector<pair<int, int>>>> q;
    q.push({ currentPos, {currentPos} });
    visited[currentPos.first][currentPos.second] = true;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        string currentWord = getWordFromPath(current.second, maze);

        // Vérifier si le mot actuel est valide
        if (currentWord.length() >= 3 && isValidWord(currentWord, validWords)) {
            foundWords.push_back({ currentWord, current.second });
        }

        // Explorer les voisins
        for (int i = 0; i < 8; i++) {
            int newX = current.first.first + dx[i];
            int newY = current.first.second + dy[i];

            if (newX >= 0 && newX < maze.size() &&
                newY >= 0 && newY < maze[0].size() &&
                maze[newX][newY].type == PATH &&
                !visited[newX][newY]) {

                vector<pair<int, int>> newPath = current.second;
                newPath.push_back({ newX, newY });

                q.push({ {newX, newY}, newPath });
                visited[newX][newY] = true;
            }
        }
    }

    return foundWords;
}

// Fonction pour appliquer l'algorithme de Dijkstra
vector<int> dijkstra(int start, int end) {
    int n = adjacencyMatrix.size();
    vector<int> dist(n, INT_MAX);
    vector<int> prev(n, -1);
    dist[start] = 0;

    priority_queue<Node> pq;
    pq.push({ start, 0 });

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.index == end) break;

        for (int neighbor = 0; neighbor < n; ++neighbor) {
            if (adjacencyMatrix[current.index][neighbor] == 1) {
                int newDist = dist[current.index] + 1; // Poids de l'arête est 1
                if (newDist < dist[neighbor]) {
                    dist[neighbor] = newDist;
                    prev[neighbor] = current.index;
                    pq.push({ neighbor, newDist });
                }
            }
        }
    }

    // Récupération du chemin
    vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    // Si le chemin trouvé est vide ou commence par un autre nœud que start, il n'existe pas
    if (path.empty() || path[0] != start) {
        return {};
    }

    return path;
}

// Fonction pour afficher le chemin dans le labyrinthe
void displayPath(vector<vector<Cell>>& maze, const vector<int>& path) {
    for (int index : path) {
        pair<int, int> pos = indexToPosition[index];
        maze[pos.first][pos.second].letter = '*'; // Marquer le chemin avec '*'
    }
}

// Fonction pour calculer le score
int calculateScore(const vector<FoundWord>& foundWords, const vector<int>& shortestPath, const vector<pair<int, int>>& playerPath) {
    int score = 0;

    // 1. Ajouter la longueur des mots trouvés au score
    for (const auto& foundWord : foundWords) {
        score += foundWord.word.length(); // Chaque lettre du mot rapporte 1 point
    }

    // 2. Calculer un bonus en fonction du chemin parcouru par rapport au plus court chemin
    if (!shortestPath.empty()) {
        // Calculer le ratio entre la longueur du chemin du joueur et celle du plus court chemin
        float pathRatio = static_cast<float>(playerPath.size()) / shortestPath.size();

        // Si le joueur suit un chemin plus long, le ratio sera supérieur à 1
        // On peut ajuster le bonus en fonction de ce ratio
        if (pathRatio <= 1.0f) {
            // Si le joueur suit un chemin plus court ou égal au plus court chemin, il reçoit un bonus maximal
            score += 100; // Bonus fixe pour avoir suivi le chemin optimal
        }
        else {
            // Sinon, le bonus diminue proportionnellement à l'écart par rapport au plus court chemin
            score += static_cast<int>(100 / pathRatio); // Bonus réduit
        }
    }

    return score;
}

// Fonction pour afficher le message de "Game Over" avec le score
void displayGameOver(sf::RenderWindow& window, const sf::Font& font, int score) {
    // Créer un rectangle pour l'arrière-plan du message
    sf::RectangleShape background(sf::Vector2f(400, 200));
    background.setFillColor(sf::Color(0, 0, 0, 200)); // Noir semi-transparent
    background.setPosition(window.getSize().x / 2 - 200, window.getSize().y / 2 - 100);

    // Créer le texte "Game Over"
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(window.getSize().x / 2 - 120, window.getSize().y / 2 - 70);

    // Créer le texte du score
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + to_string(score));
    scoreText.setCharacterSize(40);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(window.getSize().x / 2 - 80, window.getSize().y / 2);

    // Afficher les éléments
    window.clear();
    window.draw(background);
    window.draw(gameOverText);
    window.draw(scoreText);
    window.display();

    // Attendre 3 secondes avant de fermer la fenêtre
    sf::sleep(sf::seconds(3));
    window.close();
}

void displayMaze(vector<vector<Cell>>& maze, const vector<int>& path, const vector<string>& words) {
    displayPath(maze, path); // Appel de la fonction displayPath
    sf::RenderWindow window(sf::VideoMode(maze[0].size() * CELL_SIZE + 200, maze.size() * CELL_SIZE), "Labyrinthe SFML");
    sf::Font font;
    if (!font.loadFromFile(FONT_PATH)) {
        cerr << "Erreur : Impossible de charger la police !" << endl;
        return;
    }

    pair<int, int> playerPos = findStartPoint(maze);
    vector<pair<int, int>> playerPath = { playerPos };
    vector<FoundWord> foundWords;
    pair<int, int> endPoint = findEndPoint(maze);
    bool gameOver = false;
    bool showShortestPath = false;

    // The current word and its path
    string currentWord;
    vector<pair<int, int>> currentWordPath;

    // NEW: Flag for toggling word collection
    bool collectingWord = false;

    // NEW: Compte à rebours
    sf::Clock clock;
    float timeLimit = 120.0f; // 2 minutes
    float remainingTime = timeLimit;

    // NEW: Bouton de retour
    sf::RectangleShape backButton(sf::Vector2f(100, 50));
    backButton.setPosition(maze[0].size() * CELL_SIZE + 50, maze.size() * CELL_SIZE - 150);
    backButton.setFillColor(sf::Color(208, 185, 112));

    sf::Text backButtonText;
    backButtonText.setFont(font);
    backButtonText.setString("Retour");
    backButtonText.setCharacterSize(20);
    backButtonText.setFillColor(sf::Color::White);
    backButtonText.setPosition(maze[0].size() * CELL_SIZE + 60, maze.size() * CELL_SIZE - 140);

    // NEW: Score
    int score = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // NEW: Gestion du bouton de retour
            if (event.type == sf::Event::MouseButtonPressed) {
                if (backButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    window.close();
                    // Ajouter ici le code pour retourner au menu principal
                }
            }

            // Only handle key events if game is not over
            if (!gameOver && event.type == sf::Event::KeyPressed) {

                // When Enter is pressed, toggle word collection.
                if (event.key.code == sf::Keyboard::Enter) {
                    if (!collectingWord) {
                        // Start collecting a new word.
                        collectingWord = true;
                        // (Optional) Reset any previous word:
                        currentWord.clear();
                        currentWordPath.clear();
                        // Optionally, start with the letter of the current cell:
                        currentWord += maze[playerPos.first][playerPos.second].letter;
                        currentWordPath.push_back(playerPos);
                    }
                    else {
                        // Finish word collection: validate and reset.
                        if (isValidWord(currentWord, words)) {
                            auto it = find_if(foundWords.begin(), foundWords.end(),
                                [&currentWord](const FoundWord& fw) { return fw.word == currentWord; });
                            if (it == foundWords.end()) {
                                foundWords.push_back({ currentWord, currentWordPath });
                                cout << "Nouveau mot trouvé : " << currentWord << endl;
                            }
                        }
                        // Reset collection variables and flag.
                        currentWord.clear();
                        currentWordPath.clear();
                        collectingWord = false;
                    }
                }
                else {
                    // Handle movement keys.
                    int dx = 0, dy = 0;
                    switch (event.key.code) {
                        // Utilisation des touches fléchées pour les directions de base
                    case sf::Keyboard::Up:    dx = -1; dy = 0; break;  // Haut
                    case sf::Keyboard::Down:  dx = 1; dy = 0; break;   // Bas
                    case sf::Keyboard::Left:  dx = 0; dy = -1; break;  // Gauche
                    case sf::Keyboard::Right: dx = 0; dy = 1; break;   // Droite

                        // Utilisation d'autres touches pour les directions diagonales
                    case sf::Keyboard::Q: dx = -1; dy = -1; break;  // Diagonale haut-gauche
                    case sf::Keyboard::E: dx = -1; dy = 1; break;   // Diagonale haut-droite
                    case sf::Keyboard::A: dx = 1; dy = -1; break;   // Diagonale bas-gauche
                    case sf::Keyboard::D: dx = 1; dy = 1; break;    // Diagonale bas-droite

                    default: break;
                    }

                    int nx = playerPos.first + dx;
                    int ny = playerPos.second + dy;

                    // Check for valid move within the maze bounds and on a PATH cell.
                    if (nx >= 0 && nx < maze.size() && ny >= 0 && ny < maze[0].size() && maze[nx][ny].type == PATH) {
                        playerPos = { nx, ny };
                        playerPath.push_back(playerPos);

                        // If word collection is active, add the letter of the new cell.
                        if (collectingWord) {
                            currentWord += maze[playerPos.first][playerPos.second].letter;
                            currentWordPath.push_back(playerPos);
                        }
                    }
                }
            }

            // Game over handling (unchanged)
            if (gameOver && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (playerPath.size() == path.size()) {
                        cout << "Félicitations ! Vous avez trouvé le chemin le plus court !" << endl;
                    }
                    else {
                        cout << "Vous n'avez pas trouvé le chemin le plus court. Essayez encore !" << endl;
                        showShortestPath = true;
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        // NEW: Mise à jour du compte à rebours
        remainingTime = timeLimit - clock.getElapsedTime().asSeconds();
        if (remainingTime <= 0) {
            gameOver = true;
            remainingTime = 0;
        }

        // NEW: Calcul du score
        score = calculateScore(foundWords, path, playerPath);

        // NEW: Affichage du message de "Game Over" si le temps est écoulé
        if (gameOver) {
            displayGameOver(window, font, score); // Afficher le message de fin de jeu avec le score
            break; // Sortir de la boucle principale
        }

        window.clear(sf::Color::White);

        // Draw the maze.
        for (int i = 0; i < maze.size(); ++i) {
            for (int j = 0; j < maze[0].size(); ++j) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color::Black);

                if (maze[i][j].type == WALL) {
                    cell.setFillColor(sf::Color::Black);
                }
                else {
                    cell.setFillColor(sf::Color::White);
                }

                window.draw(cell);

                if (maze[i][j].letter != ' ') {
                    sf::Text text;
                    text.setFont(font);
                    text.setString(maze[i][j].letter);
                    text.setCharacterSize(FONT_SIZE);
                    text.setFillColor(sf::Color::Red);
                    text.setPosition(j * CELL_SIZE + 5, i * CELL_SIZE + 5);
                    window.draw(text);
                }
            }
        }

        // Draw the player's path.
        for (auto& pos : playerPath) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(pos.second * CELL_SIZE, pos.first * CELL_SIZE);
            // If the cell is part of the current word path, draw a darker green.
            if (find(currentWordPath.begin(), currentWordPath.end(), pos) != currentWordPath.end()) {
                cell.setFillColor(sf::Color(0, 128, 0, 128)); // Dark green semi-transparent
            }
            else {
                cell.setFillColor(sf::Color(0, 255, 0, 128)); // Light green semi-transparent
            }
            window.draw(cell);
        }

        // Highlight the current cell.
        sf::RectangleShape currentCell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        currentCell.setPosition(playerPos.second * CELL_SIZE, playerPos.first * CELL_SIZE);
        currentCell.setFillColor(sf::Color(144, 238, 144, 200)); // More visible light green
        window.draw(currentCell);

        // Draw the shortest path if needed.
        if (showShortestPath) {
            for (auto& pos : path) {
                pair<int, int> posPair = indexToPosition[pos];
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(posPair.second * CELL_SIZE, posPair.first * CELL_SIZE);
                cell.setFillColor(sf::Color(0, 0, 255, 128)); // Blue semi-transparent
                window.draw(cell);
            }
        }

        // Draw the end cell.
        sf::RectangleShape endCell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        endCell.setPosition(endPoint.second * CELL_SIZE, endPoint.first * CELL_SIZE);
        endCell.setFillColor(sf::Color::Blue);
        window.draw(endCell);

        // Charger l'image de fond
        sf::Texture backgroundTexture;
        if (!backgroundTexture.loadFromFile("C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond_ecran.jpeg")) { // Remplacez par le chemin de votre image
            cerr << "Erreur : Impossible de charger l'image de fond !" << endl;
            return;
        }
        sf::Sprite backgroundSprite(backgroundTexture);
        backgroundSprite.setPosition(maze[0].size() * CELL_SIZE, 0); // Positionner l'image à droite du labyrinthe

        // Dessiner l'image de fond
        window.draw(backgroundSprite);

        // Display score and found words.
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + to_string(score));
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(maze[0].size() * CELL_SIZE + 10, 10);
        window.draw(scoreText);

        float yPos = 50;
        for (const auto& word : foundWords) {
            sf::Text wordText;
            wordText.setFont(font);
            wordText.setString(word.word);
            wordText.setCharacterSize(16);
            wordText.setFillColor(sf::Color::White);
            wordText.setPosition(maze[0].size() * CELL_SIZE + 10, yPos);
            window.draw(wordText);
            yPos += 25;
        }

        // Display the current word.
        sf::Text currentWordText;
        currentWordText.setFont(font);
        currentWordText.setString("Mot actuel : " + currentWord);
        currentWordText.setCharacterSize(16);
        currentWordText.setFillColor(sf::Color::Black);
        currentWordText.setPosition(maze[0].size() * CELL_SIZE + 10, maze.size() * CELL_SIZE - 80);
        window.draw(currentWordText);

        // NEW: Display the remaining time.
        sf::Text timeText;
        timeText.setFont(font);
        timeText.setString("Temps restant : " + to_string((int)remainingTime) + "s");
        timeText.setCharacterSize(16);
        timeText.setFillColor(sf::Color::Black);
        timeText.setPosition(maze[0].size() * CELL_SIZE + 10, maze.size() * CELL_SIZE - 50);
        window.draw(timeText);

        // NEW: Draw the back button.
        window.draw(backButton);
        window.draw(backButtonText);

        window.display();
    }
}

// Fonction principale pour démarrer le jeu
void startSFMLGame() {
    srand(time(0));
    vector<string> words = readWordsFromFile("C:/Users/hp/Desktop/lybrinthe/lybrinthe/Ressources/doc.txt");

    int rows = 20, cols = 20;
    vector<vector<Cell>> maze(rows, vector<Cell>(cols, { WALL, ' ' }));

    maze[1][1].type = PATH;
    generateMaze(maze, 1, 1);
    buildGraph(maze);
    placeWords(maze, words);

    pair<int, int> startPoint = findStartPoint(maze);
    int start = positionToIndex[startPoint];

    pair<int, int> endPoint = findEndPoint(maze);
    int end = positionToIndex[endPoint];

    vector<int> shortestPath = dijkstra(start, end);

    displayMaze(maze, shortestPath, words);
}