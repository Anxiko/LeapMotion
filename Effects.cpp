#include "Effects.hpp"

namespace DSI
{
    /*
        Background
    */

    /* Constructors, copy control */

    /*Constructors*/

    //Complete constructor
    Background::Background(sf::Texture &itexture,int ix, int iy, int ispeed)
    :sp(itexture),sp2(itexture),x(ix),y(iy),speed_y(ispeed)
    {
        sp.setScale(ix/itexture.getSize().x,iy/itexture.getSize().y);
        sp2.setScale(ix/itexture.getSize().x,iy/itexture.getSize().y);
        sp2.setOrigin(0,iy);
    }

    /* Methods */

    /*Drawing*/

    //Draw the ship
    void Background::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(sp,states);
        target.draw(sp2,states);
    }

    /*Update*/

    //Move the sprites
    void Background::update()
    {
        auto pos=sp.getPosition();
        pos.y+=speed_y;
        if (pos.y>y)
            pos.y-=y;

        sp.setPosition(pos);
        sp2.setPosition(pos);
    }
}
