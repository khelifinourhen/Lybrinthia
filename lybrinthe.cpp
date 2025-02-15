#include <SFML/Graphics.hpp>
#include "MainMenu.hpp"
#include "OptionsMenu.hpp"
#include "DifficultyMenu.hpp"
#include <iostream>

int main() {
    // Créer la fenêtre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Labyrinth Game");

    // Créer les menus
    MainMenu mainMenu(window);
    OptionsMenu optionsMenu(window);
    DifficultyMenu difficultyMenu(window);

    // État initial du programme
    std::string currentState = "main_menu";

    // Boucle principale du programme
    while (window.isOpen()) {
        if (currentState == "main_menu") {
            // Exécuter le menu principal
            std::string nextState = mainMenu.run();

            if (nextState == "quit") {
                window.close(); // Quitter le programme
            }
            else {
                currentState = nextState; // Changer d'état (par exemple, aller aux options)
            }
        }
        else if (currentState == "options") {
            // Exécuter le menu des options
            std::string nextState = optionsMenu.run();

            if (nextState == "back") {
                currentState = "main_menu"; // Retour au menu principal
            }
            else if (nextState == "difficulty_menu") {
                currentState = "difficulty_menu"; // Aller au menu de difficulté
            }
        }
        else if (currentState == "difficulty_menu") {
            // Exécuter le menu de difficulté
            std::string nextState = difficultyMenu.run();

            if (nextState == "back") {
                currentState = "options"; // Retour au menu des options
            }
            else if (nextState == "easy" || nextState == "medium" || nextState == "hard") {
                // Gérer la sélection de la difficulté
                std::cout << "Difficulté sélectionnée : " << nextState << std::endl;
                currentState = "main_menu"; // Retour au menu principal (ou démarrer le jeu)
            }
        }
        else if (currentState == "start_game") {
            // Démarrer le jeu (à implémenter)
            std::cout << "Démarrage du jeu..." << std::endl;
            currentState = "main_menu"; // Retour au menu principal après la fin du jeu
        }
    }

    return 0;
}