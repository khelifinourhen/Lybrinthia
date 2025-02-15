#include "MainMenu.hpp"
#include "Constants.hpp"
#include "Test.hpp"
#include <iostream>

MainMenu::MainMenu(sf::RenderWindow& window) : window(window) {
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

    // Configurer les textes des boutons
    startText.setFont(font);
    startText.setString("Start Game");
    startText.setCharacterSize(30);
    startText.setFillColor(WHITE);
    startText.setPosition(SCREEN_WIDTH / 2 - startText.getLocalBounds().width / 2, 200);

    optionsText.setFont(font);
    optionsText.setString("Options");
    optionsText.setCharacterSize(30);
    optionsText.setFillColor(WHITE);
    optionsText.setPosition(SCREEN_WIDTH / 2 - optionsText.getLocalBounds().width / 2, 250);

    scoresText.setFont(font);
    scoresText.setString("Scores");
    scoresText.setCharacterSize(30);
    scoresText.setFillColor(WHITE);
    scoresText.setPosition(SCREEN_WIDTH / 2 - scoresText.getLocalBounds().width / 2, 300);

    quitText.setFont(font);
    quitText.setString("Quit");
    quitText.setCharacterSize(30);
    quitText.setFillColor(WHITE);
    quitText.setPosition(SCREEN_WIDTH / 2 - quitText.getLocalBounds().width / 2, 350);
}

std::string MainMenu::run() {
    std::string nextState = "main_menu";  // Initialisation de nextState

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

                    if (startText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        std::cout << "Bouton Start Game cliqué" << std::endl;
                        startSFMLGame();  // Lance l'interface SFML directement
                    }

                    // Retour au menu principal si l'utilisateur clique sur "Retour"
                    if (nextState == "main_menu") {
                        return "main_menu";
                    }

                    // Vérifier si la souris est sur le bouton "Options"
                    if (optionsText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        std::cout << "Bouton Options cliqué" << std::endl;
                        nextState = "options";  // Passer à l'écran des options
                    }

                    // Vérifier si la souris est sur le bouton "Scores"
                    if (scoresText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        std::cout << "Bouton Scores cliqué" << std::endl;
                        nextState = "scores";  // Passer à l'écran des scores
                    }

                    // Vérifier si la souris est sur le bouton "Quit"
                    if (quitText.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        std::cout << "Bouton Quit cliqué" << std::endl;
                        return "quit";  // Quitter l'application
                    }
                }
            }
        }

        draw();
        window.display();
    }
    return "quit";  // Valeur par défaut en cas de fermeture
}

void MainMenu::draw() {
    window.clear(BLACK);

    // Afficher les textes des boutons
    window.draw(backgroundSprite);
    window.draw(startText);
    window.draw(optionsText);
    window.draw(scoresText);
    window.draw(quitText);
}
