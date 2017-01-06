#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "Laser.hpp"

#include <vector>

#include "FDX_Geo/FDX_Geo.hpp"

using namespace fdx;

namespace DSI
{
    class World
    {
        /*  Members */

        /*Walls*/
        private:

            std::vector<arrow::Rct> walls;//Walls to collide against

        /*Laser*/
        private:

            //Array laser to shoot to
            ArrayLaser &shoot_to;

            //Array laser to get hit from
            ArrayLaser &hit_from;

        /*Constructors, copy control*/

        /*Constructors*/
        public:

            //Complete constructor (wall stealing)
            World(std::vector<arrow::Rct> &&iwalls, ArrayLaser &ishoot_to, ArrayLaser &ihit_from);

        /* Methods */

        /*Movement*/
        public:

            //Move a shape against the walls, return final movement
            arrow::Vct mov_against(arrow::Shp &shp, arrow::Vct &speed) const;

        /*Laser*/
        public:

            //Shoot a laser into the array
            void shoot(const LaserModel &ilm, const arrow::Vct &iposition, arrow::Vct ispeed);

            //Get hit from the lasers of the array
            EnergyT hit(const arrow::Shp &shp);
    };
}

#endif // _WORLD_HPP_
