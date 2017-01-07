#include "IController.hpp"
#include "ControllerLeapMotion.hpp"
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

//Type of controller
constexpr bool CONTROLLER_MOUSE = false;
constexpr bool CONTROLLER_LEAP = true;

int welcome_screen(const int WINDOW_WIDTH, const int WINDOW_HEIGHT, sf::RenderWindow &window, bool &controller,  int const * player_score=nullptr);
int play_game (const int WINDOW_WIDTH, const int WINDOW_HEIGHT, sf::RenderWindow &window, bool controller, int &player_points);

int main()
{
	//Window size
	constexpr int WINDOW_WIDTH = 800;
	constexpr int WINDOW_HEIGHT = 600;

	//Window name
	const char * const WINDOW_NAME = "LeapMotion";

	//Create the window
	constexpr int FPS=60;

	//Player score
	int player_score=0;

	//Player controller
	bool player_controller = CONTROLLER_MOUSE;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME, sf::Style::Close);
	window.setFramerateLimit(FPS);

	if (welcome_screen(WINDOW_WIDTH,WINDOW_HEIGHT,window,player_controller))
        return 1;
    while(window.isOpen())
    {
        if (play_game(WINDOW_WIDTH,WINDOW_HEIGHT,window,player_controller, player_score)||welcome_screen(WINDOW_WIDTH,WINDOW_HEIGHT,window,player_controller,&player_score))
            return 1;
    }
}

