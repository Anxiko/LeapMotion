#pragma once

namespace DSI
{
	//Interfaz de controlador
	class IController
	{
		/* 
			Attributes
		*/

		/* Windows size */
		protected:

			//Size of the window
			const int x, y;

		/*
			Methods
		*/

		/* Constructors, copy control */

		/*Constructor*/
		protected:

			//Complete constructor
			IController(const int ix, const int iy)
			:x(ix),y(iy)
			{}

			//Virtual destructor, inheritance
			virtual ~IController(){}

		/* Interface */
		public:

			/*Update*/

			//Update the controller (call each frame before getting data)
			virtual void update() = 0;

			/*Positions*/

			//Get X position (from 0 to x)
			virtual int getX() const = 0;

			//Get Y position (from 0 to y)
			virtual int getY() const = 0;

			/*Commands*/

			//Fire
			virtual bool fire() const = 0;

			//Ability
			virtual bool ability() const = 0;
	};
}