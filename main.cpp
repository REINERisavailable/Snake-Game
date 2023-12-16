#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>

using namespace std;

class Serpent {
public:
    Serpent();
    void deplacer(sf::Vector2f direction);
    void grandir();
    bool verifierCollision();
    sf::Vector2f getPositionTete();
    void dessiner(sf::RenderWindow& fenetre);

private:
    deque<sf::RectangleShape> segments;
};

Serpent::Serpent() {
    segments.push_front(sf::RectangleShape(sf::Vector2f(20, 20)));
    segments.front().setPosition(100, 100);
    segments.front().setFillColor(sf::Color::Green);
}

void Serpent::deplacer(sf::Vector2f direction) {
    sf::Vector2f positionTete = segments.front().getPosition() + direction;

    if (positionTete.x < 0)
        positionTete.x = 800 - 20;
    else if (positionTete.x >= 800)
        positionTete.x = 0;

    if (positionTete.y < 0)
        positionTete.y = 600 - 20;
    else if (positionTete.y >= 600)
        positionTete.y = 0;

    sf::RectangleShape nouveauSegment(sf::Vector2f(20, 20));
    nouveauSegment.setFillColor(sf::Color::Green);
    nouveauSegment.setPosition(positionTete);

    segments.push_front(nouveauSegment);
    segments.pop_back();
}

void Serpent::grandir() {
    sf::RectangleShape nouveauSegment(sf::Vector2f(20, 20));
    nouveauSegment.setFillColor(sf::Color::Green);
    nouveauSegment.setPosition(segments.front().getPosition());
    segments.push_back(nouveauSegment);
}

bool Serpent::verifierCollision() {
    sf::Vector2f positionTete = getPositionTete();

    if (positionTete.x < 0 || positionTete.x >= 800 || positionTete.y < 0 || positionTete.y >= 600) {
        return true;
    }

    for (auto it = segments.begin() + 1; it != segments.end(); ++it) {
        if (positionTete == it->getPosition()) {
            return true;
        }
    }

    return false;
}

sf::Vector2f Serpent::getPositionTete() {
    return segments.front().getPosition();
}

void Serpent::dessiner(sf::RenderWindow& fenetre) {
    for (const auto& segment : segments) {
        fenetre.draw(segment);
    }
}

class Nourriture {
public:
    Nourriture();
    void respawn();
    sf::Vector2f getPosition();
    void dessiner(sf::RenderWindow& fenetre);

private:
    sf::RectangleShape nourriture;
};

Nourriture::Nourriture() {
    nourriture.setSize(sf::Vector2f(20, 20));
    nourriture.setFillColor(sf::Color::Red);
    respawn();
}

void Nourriture::respawn() {
    nourriture.setPosition(rand() % 40 * 20, rand() % 30 * 20);
}

sf::Vector2f Nourriture::getPosition() {
    return nourriture.getPosition();
}

void Nourriture::dessiner(sf::RenderWindow& fenetre) {
    fenetre.draw(nourriture);
}

class JeuSerpent {
public:
    JeuSerpent();
    void executer();

private:
    sf::RenderWindow fenetre;
    Serpent serpent;
    Nourriture nourriture;
    sf::Vector2f direction;
    int score;
    sf::Font police;
    sf::Text texteScore;
};

JeuSerpent::JeuSerpent() : fenetre(sf::VideoMode(800, 600), "Jeu du Serpent") {
    fenetre.setFramerateLimit(10);
    direction = sf::Vector2f(0, 0);

    if (!police.loadFromFile("C:\\joystix.otf")) {
        cerr << "�chec du chargement de la police !" << endl;
    }

    score = 0;
    texteScore.setFont(police);
    texteScore.setCharacterSize(20);
    texteScore.setPosition(10, 10);
    texteScore.setFillColor(sf::Color::White);
}

void JeuSerpent::executer() {
    bool jeuDemarre = false;

    while (fenetre.isOpen()) {
        sf::Event evenement;
        while (fenetre.pollEvent(evenement)) {
            if (evenement.type == sf::Event::Closed)
                fenetre.close();
            else if (evenement.type == sf::Event::KeyPressed) {
                jeuDemarre = true;
            }
        }

        if (!jeuDemarre) {
            sf::Text texteDemarrage("Snake Game by jamri", police, 40);
            texteDemarrage.setPosition(80, 250);
            texteDemarrage.setFillColor(sf::Color::White);

            sf::Text texteDebut("Appuyez une touche pour commencer", police, 20);
            texteDebut.setPosition(100, 300);
            texteDebut.setFillColor(sf::Color::White);

            fenetre.clear();
            fenetre.draw(texteDemarrage);
            fenetre.draw(texteDebut);
            fenetre.display();
            continue;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction.y == 0)
            direction = sf::Vector2f(0, -20);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction.y == 0)
            direction = sf::Vector2f(0, 20);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction.x == 0)
            direction = sf::Vector2f(-20, 0);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction.x == 0)
            direction = sf::Vector2f(20, 0);

        serpent.deplacer(direction);

        if (serpent.getPositionTete().x < 0 || serpent.getPositionTete().x >= 800 ||
            serpent.getPositionTete().y < 0 || serpent.getPositionTete().y >= 600 ||
            serpent.verifierCollision()) {
            cout << "Game Over! Score: " << score << endl;
            fenetre.close();
        }

        if (serpent.getPositionTete() == nourriture.getPosition()) {
            nourriture.respawn();
            serpent.grandir();
            score++;
            texteScore.setString("Score : " + to_string(score));
        }

        fenetre.clear();
        nourriture.dessiner(fenetre);
        serpent.dessiner(fenetre);
        fenetre.draw(texteScore);
        fenetre.display();
    }
}

int main() {
    JeuSerpent jeu;
    jeu.executer();

    return 0;
}
