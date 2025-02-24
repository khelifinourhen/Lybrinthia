#include <SFML/Graphics.hpp>
#include "MainMenu.hpp"
#include "OptionsMenu.hpp"
#include "DifficultyMenu.hpp"
#include "Test.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Labyrinth Game");
    std::string currentDifficulty = "medium"; // Difficulté par défaut

    MainMenu mainMenu(window);
    OptionsMenu optionsMenu(window);
    DifficultyMenu difficultyMenu(window);

    std::string currentState = "main_menu";

    while (window.isOpen()) {
        if (currentState == "main_menu") {
            std::string nextState = mainMenu.run();

            if (nextState == "start_game") {
                currentState = "start_game"; // Conserver l'état du jeu
            }
            else if (nextState == "options") {
                currentState = "options";
            }
            else if (nextState == "quit") {
                window.close();
            }
        }
        else if (currentState == "options") {
            std::string nextState = optionsMenu.run();

            if (nextState == "back") {
                currentState = "main_menu";
            }
            else if (nextState == "difficulty_menu") {
                currentState = "difficulty_menu";
            }
        }
        else if (currentState == "difficulty_menu") {
            std::string nextState = difficultyMenu.run();

            if (nextState == "back") {
                currentState = "options";
            }
            else if (nextState == "easy" || nextState == "medium" || nextState == "hard") {
                currentDifficulty = nextState; // Sauvegarder la difficulté
                currentState = "main_menu"; // Retour au menu principal
                currentState = "start_game";
            }
        }
        else if (currentState == "start_game") {
            // Lancement du jeu avec la difficulté sélectionnée
            std::cout << "Lancement du jeu en difficulte: " << currentDifficulty << std::endl;

            // Appel de la fonction startSFMLGame() pour démarrer le jeu
            startSFMLGame(currentDifficulty);

            // Retour au menu principal après la fin du jeu
            currentState = "main_menu";
        }
    }

    return 0;
}