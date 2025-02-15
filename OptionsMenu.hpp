#ifndef OPTIONSMENU_HPP
#define OPTIONSMENU_HPP

#include <SFML/Graphics.hpp>
#include <string>

class OptionsMenu {
public:
    OptionsMenu(sf::RenderWindow& window);
    std::string run();

private:
    void draw();

    sf::RenderWindow& window;

    // Textes pour les boutons
    sf::Text difficultyText;
   
    sf::Texture backgroundTexture;  // Image de fond
    sf::Sprite backgroundSprite;
    sf::Text startText;
    sf::Text backText;
    sf::Text hardText;

    sf::Text mediumText;
    sf::Text optionsText;
    sf::Text scoresText;
    sf::Text quitText;

    // Police pour les textes
    sf::Font font;
};

#endif