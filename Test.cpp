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
#include "Constants.hpp"
//# define NULL 0
using namespace std;
int score=0;
// Structure pour stocker les informations d'un nœud dans la file de priorité
struct Node {
    int index;
    int distance;
    bool operator<(const Node& other) const {
        return distance > other.distance; // Pour avoir une file de priorité min
    }
};
std::vector<std::string> words;
// Variables globales
vector<vector<int>> adjacencyMatrix;
map<pair<int, int>, int> positionToIndex;
map<int, pair<int, int>> indexToPosition;

// Fonction pour lire les mots depuis un fichier
vector<string> readWordsFromFile(string& filename) {
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << endl;
        return words;
    }

    vector<string> words;
    string word;
    while (file >> word) {
        // Convertir le mot en minuscules
        
        
        transform(word.begin(), word.end(), word.begin(), ::tolower);
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

// Fonction pour afficher le chemin dans le labyrinthe
void displayPath(vector<vector<Cell>>& maze, const vector<int>& path) {
    for (int index : path) {
        pair<int, int> pos = indexToPosition[index];
        maze[pos.first][pos.second].type = PATH;
    }
}

// Fonction pour placer les mots dans le labyrinthe
void placeWords(vector<vector<Cell>>& maze, const vector<string>& words) {
    int rows = maze.size(), cols = maze[0].size();
    int directions[8][2] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    cerr << " words" << words.size() << endl;

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
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j].type == PATH && maze[i][j].letter == ' ') {
                maze[i][j].letter = 'A' + rand() % 26;

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

// conversion du labyrinthe d'une matrice à un graph
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
                int newDist = dist[current.index] + 1;
                if (newDist < dist[neighbor]) {
                    dist[neighbor] = newDist;
                    prev[neighbor] = current.index;
                    pq.push({ neighbor, newDist });
                }
            }
        }
    }

    // Reconstruire le chemin sous forme d'indices
    vector<int> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

// Fonction pour calculer le score

