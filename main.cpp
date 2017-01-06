#include "IController.hpp"
//#include "ControllerLeapMotion.hpp"
#include "ControllerMouse.hpp"

#include "World.hpp"
#include "Laser.hpp"
#include "Ship.hpp"
#include "EnemyManager.hpp"

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
	constexpr int FPS=60;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME, sf::Style::Close);
	window.setFramerateLimit(FPS);

	//Array Laser
	DSI::ArrayLaser player_array;//Lasers from the player
	DSI::ArrayLaser enemy_array;//Lasers from the enemy

	//Player laser model
	constexpr int PLAYER_LASER_DMG = 25;
	constexpr int PLAYER_LASER_SZ = 10;
	constexpr int PLAYER_LASER_TICKS=20;
	constexpr arrow::Vct::Mod PLAYER_LASER_SPEED = 5;
	const sf::Color &PLAYER_LASER_COLOR = sf::Color::Magenta;

	const DSI::LaserModel player_laser_model(PLAYER_LASER_DMG,PLAYER_LASER_TICKS,PLAYER_LASER_SZ,PLAYER_LASER_SPEED,PLAYER_LASER_COLOR);

	//Enemy laser model
	constexpr int ENEMY_LASER_DMG=10;
	constexpr int ENEMY_LASER_SZ=7;
	constexpr int ENEMY_LASER_TICKS=20;
	constexpr arrow::Vct::Mod ENEMY_LASER_SPEED=2;
	const sf::Color &ENEMY_LASER_COLOR = sf::Color::Red;

	const DSI::LaserModel enemy_laser_model(ENEMY_LASER_DMG,ENEMY_LASER_TICKS,ENEMY_LASER_SZ,ENEMY_LASER_SPEED,ENEMY_LASER_COLOR);

	//Player world
	std::vector<arrow::Rct> walls;
	constexpr int WALL_THICKNESS=10;

	walls.push_back(arrow::Rct(arrow::Vct(0,-WALL_THICKNESS),arrow::Vct(WINDOW_WIDTH,WALL_THICKNESS)));//Up
	walls.push_back(arrow::Rct(arrow::Vct(WINDOW_WIDTH,0),arrow::Vct(WALL_THICKNESS,WINDOW_HEIGHT)));//Right
	walls.push_back(arrow::Rct(arrow::Vct(0,WINDOW_HEIGHT),arrow::Vct(WINDOW_WIDTH,WALL_THICKNESS)));//Down
	walls.push_back(arrow::Rct(arrow::Vct(-WALL_THICKNESS,0),arrow::Vct(WALL_THICKNESS,WINDOW_HEIGHT)));//Right

	DSI::World player_world(std::move(walls),player_array,enemy_array);

	//Enemy world
	DSI::World enemy_world(std::vector<arrow::Rct>(),enemy_array,player_array);

    //Player texture
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("media/PlayerShip.png"))
    {
        std::cerr<<"Error loading the player's texture"<<std::endl;
        return 1;
    }

    //Enemy texture
    sf::Texture &enemy_texture=player_texture;

    //Player ship
    constexpr DSI::EnergyT PLAYER_ENER=100;
    DSI::Ship player_ship(arrow::Vct(WINDOW_WIDTH/2,WINDOW_HEIGHT/2),player_texture,DSI::Ship::ORIENTATION_UP,player_world, player_laser_model,PLAYER_ENER,new DSI::ControllerMouse(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,window));


    //Enemy ships
    //DSI::Ship enemy_ship(arrow::Vct(WINDOW_WIDTH/2,40),player_texture,DSI::Ship::ORIENTATION_DOWN,enemy_world,player_laser_model,PLAYER_ENER);
    constexpr int ENEMY_SHIPS=5;
    constexpr int ENEMY_ENER=50;

    DSI::EnemyManager enemy_ships(ENEMY_SHIPS,WINDOW_RCT,enemy_texture,enemy_laser_model,ENEMY_ENER,enemy_world);

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
		enemy_array.update(WINDOW_RCT);
		player_ship.update();
		enemy_ships.update();

		window.clear(sf::Color::Black);
		window.draw(enemy_ships);
		window.draw(player_ship);
		window.draw(enemy_array);
		window.draw(player_array);
		window.display();
	}

	return 0;
}
