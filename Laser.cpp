#include "Laser.hpp"

namespace DSI
{
    /*
        LaserModel
    */

    /* Constructors, copy control */

    /*Constructors*/

    LaserModel::LaserModel(EnergyT idamage, int iticks, unsigned int isz, arrow::Vct::Mod ispeed, const sf::Color &icolor)
    :damage(idamage),ticks(iticks),sz(isz),speed(ispeed),circle(isz)
    {
        circle.setFillColor(icolor);
        circle.setOrigin(isz,isz);
        circle.setOutlineThickness(0);
    }

    /* Members */

    /*Drawing*/

    void LaserModel::draw_laser(sf::RenderTarget& target, sf::RenderStates states, const arrow::Vct &position) const
    {
        circle.setPosition(position.x,position.y);
        target.draw(circle,states);
    }

    /*
        Laser
    */

    /* Constructos, copy control */

    /*Constructors*/

    Laser::Laser(const LaserModel &ilm, const arrow::Vct &iposition, arrow::Vct ispeed)
    :lm(ilm),c(iposition,ilm.sz),v()
    {
        ispeed.unitary();
        ispeed*=ilm.sz;
        v=ispeed;
    }

    /* Methods */

    /*Update*/

    //Move the laser
    void Laser::update()
    {
        c.mov(v);
    }

    /*Drawing*/

    //Draw the ship
    void Laser::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        lm.draw_laser(target,states,c.get_pos_center());
    }

    /*Hit*/

    //Check if it'll hit a laser
    bool Laser::hit(const arrow::Shp &shp) const
    {
        auto tth=c.tth(shp,v);
        return tth>=0 && tth<1;
    }

    //Return the energy of this laser
    EnergyT Laser::get_dmg() const
    {
        return lm.damage;
    }

    /*Contact*/

    //Check for contact with another shape
    bool Laser::contact(const arrow::Shp &shp) const
    {
        return c.contact(shp);
    }

    /*
        ArrayLaser
    */

    /* Methods */

    /*Update*/

    //Move the lasers
    void ArrayLaser::update(const arrow::Shp &screen)
    {
        for (auto it=lasers.begin();it!=lasers.end();)
        {
            it->update();

            //If not in contact with the screen after moving, delete
            if (!it->contact(screen))
                it=lasers.erase(it);
            else
                ++it;
        }
    }

    /*Drawing*/

    //Draw the ship
    void ArrayLaser::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (const auto &laser : lasers)
        {
            target.draw(laser,states);
        }
    }

    /*Array*/

    //Add a laser
    void ArrayLaser::add_laser(const LaserModel &ilm, const arrow::Vct &iposition, arrow::Vct ispeed)
    {
        lasers.emplace_back(ilm,iposition,ispeed);
    }

    /*Hit*/

    //Return damage made to a shape
    EnergyT ArrayLaser::hit(const arrow::Shp &shp)
    {
        EnergyT rv=0;//Total damage

        for (auto it=lasers.begin();it!=lasers.end();)
        {
            if (it->hit(shp))//Laser hits the shape
            {
                rv+=it->get_dmg();
                it=lasers.erase(it);
            }
            else
                ++it;
        }

        return rv;
    }

}
