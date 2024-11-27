#pragma once
#include <SFML/Graphics.hpp>

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

class ComplexPlane : public sf::Drawable {

public:
	enum class State {
		CALCULATING,
		DISPLAYING
	};

	ComplexPlane(int pixelWidth, int pixelHeight);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void updateRender();
	void zoomIn();
	void zoomOut();
	void setCenter(sf::Vector2i mouseLoc);
	void setMouseLocation(sf::Vector2i mouseLoc);
	void loadText(sf::Text& text);

private:
	size_t countIterations(sf::Vector2f coord);
	void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);
	sf::Vector2f mapPixelToCoords(sf::Vector2i mousLoc);
	sf::VertexArray m_vArray;
	sf::Vector2f m_plane_center;
	sf::Vector2f m_plane_size;
	sf::Vector2f m_mouseLocation;
	float m_aspectRatio;
	int m_pixel_width;
	int m_pixel_height;
	int m_zoomCount;
	State m_state;
};