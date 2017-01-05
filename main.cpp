#include "IController.hpp"
//#include "ControllerLeapMotion.hpp"
#include "ControllerMouse.hpp"

#include "World.hpp"
#include "Ship.hpp"

#include "FDX_Geo/FDX_Geo.hpp"
#include "SFML/Graphics.hpp"

#include <iostream>

using namespace fdx;

int main()
{
	//Window size
	constexpr int WINDOW_WIDTH = 400;
	constexpr int WINDOW_HEIGHT = 400;

	//Window name
	const char * const WINDOW_NAME = "LeapMotion";

	//Create the window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME, sf::Style::Close);

	//Player world
	std::vector<arrow::Rct> walls;
	constexpr int WALL_THICKNESS=10;

	walls.push_back(arrow::Rct(arrow::Vct(0,-WALL_THICKNESS),arrow::Vct(WINDOW_WIDTH,WALL_THICKNESS)));//Up
	walls.push_back(arrow::Rct(arrow::Vct(WINDOW_WIDTH,0),arrow::Vct(WALL_THICKNESS,WINDOW_HEIGHT)));//Right
	walls.push_back(arrow::Rct(arrow::Vct(0,WINDOW_HEIGHT),arrow::Vct(WINDOW_WIDTH,WALL_THICKNESS)));//Down
	walls.push_back(arrow::Rct(arrow::Vct(-WALL_THICKNESS,0),arrow::Vct(WALL_THICKNESS,WINDOW_HEIGHT)));//Right

	DSI::World player_world(std::move(walls));

    //Player texture
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("media/PlayerShip.png"))
    {
        std::cerr<<"Error loading the player's texture"<<std::endl;
        return 1;
    }
    //Player ship
    DSI::Ship player_ship(arrow::Vct(WINDOW_WIDTH/2,WINDOW_HEIGHT/2),player_texture,player_world,new DSI::ControllerMouse(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,window));

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

		player_ship.update();

		window.clear(sf::Color::Black);
		window.draw(player_ship);
		window.display();
	}

	return 0;
}
