#ifndef _SHIPS_HPP_
#define _SHIPS_HPP_

#include "SFML/Graphics.hpp"
#include "FDX_Geo/FDX_Geo.hpp"

#include "IController.hpp"
#include "World.hpp"

using namespace fdx;

namespace DSI
{

    //Ship, player or enemy
    class Ship : public sf::Drawable
    {
        /* Config */

        /*Orientation*/
        public:

            //Ship going up or down (sprite)
            static constexpr bool ORIENTATION_UP=true,ORIENTATION_DOWN=false;

        /*  Members */

        /*Phyisics*/
        private:

            //Position and size
            arrow::Rct r;

        /*Looks*/
        private:

            //Sprite
            sf::Sprite sp;

            //Orientation
            bool orientation;

        /*Enviroment*/
        private:

            World &world;//World to interact with

            LaserModel &lm;//Laser to shoot

        /*Controls*/
        private:

            //Pointer to the controller of this ship, owned by this (free at end)
            IController *ptr_controller;

        /* Constructors, copy control */

        /*Constructors*/
        public:

            //Complete constructor
            Ship(const arrow::Vct &icenter, sf::Texture &itexture, bool iorientation, World &iworld, LaserModel &ilm, IController *iptr_controller=nullptr);

            //Destructor
            ~Ship();

        /* Methods */

        /*Drawing*/
        public:

            //Draw the ship
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:

            //Create the sprite
            void create_sprite(const sf::Texture &texture);

        /*Update*/
        public:

            //Update the ship's status (process info from controller)
            void update();

        /*Controller*/
        public:

            //Replace this ship's current controller by a new one
            void set_controller(IController *iptr_controller);

        /*Contact*/
        public:

            //Check for contact with a shape
            bool contact(const arrow::Shp &shp) const;

        /*Energy*/
        public:

            //Check if a ship is a alive
            bool is_alive() const;
    };
}

#endif // _SHIPS_HPP_
