#include "DifficultyMenu.hpp"
#include "Constants.hpp"
#include <iostream>

DifficultyMenu::DifficultyMenu(sf::RenderWindow& window) : window(window) {
    // Charger la police
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "ERREUR: Police introuvable a: " << FONT_PATH << std::endl;
    }

    // Charger l'arrière-plan
    if (!backgroundTexture.loadFromFile("C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond_ecran.jpeg")) { // Chemin relatif
        std::cerr << "ERREUR: Image de fond introuvable" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Ajustement de la taille
    backgroundSprite.setScale(
        window.getSize().x / backgroundSprite.getLocalBounds().width,
        window.getSize().y / backgroundSprite.getLocalBounds().height
    );

    // Configuration des textes avec la même taille que MainMenu
    setupText(easyText, "Facile", 200);
    setupText(mediumText, "Moyen", 300);
    setupText(hardText, "Difficile", 400);
    setupText(backText, "Retour", 500);
}

void DifficultyMenu::setupText(sf::Text& text, const std::string& str, float yPos) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(50); 
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(
        (window.getSize().x - text.getLocalBounds().width) / 2,
        yPos
    );
}

std::string DifficultyMenu::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return "quit";
            }

            // Gestion des clics souris
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(
                    sf::Mouse::getPosition(window)
                );

                if (easyText.getGlobalBounds().contains(mousePos)) {
                    easyText.setFillColor(sf::Color::Green);
                    return "easy";
                }
                else if (mediumText.getGlobalBounds().contains(mousePos)) {
                    mediumText.setFillColor(sf::Color::Yellow);
                    return "medium";
                }
                else if (hardText.getGlobalBounds().contains(mousePos)) {
                    hardText.setFillColor(sf::Color::Red);
                    return "hard";
                }
                else if (backText.getGlobalBounds().contains(mousePos)) {
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
    window.clear();
    window.draw(backgroundSprite);
    window.draw(easyText);
    window.draw(mediumText);
    window.draw(hardText);
    window.draw(backText);
}