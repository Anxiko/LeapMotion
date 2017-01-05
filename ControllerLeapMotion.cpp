#include "ControllerLeapMotion.hpp"

namespace DSI
{
	/*
		ControllerLeapMotion
	*/
	
	/* Listener */

	/*Frame*/

	void ControllerLeapMotion::onFrame(const Leap::Controller & controller)
	{
		//Get the frame
		const Leap::Frame &frame = controller.frame();

		//There's only hand, get the data
		if (frame.hands().count()==1)
		{
			//Get the hand
			const Leap::Hand &hand = *(frame.hands().begin());

			//Get the position
			pos_x = static_cast<int>(hand.palmPosition().x);
			pos_y = static_cast<int>(hand.palmPosition().y);
			pos_z = static_cast<int>(hand.palmPosition().z);

			//Check if the fist is closed
			fist_closed = hand.fingers().extended().count() == 0;
		}
	}
}
