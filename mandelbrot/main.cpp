#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

int main() {
	unsigned int width = sf::VideoMode::getDesktopMode().width / 2;
	unsigned int height = sf::VideoMode::getDesktopMode().height / 2;
	sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");

	ComplexPlane complexPlane(width, height);

	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::White);
	text.setPosition(5, 5);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				if (event.mouseButton.button == sf::Mouse::Right) {
					complexPlane.zoomOut();
				}
				else if (event.mouseButton.button == sf::Mouse::Left) {
					complexPlane.zoomIn();
				}
				complexPlane.setCenter(mousePos);
			}
		}

		if (event.type == sf::Event::MouseMoved) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			complexPlane.setMouseLocation(mousePos);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();

		complexPlane.updateRender();
		complexPlane.loadText(text);

		window.clear();
		window.draw(complexPlane);
		window.draw(text);
		window.display();
	}
	return 0;
}

