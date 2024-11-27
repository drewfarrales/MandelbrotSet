#include "ComplexPlane.h"
#include <complex>
#include <sstream>
#include <cmath>

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
	m_pixel_width = pixelWidth;
	m_pixel_height = pixelHeight;
	m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
	m_plane_center = { 0.0f, 0.0f };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_state = State::CALCULATING;
	m_vArray.setPrimitiveType(sf::Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_vArray);
}

void ComplexPlane::updateRender() {
	if (m_state == State::CALCULATING) {
		m_vArray.resize(m_pixel_width * m_pixel_height);
		for (int i = 0; i < m_pixel_height; i++) {
			for (int j = 0; j < m_pixel_width; j++) {
				size_t index = j + i * m_pixel_width;
				if (index < m_vArray.getVertexCount()) {
					m_vArray[index].position = { static_cast<float>(j), static_cast<float>(i) };
					sf::Vector2f coord = mapPixelToCoords({ j,i });
					size_t iterations = countIterations(coord);
					sf::Uint8 r, g, b;
					iterationsToRGB(iterations, r, g, b);
					m_vArray[index].color = { r,g,b };
				}
			}
		}
		m_state = State::DISPLAYING;
	}
}

void ComplexPlane::zoomIn() {
	m_zoomCount++;
	float x_size = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
	float y_size = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x_size, y_size };
	m_state = State::CALCULATING;
}
void ComplexPlane::zoomOut() {
	m_zoomCount--;
	float x_size = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
	float y_size = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
	m_plane_size = { x_size, y_size };
	m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i mouseLoc) {
	m_plane_center = mapPixelToCoords(mouseLoc);
	m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i mouseLoc) {
	m_mouseLocation = mapPixelToCoords(mouseLoc);
}

void ComplexPlane::loadText(sf::Text& text) {
	std::stringstream ss;
	ss << "Mandelbrot set \n" << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n" << "Cursor: " << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n"
		<< "Zoom: " << std::pow(BASE_ZOOM, m_zoomCount) << "\nLeft-Click to Zoom in\nRight-Click to Zoom out";
	text.setString(ss.str());
}

size_t ComplexPlane::countIterations(sf::Vector2f coord) {
	std::complex<double> c(coord.x, coord.y);
	std::complex<double> z(0, 0);

	size_t iterations = 0;
	while (abs(z) < 2 && iterations < MAX_ITER) {
		z = z * z + c;
		iterations++;
	}
	return iterations;
}
void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b)
{
	if (count == MAX_ITER) {
		r = g = b = 0;
	}
	else if (count < MAX_ITER / 5) {

		r = 255 - count * 50;
		g = 40;
		b = 94;
	}
	else if (count < 2 * MAX_ITER / 5) {

		r = 255 - count * 50;
		g = 96;
		b = 5;
	}
	else if (count < 3 * MAX_ITER / 5) {

		r = 212;
		g = 255 - count * 50;
		b = 27;
	}
	else if (count < 4 * MAX_ITER / 5) {

		r = 5;
		g = 74;
		b = 255 - count * 50;
	}
	else {
		r = 255 - count * 50;
		g = 255 - count * 50;
		b = 255 - count * 50;
	}
}
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mouseLoc) {
	float x = ((mouseLoc.x - 0.0f) / (m_pixel_width - 0.0f)) * (m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
	float y = ((mouseLoc.y - m_pixel_height) / (0.0f - m_pixel_height)) * (m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);
	return { x,y };
}