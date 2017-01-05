#ifndef _SHIPS_HPP_
#define _SHIPS_HPP_

#include "SFML/Graphics.hpp"
#include "FDX_Geo/FDX_Geo.hpp"

using namespace fdx;

namespace DSI
{

    //Ship, player or enemy
    class Ship : public sf::Drawable
    {
        /*  Members */

        /*Phyisics*/
        private:

            //Position and size
            arrow::Rct r;

            //Speeds
            arrow::Vct up,dn,lf,rt;//Up, down, left and right speeds

        /*Looks*/
        private:

            //Sprite
            sf::Sprite sp;

        /* Constructors, copy control */

        /*Constructors*/
        public:

            //Complete constructor
            Ship(arrow::Vct &icenter, arrow::Vct &iup, arrow::Vct &idn, arrow::Vct &ilf, arrow::Vct &irt, sf::Texture &itexture);

        /* Methods */

        /*Drawing*/
        public:

            //Draw the ship
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:

            //Create the sprite
            void create_sprite(const sf::Texture &texture);
    };
}

#endif // _SHIPS_HPP_