int calculateScore_words(const vector<FoundWord>& foundWords, const vector<int>& shortestPath, const vector<pair<int, int>>& playerPath) {
   

    for (const auto& foundWord : foundWords) {
        score += foundWord.word.length();
        cout << "visité";
    }

    return score;
}
int calculateScore_path( const vector<int>& shortestPath, const vector<pair<int, int>>& playerPath) {
    
    if (!shortestPath.empty()) {
        cout << shortestPath.empty();
        int shortestPathLength = shortestPath.size();
        int playerPathLength = playerPath.size();

        if (playerPathLength == shortestPathLength) {
            // Si le joueur suit un chemin plus court ou égal au plus court chemin, il reçoit un bonus maximal
            score += 100; // Bonus fixe pour avoir suivi le chemin optimal
        }
        else if (playerPathLength <= shortestPathLength + 5) {
            score += 80; // Bonus légèrement réduit si le chemin est légèrement plus long
        }
        else {
            score += 40; // Bonus minimal si le chemin est significativement plus long
        }
    }
    cout << "score" << score;
    return score;
}
// Fonction pour afficher le message de "Game Over" avec le score
// Fonction pour afficher le message de "Game Over" avec le score
void displayGameOver(sf::RenderWindow& window, const sf::Font& font, int score,
    const vector<FoundWord>& foundWords, const vector<pair<int, int>>& playerPath,
    const vector<int>& shortestPath, bool reachedEnd) {

    // Charger la texture de l'arrière-plan
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond_ecran.jpeg")) {
        std::cerr << "ERREUR: Image de fond introuvable" << std::endl;
        return; // Quitter la fonction si l'image ne peut pas être chargée
    }

    // Créer un sprite pour l'arrière-plan
    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    // Ajuster la taille du sprite pour qu'il corresponde à la taille de la fenêtre
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    // Créer un rectangle pour l'arrière-plan du message avec une bordure
    sf::RectangleShape messageBackground(sf::Vector2f(600, 500));
    messageBackground.setFillColor(sf::Color(0xD0, 0xB9, 0x70, 230)); // Couleur #d0b970 avec opacité
    messageBackground.setOutlineThickness(5);
    messageBackground.setOutlineColor(sf::Color::White);
    messageBackground.setPosition(window.getSize().x / 2 - 300, window.getSize().y / 2 - 250);

    // Texte "Game Over" avec une ombre
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over!");
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getLocalBounds().width / 2, window.getSize().y / 2 - 220);

    // Score final
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score Final: " + to_string(score));
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(window.getSize().x / 2 - scoreText.getLocalBounds().width / 2, window.getSize().y / 2 - 150);

    
    sf::Text congratsText;
    congratsText.setFont(font);
    congratsText.setString("Félicitations !");
    congratsText.setCharacterSize(40);
    congratsText.setFillColor(sf::Color::Cyan);
    congratsText.setStyle(sf::Text::Bold);
    congratsText.setPosition(window.getSize().x / 2 - congratsText.getLocalBounds().width / 2, window.getSize().y / 2 - 50);

    // Animation du texte de félicitations
    sf::Clock animationClock; // Pour gérer l'animation
    bool showCongratsText = true; // Pour faire clignoter le texte

    // Mots collectés
    sf::Text wordsTitle;
    wordsTitle.setFont(font);
    wordsTitle.setString("Mots collectés:");
    wordsTitle.setCharacterSize(30);
    wordsTitle.setFillColor(sf::Color::White);
    wordsTitle.setPosition(window.getSize().x / 2 - 250, window.getSize().y / 2 + 50);

    // Liste des mots
    vector<sf::Text> wordTexts;
    float yOffset = window.getSize().y / 2 + 90;
    for (const auto& word : foundWords) {
        sf::Text wordText;
        wordText.setFont(font);
        wordText.setString("- " + word.word + " (" + to_string(word.word.length()) + " points)");
        wordText.setCharacterSize(20);
        wordText.setFillColor(sf::Color::White);
        wordText.setPosition(window.getSize().x / 2 - 230, yOffset);
        wordTexts.push_back(wordText);
        yOffset += 30;
    }

    // Boutons
    sf::RectangleShape restartButton(sf::Vector2f(200, 50));
    restartButton.setFillColor(sf::Color::Green);
    restartButton.setPosition(window.getSize().x / 2 - 250, window.getSize().y / 2 + 300);

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setString("Redémarrer");
    restartText.setCharacterSize(25);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(restartButton.getPosition().x + 50, restartButton.getPosition().y + 10);

    sf::RectangleShape quitButton(sf::Vector2f(200, 50));
    quitButton.setFillColor(sf::Color::Red);
    quitButton.setPosition(window.getSize().x / 2 + 50, window.getSize().y / 2 + 300);

    sf::Text quitText;
    quitText.setFont(font);
    quitText.setString("Quitter");
    quitText.setCharacterSize(25);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(quitButton.getPosition().x + 60, quitButton.getPosition().y + 10);

    bool isClosed = false;

    while (!isClosed) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                isClosed = true;
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (restartButton.getGlobalBounds().contains(mousePos)) {
                    // Redémarrer le jeu
                    isClosed = true;
                }
                else if (quitButton.getGlobalBounds().contains(mousePos)) {
                    window.close();
                    isClosed = true;
                }
            }
        }

        // Gestion de l'animation du texte de félicitations
        if (reachedEnd && playerPath.size() == shortestPath.size()) {
            if (animationClock.getElapsedTime().asSeconds() > 0.5f) { // Clignotement toutes les 0.5 secondes
                showCongratsText = !showCongratsText; // Inverser l'état du texte
                animationClock.restart(); // Redémarrer le chronomètre
            }
        }

        window.clear();
        window.draw(backgroundSprite); // Dessiner l'arrière-plan
        window.draw(messageBackground); // Dessiner le fond du message
        window.draw(gameOverText);
        window.draw(scoreText);
        
        // Afficher le texte de félicitations si le chemin est optimal
        if (reachedEnd && playerPath.size() == shortestPath.size() && showCongratsText) {
            window.draw(congratsText);
        }

        window.draw(wordsTitle);

        // Dessiner tous les mots collectés
        for (const auto& wordText : wordTexts) {
            window.draw(wordText);
        }

        // Dessiner les boutons
        window.draw(restartButton);
     
        window.draw(congratsText);
        window.draw(restartText);
        window.draw(quitButton);
        window.draw(quitText);

        window.display();
    }
}
void displayMaze(vector<vector<Cell>>& maze, const vector<int>& shortestPath, const std::vector<std::string>& words, const std::string& difficulty) {
    bool followShortestPath = false; // Nouveau flag pour suivre le chemin le plus court
    bool reachedEndPoint = false; // Nouveau flag pour vérifier si le joueur a atteint le point d'arrivée

    displayPath(maze, shortestPath); // Appel de la fonction displayPath
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
    float timeLimit = 160.0f; // 3minutes
    float remainingTime = timeLimit;

    // NEW: Bouton "Passer au chemin"
    sf::RectangleShape togglePathButton(sf::Vector2f(180, 50)); // Taille du bouton
    togglePathButton.setPosition(maze[0].size() * CELL_SIZE + 20, maze.size() * CELL_SIZE - 200); // Position du bouton
    togglePathButton.setFillColor(sf::Color(100, 150, 200)); // Couleur du bouton (bleu clair)

    // Texte du bouton "Passer au chemin"
    sf::Text togglePathButtonText;
    togglePathButtonText.setFont(font); // Utiliser la police chargée
    togglePathButtonText.setString("Passer au chemin"); // Texte du bouton
    togglePathButtonText.setCharacterSize(20); // Taille du texte
    togglePathButtonText.setFillColor(sf::Color::White); // Couleur du texte
    togglePathButtonText.setPosition(maze[0].size() * CELL_SIZE + 30, maze.size() * CELL_SIZE - 190); // Position du texte

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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Fermer la fenêtre si l'événement "Closed" est détecté
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
                    case sf::Keyboard::Z:    dx = -1; dy = 0; break;  // Haut
                    case sf::Keyboard::S:  dx = 1; dy = 0; break;   // Bas
                    case sf::Keyboard::Q:  dx = 0; dy = -1; break;  // Gauche
                    case sf::Keyboard::D: dx = 0; dy = 1; break;   // Droite

                        // Utilisation d'autres touches pour les directions diagonales
                    case sf::Keyboard::A: dx = -1; dy = -1; break;  // Diagonale haut-gauche
                    case sf::Keyboard::E: dx = -1; dy = 1; break;   // Diagonale haut-droite
                    case sf::Keyboard::W: dx = 1; dy = -1; break;   // Diagonale bas-gauche
                    case sf::Keyboard::C: dx = 1; dy = 1; break;    // Diagonale bas-droite

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

            // Gestion du clic sur le bouton "Passer au chemin"
            if (event.type == sf::Event::MouseButtonPressed) {
                if (togglePathButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    followShortestPath = true; // Activer le suivi du chemin le plus court
                    playerPos = findStartPoint(maze); // Rediriger le joueur au point de départ
                    playerPath.clear(); // Réinitialiser le chemin du joueur
                    playerPath.push_back(playerPos); // Ajouter la nouvelle position de départ
                }
            }
        }
        
        // Mise à jour du compte à rebours et du score
        remainingTime = timeLimit - clock.getElapsedTime().asSeconds();
        if (difficulty== "hard") {
        if (remainingTime <= 0) {
            gameOver = true;
            remainingTime = 0;
        }
        }

        // Vérifier si le joueur a atteint le point d'arrivée
        if (playerPos == endPoint) {
            reachedEndPoint = true;
            
            // Le joueur a atteint le point d'arrivée
            gameOver = true; // Terminer le jeu
        }

        // Affichage du message de "Game Over" si le temps est écoulé ou si le joueur a atteint la fin
        if (gameOver) {
            score = calculateScore_words(foundWords, shortestPath, playerPath);
            score = calculateScore_path(shortestPath, playerPath);
            displayGameOver(window, font, score, foundWords, playerPath, shortestPath, reachedEndPoint);

            break; // Sortir de la boucle principale
        }

        // Effacer la fenêtre une seule fois au début de la boucle de rendu
        window.clear(sf::Color::White);

        // Dessiner le labyrinthe
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
                    text.setFillColor(sf::Color::Black);
                    text.setPosition(j * CELL_SIZE + 5, i * CELL_SIZE + 5);
                    window.draw(text);
                }
            }
        }

        // Dessiner le chemin du joueur
        for (auto& pos : playerPath) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(pos.second * CELL_SIZE, pos.first * CELL_SIZE);
            if (followShortestPath) {
                cell.setFillColor(sf::Color(255, 0, 0, 128)); // Rouge semi-transparent pour le chemin suivi
            }
            else if (find(currentWordPath.begin(), currentWordPath.end(), pos) != currentWordPath.end()) {
                cell.setFillColor(sf::Color(0, 128, 0, 128)); // Vert foncé semi-transparent
            }
            else {
                cell.setFillColor(sf::Color(0, 255, 0, 128)); // Vert clair semi-transparent
            }
            window.draw(cell);
        }

        // Dessiner la cellule actuelle du joueur
        sf::RectangleShape currentCell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        currentCell.setPosition(playerPos.second * CELL_SIZE, playerPos.first * CELL_SIZE);
        currentCell.setFillColor(sf::Color(144, 238, 144, 200)); // Vert clair plus visible
        window.draw(currentCell);

        // Dessiner le chemin le plus court uniquement si le joueur a atteint le point d'arrivée
        if (reachedEndPoint) {
            for (auto& pos : shortestPath) {
                pair<int, int> posPair = indexToPosition[pos];
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(posPair.second * CELL_SIZE, posPair.first * CELL_SIZE);
                cell.setFillColor(sf::Color(0, 0, 255, 128)); // Bleu semi-transparent
                window.draw(cell);
            }
        }

        // Dessiner la cellule de fin
        sf::RectangleShape endCell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        endCell.setPosition(endPoint.second * CELL_SIZE, endPoint.first * CELL_SIZE);
        endCell.setFillColor(sf::Color::Blue);
        window.draw(endCell);

        // Dessiner l'image de fond (si nécessaire)
        sf::Texture backgroundTexture;
        if (backgroundTexture.loadFromFile("C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond_ecran.jpeg")) {
            sf::Sprite backgroundSprite(backgroundTexture);
            backgroundSprite.setPosition(maze[0].size() * CELL_SIZE, 0);
            window.draw(backgroundSprite);
        }

        // Dessiner le score et les mots trouvés
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

        // Dessiner le mot actuel
        sf::Text currentWordText;
        currentWordText.setFont(font);
        currentWordText.setString("Mot actuel : " + currentWord);
        currentWordText.setCharacterSize(16);
        currentWordText.setFillColor(sf::Color::Black);
        currentWordText.setPosition(maze[0].size() * CELL_SIZE + 10, maze.size() * CELL_SIZE - 100); // Ajusté pour l'espacement
        window.draw(currentWordText);

        // Dessiner le temps restant uniquement en mode difficile
        if (difficulty == "hard") {
            sf::Text timeText;
            timeText.setFont(font);
            timeText.setString("Temps restant : " + to_string((int)remainingTime) + "s");
            timeText.setCharacterSize(16);
            timeText.setFillColor(sf::Color::Black);
            timeText.setPosition(maze[0].size() * CELL_SIZE + 10, maze.size() * CELL_SIZE - 70); // Ajusté pour l'espacement
            window.draw(timeText);
        }

        // Dessiner le bouton "Passer au chemin"
        window.draw(togglePathButton);
        window.draw(togglePathButtonText);
        // NEW: Draw the back button.
        window.draw(backButton);
        window.draw(backButtonText);

        // Afficher la fenêtre
        window.display();
    }
}
void startSFMLGame(const std::string& difficulty) {
    srand(time(0));
    // Initialisation du générateur de nombres aléatoires

    // Déterminer le fichier de mots en fonction de la difficulté
    std::string wordFile;
    if (difficulty == "easy") {
        wordFile = "facile.txt";
        cerr << " le texte de facile est lu !" << wordFile << endl;
    }
    else if (difficulty == "medium") {
        wordFile = "C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/medium.txt";
        cerr << " le texte de medium est lu !" << endl;
    }
    else if (difficulty == "hard") {
        wordFile = "C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/diff.txt";
        cerr << " le texte de  difficile est lu !" << endl;
    }
    else {
        wordFile = "C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/medium.txt"; // Par défaut
    }

    // Lecture des mots depuis le fichier
    vector<string> words = readWordsFromFile(wordFile);


   

    // Initialisation du labyrinthe
    
    int rows = 20, cols = 20;
    vector<vector<Cell>> maze(rows, vector<Cell>(cols, { WALL, ' ' }));

    // Génération du labyrinthe
    maze[1][1].type = PATH;
    generateMaze(maze, 1, 1);

    // Construction du graphe à partir du labyrinthe
    buildGraph(maze);

    // Placement des mots dans le labyrinthe
    placeWords(maze, words);

    // Trouver le point de départ et d'arrivée
    pair<int, int> startPoint = findStartPoint(maze);
    int start = positionToIndex[startPoint];

    pair<int, int> endPoint = findEndPoint(maze);
    int end = positionToIndex[endPoint];

    // Calcul du chemin le plus court avec Dijkstra
    vector<int> shortestPath = dijkstra(start, end);

    // Affichage du labyrinthe et lancement de la boucle SFML
    displayMaze(maze, shortestPath, words, difficulty);

}