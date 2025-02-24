#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class DifficultyMenu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text easyText;
    sf::Text mediumText;
    sf::Text hardText;
    sf::Text backText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    void setupText(sf::Text& text, const std::string& str, float yPosition);

public:
    explicit DifficultyMenu(sf::RenderWindow& window);
    std::string run();
    void draw();
};