int play_game (const int WINDOW_WIDTH, const int WINDOW_HEIGHT, sf::RenderWindow &window, bool controller, int &player_points)
{
    //Window size
    const arrow::Rct WINDOW_RCT(arrow::Vct(0,0),arrow::Vct(WINDOW_WIDTH,WINDOW_HEIGHT));

    //Font
	sf::Font ft;
	if (!ft.loadFromFile("media/fonts/ethnocentric rg.ttf"))
    {
        std::cerr<<"Error loading the font"<<std::endl;
        return 1;
    }

	//HP_HUD
	const int HP_HUD_X=WINDOW_WIDTH-110;
	const int HP_HUD_Y=WINDOW_HEIGHT-50;

	DSI::Value_HUD hp_hud(ft,HP_HUD_X,HP_HUD_Y,"+ ");

	//XP_HUD
	const int XP_HUD_X=10;
	const int XP_HUD_Y=WINDOW_HEIGHT-50;

	DSI::Value_HUD xp_hud(ft,XP_HUD_X,XP_HUD_Y,"XP: ");

	//KS_HUD
	const int KS_HUD_X=10;
	const int KS_HUD_Y=10;

	DSI::Value_HUD ks_hud(ft,KS_HUD_X,KS_HUD_Y,"KS: ");

	//AB_HUD
	const int AB_HUD_X=WINDOW_WIDTH-185;
	const int AB_HUD_Y=10;

	DSI::Value_HUD ab_hud(ft,AB_HUD_X,AB_HUD_Y,"Bomb: ");

    //Background
	sf::Texture background_texture;
	if (!background_texture.loadFromFile("media/background.jpg"))
    {
        std::cerr<<"Error loading the background's texture"<<std::endl;
        return 1;
    }

    const int WINDOW_BACKGROUND_SPEED=1;
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

	const int BOMB_FLASH_TICKS=30;
	int flash_ticks=0;//Ticks to flash the screen when a bomb goes off

	//Array Laser
	DSI::ArrayLaser player_array;//Lasers from the player
	DSI::ArrayLaser enemy_array;//Lasers from the enemy

	//Player laser model
	const int PLAYER_LASER_DMG = 25;
	const int PLAYER_LASER_SZ = 7;
	const int PLAYER_LASER_TICKS=20;
	const arrow::Vct::Mod PLAYER_LASER_SPEED = 5;
	const sf::Color &PLAYER_LASER_COLOR = sf::Color::Magenta;

	const DSI::LaserModel player_laser_model(PLAYER_LASER_DMG,PLAYER_LASER_TICKS,PLAYER_LASER_SZ,PLAYER_LASER_SPEED,PLAYER_LASER_COLOR);

	//Enemy laser model
	const int ENEMY_LASER_DMG=40;
	const int ENEMY_LASER_SZ=7;
	const int ENEMY_LASER_TICKS=20;
	const arrow::Vct::Mod ENEMY_LASER_SPEED=2;
	const sf::Color &ENEMY_LASER_COLOR = sf::Color::Red;

	const DSI::LaserModel enemy_laser_model(ENEMY_LASER_DMG,ENEMY_LASER_TICKS,ENEMY_LASER_SZ,ENEMY_LASER_SPEED,ENEMY_LASER_COLOR);

	//Player world
	std::vector<arrow::Rct> walls;
	const int WALL_THICKNESS=10;

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
    const DSI::EnergyT PLAYER_ENER=100;
    DSI::Ship player_ship(arrow::Vct(WINDOW_WIDTH/2,WINDOW_HEIGHT/2),player_texture,DSI::Ship::ORIENTATION_UP,player_world, player_laser_model,PLAYER_ENER,controller==CONTROLLER_MOUSE?(DSI::IController *)new DSI::ControllerMouse(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,window):(DSI::IController *)new DSI::ControllerLeapMotion(WINDOW_WIDTH,WINDOW_HEIGHT));

    player_points=0;//Points the player has obtained
    int player_kill_streak=0;//Number of ships killed by the player before getting hit a single time
    bool player_ability_ready=false;//Player has the ability ready to use
    const int PLAYER_KILLSTREAK_ABILITY=2;//Killstreak ready to unlock the ability
    const int PLAYER_RECHARGE_ENER_PER_KILL=10;//Energy recharged per kill
    const int PLAYER_POINTS_PER_KILL=10;//Points gained per kill

    //Enemy ships
    //DSI::Ship enemy_ship(arrow::Vct(WINDOW_WIDTH/2,40),player_texture,DSI::Ship::ORIENTATION_DOWN,enemy_world,player_laser_model,PLAYER_ENER);
    const int ENEMY_SHIPS=10;
    const int ENEMY_ENER=25;

    DSI::EnemyManager enemy_ships(ENEMY_SHIPS,WINDOW_RCT,enemy_texture,enemy_laser_model,ENEMY_ENER,enemy_world);

	//Main loop
	while (window.isOpen()&&player_ship.is_alive())
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

int welcome_screen(const int WINDOW_WIDTH, const int WINDOW_HEIGHT, sf::RenderWindow &window, bool &controller, int const * player_score)
{
    //Text to print
    //Font
	sf::Font ft;
	if (!ft.loadFromFile("media/fonts/ethnocentric rg.ttf"))
    {
        std::cerr<<"Error loading the font"<<std::endl;
        return 1;
    }

    sf::FloatRect textRect;

    //Title text
    const int TITLE_TEXT_Y=WINDOW_HEIGHT/2-100;

    sf::Text title_text(std::string("Hand_Ship"),ft);

    textRect = title_text.getLocalBounds();
    title_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

    title_text.setPosition(WINDOW_WIDTH/2,TITLE_TEXT_Y);


    //Enter text
    const int ENTER_TEXT_Y=WINDOW_HEIGHT/2;

    sf::Text enter_text(std::string("Press Enter to play"),ft);

    textRect = enter_text.getLocalBounds();
    enter_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

    enter_text.setPosition(WINDOW_WIDTH/2,ENTER_TEXT_Y);

    //Score text
    const int SCORE_TEXT_Y=WINDOW_HEIGHT-50;

    std::stringstream ss;
    ss<<"Previous score: "<<(player_score?*player_score:0);

    sf::Text score_text(ss.str(),ft);
    textRect = score_text.getLocalBounds();
    score_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

    score_text.setPosition(WINDOW_WIDTH/2,SCORE_TEXT_Y);

	//Controller text
	const int CONTROLLER_TEXT_Y = WINDOW_HEIGHT - 100;

	sf::Text controller_text(std::string("Using ") + (controller == CONTROLLER_MOUSE ? std::string("mouse") : std::string("Leap Motion")) + std::string(" controller, press Space to switch"), ft,10);
	textRect = controller_text.getLocalBounds();
	controller_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	controller_text.setPosition(WINDOW_WIDTH / 2, CONTROLLER_TEXT_Y);

    while (window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e))
        {
            switch (e.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }

                case sf::Event::KeyPressed:
                {
					if (e.key.code == sf::Keyboard::Return)
						return 0;
					else if (e.key.code == sf::Keyboard::Space)
					{
						controller = !controller;
						controller_text.setString(std::string("Using ") + (controller == CONTROLLER_MOUSE ? std::string("mouse") : std::string("Leap Motion")) + std::string(" controller, press Space to switch"));
						textRect = controller_text.getLocalBounds();
						controller_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
					}
                }

                default:
                    break;
            }
        }

        window.clear(sf::Color::Black);
        window.draw(title_text);
        window.draw(enter_text);
		window.draw(controller_text);
        if (player_score)
            window.draw(score_text);
        window.display();

    }

    return 0;
}
