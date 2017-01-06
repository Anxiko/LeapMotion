#ifndef _ENEMY_MANAGER_H_
#define _ENEMY_MANAGER_H_

#include "Ship.hpp"
#include "IController.hpp"
#include "FDX_Geo/FDX_Geo.hpp"

using namespace fdx;

#include <list>
#include <random>
#include <chrono>

namespace DSI
{

    class EnemyLinearController : public IController
    {
        /* Config */

        /*Speed*/
        private:

            //Default speed of the ship
            static constexpr int DEF_SPEED = 2;

            //Default probability of firing
            static constexpr int DEF_PROB_FIRING = 90;

        /*RNG*/
        private:

            //Random engine
            static std::default_random_engine re;

        /* Members */

        /*Ship*/
        private:

            //Ship being controlled
            const Ship &ship;

            //Speed of the ship
            int speed;

            //Probability of firing
            int prob_firing;

        /* Constructors, copy control */

        /*Constructoes*/
        public:

            //Complete constructor
            EnemyLinearController(int ix, int iy, const Ship &iship, int ispeed=DEF_SPEED, int iprob_firing=DEF_PROB_FIRING);

        /* Interface */
        public:

			/*Update*/

			//Update the controller (call each frame before getting data)
			void update();

			/*Positions*/

			//Get X position (from 0 to x)
			int getX() const;

			//Get Y position (from 0 to y)
			int getY() const;

			/*Commands*/

			//Fire
			bool fire() const;

			//Ability
			bool ability() const;
    };

    class EnemyManager : public sf::Drawable
    {
        /* Config */

        /*RNG*/
        private:

            //Random engine
            static std::default_random_engine re;

        /*Ship*/
        private:

            //Default orientation
            static constexpr bool DEF_ORIENTATION = Ship::ORIENTATION_DOWN;

        /* Members */

        /*Ships*/
        private:

            //List of enemies
            std::list<Ship> ships;

            //Number of ships currently in the list
            int number_ships;

            //Number of ships that should be in the list
            const int target_ships;

        /*Screen*/

            //Screen rectangle
            arrow::Rct screen;

        /*Linear ship*/
        private:

            //Texture
            sf::Texture &linear_texture;

            //Laser model
            const LaserModel &linear_lm;

            //Energy
            const EnergyT linear_energy;

        /*Ship config*/
        private:

            //Orientation
            bool orientation;

            //World
            World &world;


        /* Constructors, copy control */

        /*Constructor*/
        public:

            //Complete constructor
            EnemyManager(int itarget_ships, const arrow::Rct &iscreen, sf::Texture &ilinear_texture, const LaserModel &ilinear_lm, EnergyT ilinear_energy,World &iworld);

        /* Methods */

        /*Array*/
        private:

            //Add a linear ship to the array
            void add_linear();

            //Add ships until the target is met
            void add_ships();

        /*Ships*/
        public:

            //Update all ships, remove those that are killed/out of screen. Return the number of ships that have been killed
            int update();

        /*Drawing*/
        public:

            //Draw the ship
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        /*Ability*/
        public:

            //Kill all ships. Return number of ships killed
            int purge();
    };
}

#endif // _ENEMY_MANAGER_H_
