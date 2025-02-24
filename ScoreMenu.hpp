#ifndef SCOREMENU_HPP
#define SCOREMENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class ScoreMenu {
public:
    ScoreMenu(sf::RenderWindow& window);
    void run();

private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text backText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::vector<std::string> loadScores();
    void draw();
};

#endif // SCOREMENU_HPP
