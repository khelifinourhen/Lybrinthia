#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class MainMenu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text startText;
    sf::Text optionsText;
    sf::Text scoresText;
    sf::Text quitText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    explicit MainMenu(sf::RenderWindow& window);
    void setupText(sf::Text& text, const std::string& str, float yPos);
    std::string run();
    void draw();
};