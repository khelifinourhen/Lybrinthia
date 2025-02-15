#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <SFML/Graphics.hpp>
#include <string>

class MainMenu {
public:
    MainMenu(sf::RenderWindow& window);
    std::string run();

private:
    void draw();

    sf::RenderWindow& window;

    // Textes pour les boutons
    sf::Text startText;
    sf::Text optionsText;
    sf::Text scoresText;
    sf::Text quitText;
    sf::Texture backgroundTexture;  // Image de fond
    sf::Sprite backgroundSprite;
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    std::string nextState = "main_menu";  // Initialiser avec la valeur par défaut

    // Police pour les textes
    sf::Font font;
};

#endif