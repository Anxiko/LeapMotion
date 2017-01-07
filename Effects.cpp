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

    /*
        Value_HUD
    */

    /* Constructors, copy control */

    /*Constructors*/

    //Complete constructor
    Value_HUD::Value_HUD(const sf::Font &ift, int ix, int iy, const std::string &iprefix,int ival)
    :ft(ift),val(ival),prefix(iprefix),x(ix),y(iy)
    {}

    /* Methods */

    /*Update*/

    //Set the HP value
    void Value_HUD::update(int ival)
    {
        val=ival;
    }

    /*Drawing*/

    //Draw the HUD
    void Value_HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        std::stringstream ss;
        ss<<prefix<<val;

        sf::Text text(ss.str(),ft);
        text.setPosition(x,y);
        target.draw(text,states);
    }
}
