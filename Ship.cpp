#include "Ship.hpp"

namespace DSI
{

    /*
        Ship
    */

    /* Constructors, copy control */

    /*Constructors*/

    //Complete constructor
    Ship::Ship(arrow::Vct &icenter, arrow::Vct &iup, arrow::Vct &idn, arrow::Vct &ilf, arrow::Vct &irt, sf::Texture &itexture)
    :r(icenter,arrow::Vct(itexture.getSize().x,itexture.getSize().y)),up(iup),dn(idn),lf(ilf),rt(irt),sp()
    {
        create_sprite(itexture);
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
}
