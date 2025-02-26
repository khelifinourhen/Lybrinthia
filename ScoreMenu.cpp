#include "ScoreMenu.hpp"
#include "Constants.hpp"
#include <fstream>
#include <iostream>

ScoreMenu::ScoreMenu(sf::RenderWindow& window) : window(window) {
    if (!font.loadFromFile(FONT_PATH)) {
        std::cerr << "Erreur : Impossible de charger la police." << std::endl;
    }

    if (!backgroundTexture.loadFromFile("C:/Users/hp/source/repos/lybrinthe/lybrinthe/Ressources/fond_ecran.jpeg")) {
        std::cerr << "Erreur : Impossible de charger l'image de fond." << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);

    backText.setFont(font);
    backText.setString("Back");
    backText.setCharacterSize(50);
    backText.setFillColor(sf::Color::White);
    backText.setPosition(SCREEN_WIDTH / 2 - backText.getLocalBounds().width / 2, SCREEN_HEIGHT - 100);
}

std::vector<std::string> ScoreMenu::loadScores() {
    std::vector<std::string> scores;
    std::ifstream file("scores.txt");
    std::string line;
    while (std::getline(file, line)) {
        scores.push_back(line);
    }
    return scores;
}

void ScoreMenu::draw() {
    window.clear();
    window.draw(backgroundSprite);

    std::vector<std::string> scores = loadScores();
    float yPos = 100;
    for (const auto& score : scores) {
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString(score);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(SCREEN_WIDTH / 2 - scoreText.getLocalBounds().width / 2, yPos);
        window.draw(scoreText);
        yPos += 50;
    }

    window.draw(backText);
    window.display();
}

void ScoreMenu::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (backText.getGlobalBounds().contains(mousePos)) {
                    return;
                }
            }
        }

        draw();
    }
}