#ifndef _LASER_HPP_
#define _LASER_HPP_

#include "FDX_Geo/FDX_Geo.hpp"
#include "SFML/Graphics.hpp"

#include <list>

using namespace fdx;

namespace DSI
{
    using EnergyT = int;

    class Laser;
    class LaserModel;

    class LaserModel
    {
        /* Members */

        /*Energy*/
        public:

            //Damage that this laser provokes
            const EnergyT damage;

            //Refresh rate of the laser
            const int ticks;

        /*Physics*/
        public:

            //Size
            const unsigned int sz;

            //Speed
            const arrow::Vct::Mod speed;

        /*Looks*/
        private:

            //Circle to draw
            mutable sf::CircleShape circle;

        /* Constructors, copy control */

        /*Constructors*/
        public:

            LaserModel(EnergyT idamage, int iticks, unsigned int isz, arrow::Vct::Mod ispeed, const sf::Color &icolor);

        /* Members */

        /*Drawing*/
        public:

            void draw_laser(sf::RenderTarget& target, sf::RenderStates states, const arrow::Vct &position) const;
    };

    class Laser : public sf::Drawable
    {
        /* Members */

        /*Model*/
        private:

            //Model of the laser
            const LaserModel &lm;

        /*Physics*/
        private:

            //Circle
            arrow::Crl c;

            //Speed vector
            arrow::Vct v;

        /* Constructos, copy control */

        /*Constructors*/
        public:

            Laser(const LaserModel &ilm, const arrow::Vct &iposition, arrow::Vct ispeed);

        /* Methods */

        /*Update*/
        public:

            //Move the laser
            void update();

        /*Drawing*/
        public:

            //Draw the ship
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        /*Hit*/
        public:

            //Check if it'll hit a shape
            bool hit(const arrow::Shp &shp) const;

            //Return the energy of this laser
            EnergyT get_dmg() const;

        /*Contact*/
        public:

            //Check for contact with another shape
            bool contact(const arrow::Shp &shp) const;

    };

    class ArrayLaser : public sf::Drawable
    {
        /* Members */

        /*Lasers*/
        private:

            std::list<Laser> lasers;

        /* Constructors, copy control */

        /*Constructors*/
        public:

            //Complete constructor
            ArrayLaser() = default;

        /* Methods */

        /*Update*/
        public:

            //Move the lasers
            void update(const arrow::Shp &screen);

       /*Drawing*/
        public:

            //Draw the ship
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        /*Array*/
        public:

            //Add a laser
            void add_laser(const LaserModel &ilm, const arrow::Vct &iposition, arrow::Vct ispeed);

        /*Hit*/
        public:

            //Return damage made to a shape
            EnergyT hit(const arrow::Shp &shp);

    };
}


#endif // _LASER_HPP_
