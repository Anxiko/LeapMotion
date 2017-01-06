#include "Ship.hpp"

namespace DSI
{

    /*
        Ship
    */

    /* Constructors, copy control */

    /*Constructors*/

    //Complete constructor
    Ship::Ship(const arrow::Vct &icenter, sf::Texture &itexture, bool iorientation, World &iworld, const LaserModel &ilm, EnergyT imax_ener, IController *iptr_controller)
    :r(icenter,arrow::Vct(itexture.getSize().x,itexture.getSize().y)),sp(), orientation(iorientation), world(iworld), lm(ilm), actual_ener(imax_ener), max_ener(imax_ener), ptr_controller(iptr_controller)
    {
        create_sprite(itexture);
    }

    //Destructor
    Ship::~Ship()
    {
        delete ptr_controller;
    }

    /* Methods */

    /*Drawing*/

    //Draw the ship
    void Ship::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(sp,states);
    }

    //Create the sprite
    void Ship::create_sprite(const sf::Texture &texture)
    {
        sf::Vector2u texture_sz=texture.getSize();
        sp.setTexture(texture);
        sp.setOrigin(texture_sz.x*0.5,texture_sz.y*0.5);
        sp.setPosition(r.get_pos_center().x,r.get_pos_center().y);

        if (orientation==ORIENTATION_DOWN)
            sp.setScale(1,-1);
    }

    /*Update*/

    //Update the ship's status (process info from controller)
    void Ship::update()
    {
        //Controller updates
        if (ptr_controller)
        {
            //First, update the controller
            ptr_controller->update();

            //Move
            arrow::Vct speed(ptr_controller->getX(),ptr_controller->getY());
            speed-=r.get_pos_center();
            speed=world.mov_against(r,speed);
            r.mov(speed);
            sp.setPosition(r.get_pos_center().x,r.get_pos_center().y);

            //Shoot
            if (ptr_controller->fire())
            {
                arrow::Vct position=r.get_pos_corner()+arrow::Vct(r.get_diagonal().x/2,0);
                if (orientation==ORIENTATION_DOWN)
                    position+=arrow::Vct(0,r.get_diagonal().y);

                world.shoot(lm,position,arrow::Vct(0,orientation==ORIENTATION_UP?-1:+1));
            }
        }

        //World updates
        actual_ener-=world.hit(r);

        std::cout << this <<": "<<actual_ener<<'/'<<max_ener<<std::endl;
    }

    /*Controller*/

    //Replace this ship's current controller by a new one
    void Ship::set_controller(IController *iptr_controller)
    {
        delete ptr_controller;

        ptr_controller=iptr_controller;
    }

    /*Contact*/

    //Check for contact with a shape
    bool Ship::contact(const arrow::Shp &shp) const
    {
        return r.contact(shp);
    }

    /*Position*/

    //Get position
    arrow::Vct Ship::get_position() const
    {
        return r.get_pos_center();
    }

    //Get the orientation of the ship
    bool Ship::get_orientation() const
    {
        return orientation;
    }

    /*Energy*/

    //Check if a ship is a alive
    bool Ship::is_alive() const
    {
        return actual_ener;
    }
}
