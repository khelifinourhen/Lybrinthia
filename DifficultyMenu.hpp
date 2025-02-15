#ifndef DIFFICULTYMENU_HPP
#define DIFFICULTYMENU_HPP

#include <SFML/Graphics.hpp>
#include <string>

class DifficultyMenu {
public:
    DifficultyMenu(sf::RenderWindow& window);
    std::string run();

private:
    void draw();
    void setupText(sf::Text& text, const std::string& str, float posY);

    sf::RenderWindow& window;

    // Fond d'écran
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // Textes des boutons
    sf::Text easyText;
    sf::Text mediumText;
    sf::Text hardText;
    sf::Text backText;

    // Police
    sf::Font font;
};

#endif // DIFFICULTYMENU_HPP
