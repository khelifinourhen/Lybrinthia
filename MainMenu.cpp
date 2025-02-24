#include "MainMenu.hpp"
#include "Constants.hpp"
#include <iostream>
#include "ScoreMenu.hpp"

MainMenu::MainMenu(sf::RenderWindow& window) : window(window) {
    // Charger la police
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "ERREUR: Impossible de charger la police." << std::endl;
    }

    // Charger l'image de fond
    if (!backgroundTexture.loadFromFile("C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond_ecran.jpeg")) { // Chemin relatif
        std::cerr << "ERREUR: Impossible de charger l'image de fond." << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Redimensionner l'image de fond
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
    backgroundSprite.setScale(scaleX, scaleY);

    // Configurer les textes des boutons
    setupText(startText, "Start Game", 200);
    setupText(optionsText, "Options", 300);
    setupText(scoresText, "Scores", 400);
    setupText(quitText, "Quit", 500);
}

void MainMenu::setupText(sf::Text& text, const std::string& str, float yPos) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(40); // Taille augmentée
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(
        (window.getSize().x - text.getLocalBounds().width) / 2,
        yPos
    );
}

std::string MainMenu::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return "quit";
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (startText.getGlobalBounds().contains(mousePos)) {
                    return "start_game";
                }
                else if (optionsText.getGlobalBounds().contains(mousePos)) {
                    return "options";
                }
                else if (scoresText.getGlobalBounds().contains(mousePos)) {
                    ScoreMenu scoreMenu(window);
                    scoreMenu.run();
                }
                else if (quitText.getGlobalBounds().contains(mousePos)) {
                    return "quit";
                }
            }
        }

        draw();
        window.display();
    }
    return "quit";
}

void MainMenu::draw() {
    window.clear();
    window.draw(backgroundSprite);
    window.draw(startText);
    window.draw(optionsText);
    window.draw(scoresText);
    window.draw(quitText);
}