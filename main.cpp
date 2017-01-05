#include "IController.hpp"
#include "ControllerLeapMotion.hpp"
#include "ControllerMouse.hpp"

#include "SFML/Graphics.hpp"

#include <iostream>

int main()
{
	//Window size
	constexpr int WINDOW_WIDTH = 400;
	constexpr int WINDOW_HEIGHT = 400;

	//Window name
	const char * const WINDOW_NAME = "LeapMotion";

	//Create the window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);

	//Circle to draw
	constexpr int SHAPE_RADIUS = 10;
	constexpr int SHAPE_OUTLINE_ABIL = -5;
	sf::CircleShape circle(static_cast<float>(SHAPE_RADIUS));
	circle.setOrigin(static_cast<float>(SHAPE_RADIUS), static_cast<float>(SHAPE_RADIUS));
	circle.setFillColor(sf::Color::Green);
	circle.setOutlineColor(sf::Color::Blue);
	circle.setOutlineThickness(0);

	//Controller
	//DSI::ControllerLeapMotion leap_controller(WINDOW_WIDTH, WINDOW_HEIGHT);//Leap controller
	DSI::ControllerMouse mouse_controller(WINDOW_WIDTH, WINDOW_HEIGHT,window);//Mouse controller
	DSI::IController &controller = mouse_controller;//Selected controller
	//Main loop
	while (window.isOpen())
	{
		//Process events
		sf::Event e;
		while (window.pollEvent(e))
		{
			switch (e.type)
			{
				//Close the window
				case sf::Event::Closed:
				{
					window.close();
				}

				default:
					break;
			}
		}

		//Update the controller
		controller.update();

		//Update the shape
		circle.setPosition(static_cast<float>(controller.getX()), static_cast<float>(controller.getY()));//Position
		circle.setFillColor(controller.fire() ? sf::Color::Red : sf::Color::Green);//Firing
		circle.setOutlineThickness(controller.ability() ? static_cast<float>(SHAPE_OUTLINE_ABIL) : 0.0f);//Ability

		//Draw the shape
		window.clear();//Clear the screen
		window.draw(circle);//Draw the shape
		window.display();//Draw the screen
	}

	return 0;
}