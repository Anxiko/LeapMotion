#include "IController.hpp"
//#include "ControllerLeapMotion.hpp"
#include "ControllerMouse.hpp"

#include "World.hpp"
#include "Laser.hpp"
#include "Ship.hpp"
#include "EnemyManager.hpp"
#include "Effects.hpp"

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

	//Font
	sf::Font ft;
	if (!ft.loadFromFile("media/fonts/ethnocentric rg.ttf"))
    {
        std::cerr<<"Error loading the font"<<std::endl;
        return 1;
    }

	//HP_HUD
	constexpr int HP_HUD_X=WINDOW_WIDTH-110;
	constexpr int HP_HUD_Y=WINDOW_HEIGHT-50;

	DSI::Value_HUD hp_hud(ft,HP_HUD_X,HP_HUD_Y,"+ ");

	//XP_HUD
	constexpr int XP_HUD_X=10;
	constexpr int XP_HUD_Y=WINDOW_HEIGHT-50;

	DSI::Value_HUD xp_hud(ft,XP_HUD_X,XP_HUD_Y,"XP: ");

	//KS_HUD
	constexpr int KS_HUD_X=10;
	constexpr int KS_HUD_Y=10;

	DSI::Value_HUD ks_hud(ft,KS_HUD_X,KS_HUD_Y,"KS: ");

	//AB_HUD
	constexpr int AB_HUD_X=WINDOW_WIDTH-185;
	constexpr int AB_HUD_Y=10;

	DSI::Value_HUD ab_hud(ft,AB_HUD_X,AB_HUD_Y,"Bomb: ");

    //Background
	sf::Texture background_texture;
	if (!background_texture.loadFromFile("media/background.jpg"))
    {
        std::cerr<<"Error loading the background's texture"<<std::endl;
        return 1;
    }

    constexpr int WINDOW_BACKGROUND_SPEED=1;
    DSI::Background background(background_texture,WINDOW_WIDTH,WINDOW_HEIGHT,WINDOW_BACKGROUND_SPEED);

	//Flash bomb
	sf::Texture bomb_texture;
	if (!bomb_texture.loadFromFile("media/explosion.jpg"))
    {
        std::cerr<<"Error loading the explosion texture";
        return 1;
    }

    sf::Sprite bomb_sprite(bomb_texture);
    bomb_sprite.setScale(1.0*WINDOW_WIDTH/bomb_texture.getSize().x,1.0*WINDOW_HEIGHT/bomb_texture.getSize().y);

	constexpr int BOMB_FLASH_TICKS=30;
	int flash_ticks=0;//Ticks to flash the screen when a bomb goes off

	//Array Laser
	DSI::ArrayLaser player_array;//Lasers from the player
	DSI::ArrayLaser enemy_array;//Lasers from the enemy

	//Player laser model
	constexpr int PLAYER_LASER_DMG = 25;
	constexpr int PLAYER_LASER_SZ = 7;
	constexpr int PLAYER_LASER_TICKS=20;
	constexpr arrow::Vct::Mod PLAYER_LASER_SPEED = 5;
	const sf::Color &PLAYER_LASER_COLOR = sf::Color::Magenta;

	const DSI::LaserModel player_laser_model(PLAYER_LASER_DMG,PLAYER_LASER_TICKS,PLAYER_LASER_SZ,PLAYER_LASER_SPEED,PLAYER_LASER_COLOR);

	//Enemy laser model
	constexpr int ENEMY_LASER_DMG=40;
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
    sf::Texture enemy_texture;
    if (!enemy_texture.loadFromFile("media/EnemyShip.png"))
    {
        std::cerr<<"Error loading the enemy's texture"<<std::endl;
        return 1;
    }

    //Player ship
    constexpr DSI::EnergyT PLAYER_ENER=100;
    DSI::Ship player_ship(arrow::Vct(WINDOW_WIDTH/2,WINDOW_HEIGHT/2),player_texture,DSI::Ship::ORIENTATION_UP,player_world, player_laser_model,PLAYER_ENER,new DSI::ControllerMouse(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,window));

    int player_points=0;//Points the p`layer has obtained
    int player_kill_streak=0;//Number of ships killed by the player before getting hit a single time
    bool player_ability_ready=false;//Player has the ability ready to use
    constexpr int PLAYER_KILLSTREAK_ABILITY=2;//Killstreak ready to unlock the ability
    constexpr int PLAYER_RECHARGE_ENER_PER_KILL=10;//Energy recharged per kill
    constexpr int PLAYER_POINTS_PER_KILL=10;//Points gained per kill

    //Enemy ships
    //DSI::Ship enemy_ship(arrow::Vct(WINDOW_WIDTH/2,40),player_texture,DSI::Ship::ORIENTATION_DOWN,enemy_world,player_laser_model,PLAYER_ENER);
    constexpr int ENEMY_SHIPS=10;
    constexpr int ENEMY_ENER=25;

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

		background.update();
		hp_hud.update(player_ship.get_energy());
		xp_hud.update(player_points);
		ks_hud.update(player_kill_streak);
		ab_hud.update(player_ability_ready);

		player_array.update(WINDOW_RCT);
		enemy_array.update(WINDOW_RCT);
		if (player_ship.update())//If the player's been hit, reset the killsteak
        {
            player_kill_streak=0;
        }

		int kills=enemy_ships.update();//Update the current killstreak
		if (kills)
        {
            player_kill_streak+=kills;

            //Regen health
            player_ship.recharge_energy(kills*PLAYER_RECHARGE_ENER_PER_KILL);

            //Gain XP
            player_points+=kills*PLAYER_POINTS_PER_KILL;
        }

		if (player_kill_streak>=PLAYER_KILLSTREAK_ABILITY)//Ability unlocked
        {
            player_ability_ready=true;
        }

        //Use the ability
        if (player_ship.ability()&&player_ability_ready)
        {
            player_ability_ready=false;
            player_kill_streak=0;
            enemy_ships.purge();
            flash_ticks=BOMB_FLASH_TICKS;
        }

        window.draw(background);

        if (flash_ticks)
		{
		    sf::Uint8 alpha = (255.0*flash_ticks)/BOMB_FLASH_TICKS;
		    std::cout<<"A: "<<+alpha;
            bomb_sprite.setColor(sf::Color(255,255,255,alpha));
            window.draw(bomb_sprite);
            --flash_ticks;
        }

		window.draw(hp_hud);
		window.draw(xp_hud);
		window.draw(ks_hud);
		window.draw(ab_hud);
		window.draw(enemy_ships);
		window.draw(player_ship);
		window.draw(enemy_array);
		window.draw(player_array);
		window.display();
	}

	return 0;
}
