#ifndef Test_HPP
#define Test_HPP

#include <cstdlib>
#include <algorithm>
#include <limits.h>
#include <queue>
#include <utility>
#include <SFML/Graphics.hpp>


const int WALL = 1;
const int PATH = 0;



struct Cell {
    int type;
    char letter = ' ';
};



// Déclarations des fonctions
std::vector<std::string> readWordsFromFile(const std::string& filename);
void generateMaze(std::vector<std::vector<Cell>>& maze, int x, int y);
void buildGraph(const std::vector<std::vector<Cell>>& maze);
void placeWords(std::vector<std::vector<Cell>>& maze, const std::vector<std::string>& words);
std::pair<int, int> findStartPoint(const std::vector<std::vector<Cell>>& maze);
std::pair<int, int> findEndPoint(const std::vector<std::vector<Cell>>& maze);
void displayMaze(const std::vector<std::vector<Cell>>& maze, const std::vector<std::pair<int, int>>& shortestPath, const std::vector<std::string>& validWords);
void startSFMLGame(const std::string& difficulty);  

#endif
