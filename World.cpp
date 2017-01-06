#include "World.hpp"

namespace DSI
{
    /*
        World
    */

    /*Constructors, copy control*/

    /*Constructors*/

    //Complete constructor (wall stealing)
    World::World(std::vector<arrow::Rct> &&iwalls, ArrayLaser &ishoot_to, ArrayLaser &ihit_from)
    :walls(iwalls),shoot_to(ishoot_to),hit_from(ihit_from)
    {}

    /* Methods */

    /*Movement*/

    //Move a shape against the walls, return final movement
    arrow::Vct World::mov_against(arrow::Shp &shp, arrow::Vct &speed) const
    {
        arrow::Vct rv=speed;//Return value is the final speed

        for (const arrow::Shp & wall : walls)//Move against all walls
        {
            rv=shp.mov_against(wall,rv);//Limit the speed against this wall
        }

        return rv;//Return the final speed
    }

    /*Laser*/

    //Shoot the laser into the array
    void World::shoot(const LaserModel &ilm, const arrow::Vct &iposition, arrow::Vct ispeed)
    {
        shoot_to.add_laser(ilm,iposition,ispeed);
    }

    //Get hit from the lasers of the array
    EnergyT World::hit(const arrow::Shp &shp)
    {
        return hit_from.hit(shp);
    }
}
