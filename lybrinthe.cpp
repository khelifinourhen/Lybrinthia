#include <SFML/Graphics.hpp>
#include "MainMenu.hpp"
#include "OptionsMenu.hpp"
#include "DifficultyMenu.hpp"
#include <iostream>

int main() {
    // Cr�er la fen�tre SFML
    sf::RenderWindow window(sf::VideoMode(800, 600), "Labyrinth Game");

    // Cr�er les menus
    MainMenu mainMenu(window);
    OptionsMenu optionsMenu(window);
    DifficultyMenu difficultyMenu(window);

    // �tat initial du programme
    std::string currentState = "main_menu";

    // Boucle principale du programme
    while (window.isOpen()) {
        if (currentState == "main_menu") {
            // Ex�cuter le menu principal
            std::string nextState = mainMenu.run();

            if (nextState == "quit") {
                window.close(); // Quitter le programme
            }
            else {
                currentState = nextState; // Changer d'�tat (par exemple, aller aux options)
            }
        }
        else if (currentState == "options") {
            // Ex�cuter le menu des options
            std::string nextState = optionsMenu.run();

            if (nextState == "back") {
                currentState = "main_menu"; // Retour au menu principal
            }
            else if (nextState == "difficulty_menu") {
                currentState = "difficulty_menu"; // Aller au menu de difficult�
            }
        }
        else if (currentState == "difficulty_menu") {
            // Ex�cuter le menu de difficult�
            std::string nextState = difficultyMenu.run();

            if (nextState == "back") {
                currentState = "options"; // Retour au menu des options
            }
            else if (nextState == "easy" || nextState == "medium" || nextState == "hard") {
                // G�rer la s�lection de la difficult�
                std::cout << "Difficult� s�lectionn�e : " << nextState << std::endl;
                currentState = "main_menu"; // Retour au menu principal (ou d�marrer le jeu)
            }
        }
        else if (currentState == "start_game") {
            // D�marrer le jeu (� impl�menter)
            std::cout << "D�marrage du jeu..." << std::endl;
            currentState = "main_menu"; // Retour au menu principal apr�s la fin du jeu
        }
    }

    return 0;
}