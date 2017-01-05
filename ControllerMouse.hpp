#pragma once

#include "IController.hpp"
#include "SFML/Window.hpp"

namespace DSI
{
	//Controller implemented with a mouse in SFML
	class ControllerMouse : public IController
	{
		/*
			Atributes
		*/

		/* SFML */

		/*Window*/
		private:

			//Reference window
			const sf::Window &ref_window;

		/*
			Methods
		*/

		/* Constructors, copy control */

		/*Constructors*/
		public:

			//Complete constructor
			ControllerMouse(int ix, int iy, const sf::Window &iref_window)
			:IController(ix,iy),ref_window(iref_window)
			{}

		/* IController */
		public:

			/*Update*/

			//Update the controller (call each frame before getting data)
			void update() {}

			/*Positions*/

			//Get X position (from 0 to x)
			int getX() const
			{
				return sf::Mouse::getPosition(ref_window).x;
			}

			//Get Y position (from 0 to y)
			int getY() const
			{
				return sf::Mouse::getPosition(ref_window).y;
			}

			/*Commands*/

			//Fire
			bool fire() const
			{
				return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
			}

			//Ability
			bool ability() const
			{
				return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
			}
		};
}