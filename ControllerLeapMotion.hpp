#pragma once

#include "IController.hpp"
#include "Leap.h"

namespace DSI
{
	//Controller implemented with a Leap Motion
	class ControllerLeapMotion : public IController, public Leap::Listener
	{
		/*
			Attributes
		*/

		/* Controller */

		/*Hand*/
		private:

			//Position of the palm
			int pos_x,pos_y,pos_z;

			//Fist is closed
			bool fist_closed;

		/* LeapMotion */

		/*Controller*/
		private:

			//Controller calls this listener when frames are received
			Leap::Controller controller;

		/* Config */

		/*Height*/
		private:
			 
			//Height needed to activate the ability
			static constexpr int HEIGHT_ABIL = 50;

		/*Position of the hand*/
		private:

			//X
			static constexpr int HAND_MIN_X = -100;
			static constexpr int HAND_MAX_X = 100;

			//Y
			static constexpr int HAND_MIN_Y = -100;
			static constexpr int HAND_MAX_Y = 100;

		/*
			Methods
		*/

		/* Constructors, copy control */

		/*Constructors*/
		public:

			//Complete constructor
			ControllerLeapMotion(int ix, int iy)
			:IController(ix,iy),pos_x(ix/2),pos_y(iy/2),pos_z(0),fist_closed(false),controller()
			{
				//Add this class a listener to the controller
				controller.addListener(*this);
			}

			//Destructor
			~ControllerLeapMotion()
			{
				//Remove this object a listener
				controller.removeListener(*this);
			}

		/* Listener */

		/*Frame*/
		public:

			void onFrame(const Leap::Controller &controller);

		/* IController */
		public:

			/*Update*/

			//Update the controller (call each frame before getting data)
			void update() {}

			/*Positions*/

			//Get X position (from 0 to x)
			int getX() const
			{
				double rv = pos_x;//Return value

				rv -= HAND_MIN_X;//0 start range
				rv /= HAND_MAX_X - HAND_MIN_X;//Get it on the [0,1] range
				rv *= x;//Scale to output
				return static_cast<int>(rv);
			}

			//Get Y position (from 0 to y)
			int getY() const
			{
				double rv = pos_y;//Return value

				rv -= HAND_MIN_Y;//0 start range
				rv /= HAND_MAX_Y - HAND_MIN_Y;//Get it on the [0,1] range
				rv *= y;//Scale to output
				return static_cast<int>(rv);
			}

			/*Commands*/

			//Fire
			bool fire() const
			{
				return fist_closed;
			}

			//Ability
			bool ability() const
			{
				return pos_z >= HEIGHT_ABIL;
			}
	};
}