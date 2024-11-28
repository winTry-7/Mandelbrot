#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <complex>
#include "ComplexPlane.h"

using namespace sf;
using namespace std;

int main()
{
	int width = VideoMode::getDesktopMode().width;
	int	height = VideoMode::getDesktopMode().height;

	VideoMode vm(width, height);
	
	RenderWindow window(vm, "Mandelbrot", Style::Default);

	ComplexPlane theGraph(width, height);

	Font font;
	font.loadFromFile("Arial.ttf");

	Text welcome;
	welcome.setFont(font);
	welcome.setFillColor(sf::Color::Blue);

	cout << "all contructors done" << endl;

	while (window.isOpen())
	{


		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				// Quit when the window is closed
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					theGraph.setCenter({ event.mouseButton.x, event.mouseButton.y });
					
					theGraph.zoomIn();
				}

				if (event.mouseButton.button == sf::Mouse::Right)
				{
					theGraph.setCenter({ event.mouseButton.x, event.mouseButton.y });
					
					theGraph.zoomOut();

					cout << "set center ran" << endl;
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				theGraph.setMouseLocation({ sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y });
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		/*
		****************************************
		Update
		****************************************
		*/

		theGraph.updateRender();

		theGraph.loadText(welcome);

		/*
		****************************************
		Draw
		****************************************
		*/
		window.clear();

		theGraph.draw(window, RenderStates::Default);

		//FROM ME: draw the welcome message in the window
		window.draw(welcome);

		window.display();
	}
}