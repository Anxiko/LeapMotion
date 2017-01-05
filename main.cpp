#include "IController.hpp"
//#include "ControllerLeapMotion.hpp"
#include "ControllerMouse.hpp"

#include "World.hpp"
#include "Laser.hpp"
#include "Ship.hpp"

#include "FDX_Geo/FDX_Geo.hpp"
#include "SFML/Graphics.hpp"

#include <iostream>

using namespace fdx;

int main()
{
	//Window size
	constexpr int WINDOW_WIDTH = 800;
	constexpr int WINDOW_HEIGHT = 600;
	const arrow::Rct WINDOW_RCT(arrow::Vct(0,0),arrow::Vct(WINDOW_WIDTH,WINDOW_HEIGHT));

	//Window name
	const char * const WINDOW_NAME = "LeapMotion";

	//Create the window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME, sf::Style::Close);

	//Array Laser
	DSI::ArrayLaser player_array;//Lasers from the player
	DSI::ArrayLaser enemy_array;//Lasers from the enemy

	constexpr int PLAYER_LASER_DMG = 25;
	constexpr int PLAYER_LASER_SZ = 10;
	constexpr arrow::Vct::Mod PLAYER_LASER_SPEED = 5;
	const sf::Color &PLAYER_LASER_COLOR = sf::Color::Magenta;

	DSI::LaserModel player_laser_model(PLAYER_LASER_DMG,PLAYER_LASER_SZ,PLAYER_LASER_SPEED,PLAYER_LASER_COLOR);

	//Player world
	std::vector<arrow::Rct> walls;
	constexpr int WALL_THICKNESS=10;

	walls.push_back(arrow::Rct(arrow::Vct(0,-WALL_THICKNESS),arrow::Vct(WINDOW_WIDTH,WALL_THICKNESS)));//Up
	walls.push_back(arrow::Rct(arrow::Vct(WINDOW_WIDTH,0),arrow::Vct(WALL_THICKNESS,WINDOW_HEIGHT)));//Right
	walls.push_back(arrow::Rct(arrow::Vct(0,WINDOW_HEIGHT),arrow::Vct(WINDOW_WIDTH,WALL_THICKNESS)));//Down
	walls.push_back(arrow::Rct(arrow::Vct(-WALL_THICKNESS,0),arrow::Vct(WALL_THICKNESS,WINDOW_HEIGHT)));//Right

	DSI::World player_world(std::move(walls),player_array,enemy_array);

    //Player texture
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("media/PlayerShip.png"))
    {
        std::cerr<<"Error loading the player's texture"<<std::endl;
        return 1;
    }
    //Player ship
    DSI::Ship player_ship(arrow::Vct(WINDOW_WIDTH/2,WINDOW_HEIGHT/2),player_texture,DSI::Ship::ORIENTATION_UP,player_world, player_laser_model,new DSI::ControllerMouse(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,window));

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

		player_array.update(WINDOW_RCT);
		player_ship.update();

		window.clear(sf::Color::Black);
		window.draw(player_ship);
		window.draw(player_array);
		window.display();
	}

	return 0;
}
