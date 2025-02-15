#include "DifficultyMenu.hpp"
#include "Constants.hpp"
#include <iostream>
#include "mainMenu.hpp"
#include "OptionsMenu.hpp"


DifficultyMenu::DifficultyMenu(sf::RenderWindow& window)
    : window(window)  // Correctement initialisé ici
{

    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Erreur : Impossible de charger la police." << std::endl;
    }

    // Charger l'image de fond
    if (!backgroundTexture.loadFromFile("C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond_ecran.jpeg")) {
        std::cerr << "Erreur : Impossible de charger l'image de fond." << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Redimensionner l'image de fond pour qu'elle remplisse la fenêtre
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale(scaleX, scaleY);

    // Configuration des boutons
    setupText(easyText, "Easy", 200);
    setupText(mediumText, "Medium", 250);
    setupText(hardText, "Hard", 300);
    setupText(backText, "Back", 350);
}

void DifficultyMenu::setupText(sf::Text& text, const std::string& str, float yPosition) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setPosition(SCREEN_WIDTH / 2 - text.getLocalBounds().width / 2, yPosition);
}

std::string DifficultyMenu::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return "quit";
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (easyText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    std::cout << "Bouton Easy cliqué" << std::endl;
                    return "easy";
                }
                if (mediumText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    std::cout << "Bouton Medium cliqué" << std::endl;
                    return "medium";
                }
                if (hardText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    std::cout << "Bouton Hard cliqué" << std::endl;
                    return "hard";
                }
                if (backText.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    std::cout << "Bouton Back cliqué" << std::endl;
                    return "back";
                }
            }
        }

        draw();
        window.display();
    }
    return "quit";
}

void DifficultyMenu::draw() {
  
    window.draw(backgroundSprite);
    window.draw(easyText);
    window.draw(mediumText);
    window.draw(hardText);
    window.draw(backText);
    
}
