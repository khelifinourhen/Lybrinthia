#include "OptionsMenu.hpp"
#include "Constants.hpp"
#include <iostream>

OptionsMenu::OptionsMenu(sf::RenderWindow& window) : window(window) {
    // Charger la police
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Erreur : Impossible de charger la police." << std::endl;
    }
    if (!backgroundTexture.loadFromFile("C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond_ecran.jpeg")) {
        std::cerr << "Erreur : Impossible de charger l'image de fond." << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale(scaleX, scaleY);

    // Configurer les textes des boutons avec la même taille que MainMenu
    difficultyText.setFont(font);
    difficultyText.setString("Difficulty");
    difficultyText.setCharacterSize(50); // Taille de police identique à MainMenu
    difficultyText.setFillColor(WHITE);
    difficultyText.setPosition(SCREEN_WIDTH / 2 - difficultyText.getLocalBounds().width / 2, 200);

    backText.setFont(font);
    backText.setString("Back");
    backText.setCharacterSize(50); // Taille de police identique à MainMenu
    backText.setFillColor(WHITE);
    backText.setPosition(SCREEN_WIDTH / 2 - backText.getLocalBounds().width / 2, 300); // Ajuster la position verticale
}

std::string OptionsMenu::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return "quit";
            }

            // Gestion des clics de souris
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // Récupérer la position de la souris
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Vérifier si la souris est sur le bouton "Difficulty"
                    if (difficultyText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        std::cout << "Bouton Difficulty cliqué" << std::endl;
                        return "difficulty_menu";
                    }

                    // Vérifier si la souris est sur le bouton "Back"
                    if (backText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        std::cout << "Bouton Back cliqué" << std::endl;
                        return "back";
                    }
                }
            }
        }

        draw();
        window.display();
    }
    return "quit";
}

void OptionsMenu::draw() {
    window.clear(BLACK);

    // Afficher les textes des boutons
    window.draw(backgroundSprite);
    window.draw(difficultyText);
    window.draw(backText);
}