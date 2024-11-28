#include "ComplexPlane.h"
#include <string>
#include <iostream>
#include <sstream>
#include <complex>

using namespace sf;
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;

	m_aspectRatio = (float)pixelHeight / pixelWidth;

	m_plane_center.x = 0;
	m_plane_center.y = 0;

	m_plane_size.x = BASE_WIDTH;
	m_plane_size.y = BASE_HEIGHT * m_aspectRatio;

	m_zoomCount = 0;

	m_state = State::CALCULATING;

	m_vArray.setPrimitiveType(Points);
	m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
	int height = m_pixel_size.y; //VideoMode::getDesktopMode().height;
	int width = m_pixel_size.x;//VideoMode::getDesktopMode().width;

	Vector2i ji;

	if (m_state == CALCULATING)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				m_vArray[j + i * m_pixel_size.x].position = { (float)j,(float)i };
				
				ji.x = j;
				ji.y = i;

				Vector2f complexCoords = mapPixelToCoords(ji);

				int iterNum = countIterations(complexCoords);

				Uint8 r, g, b;

				iterationsToRGB(iterNum, r, g, b);

				m_vArray[j + i * m_pixel_size.x].color = { r,g,b };
			}
		}
	}

	m_state = DISPLAYING;
}

void ComplexPlane::zoomIn()
{
	m_zoomCount++;

	float xSize = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	float ySize = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));

	m_plane_size.x = xSize;
	m_plane_size.y = ySize;

	m_state = CALCULATING;
}

void ComplexPlane::zoomOut()
{
	m_zoomCount--;

	float xSize = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	float ySize = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));

	m_plane_size.x = xSize;
	m_plane_size.y = ySize;

	m_state = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
	m_plane_center = mapPixelToCoords(mousePixel);

	cout << "mapPixelToCoords worked" << endl;

	m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
	stringstream displayText;

	displayText << "Mandelbrot Set" << endl;
	displayText << "Center: (" <<  m_plane_center.x << ", " << m_plane_center.y << ")" << endl;
	displayText << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << endl;
	displayText << "Left-click to Zoom in" << endl;
	displayText << "Right-click to Zoom out" << endl;

	text.setString(displayText.str());
}

int ComplexPlane::countIterations(Vector2f coord)
{
	complex<float> c = { coord.x, coord.y };
	
	complex<float> z = 0;
	
	int i = 0;
	
	while (norm(z) < 4.0 && i < 64)
	{
		z = (z * z) + c;
		i++;
	}

	return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
	if (count == 64)
	{
		//black
		r = 0;
		g = 0;
		b = 0;
	}
	else if(count >= 45)
	{
		//navy blue
		r = 25;
		g = 25;
		b = 85;
	}
	else if (count >= 35)
	{
		//maroon
		r = 105;
		g = 45;
		b = 60;
	}
	else if (count >= 25)
	{
		//neon purple
		r = 205;
		g = 20;
		b = 240;
	}
	else if (count >= 15)
	{
		//white
		r = 255;
		g = 255;
		b = 255;
	}
	else
	{
		//neon green
		r = 30;
		g = 235;
		b = 23;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
	Vector2f currentCoord;
	
	//calculate the position of the mouse on the real+imaginary plane
	currentCoord.x = (((float)mousePixel.x - 0) / (m_pixel_size.x - 0)) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0);
	currentCoord.y = (((float)mousePixel.y - m_pixel_size.y) / (0 - m_pixel_size.y)) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0);

	return currentCoord;
}