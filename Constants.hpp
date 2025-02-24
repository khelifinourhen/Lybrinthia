#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <SFML/Graphics.hpp>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TAILLE= 15;
const int COUNTDOWN_TIME = 60;

// Colors
const sf::Color WHITE(255, 255, 255);
const sf::Color BLACK(0, 0, 0);
const sf::Color GRAY(128, 128, 128);

// Font
const std::string FONT_PATH = "C:/Windows/Fonts/Arial.ttf";

// Background image
const std::string BACKGROUND_IMAGE_PATH = "C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond d'ecran.jpeg"; 


extern std::vector<std::string> words; // Liste de mots lus du fichier


    
const int CELL_SIZE = 30; // Taille d'une cellule en pixels pour l'affichage SFML
const int FONT_SIZE = 20; // Taille de la police pour l'affichage des lettres
#endif