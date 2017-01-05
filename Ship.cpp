#include "Ship.hpp"

namespace DSI
{

    /*
        Ship
    */

    /* Constructors, copy control */

    /*Constructors*/

    //Complete constructor
    Ship::Ship(const arrow::Vct &icenter, sf::Texture &itexture, World &iworld, IController *iptr_controller)
    :r(icenter,arrow::Vct(itexture.getSize().x,itexture.getSize().y)),sp(), world(iworld) ,ptr_controller(iptr_controller)
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
        }
    }
}
