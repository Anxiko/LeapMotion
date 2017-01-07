#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#include "SFML/Graphics.hpp"

namespace DSI
{
    class Background : public sf::Drawable
    {
        /* Members */

        /*Sprites*/
        private:

            //Sprite to draw twice
            sf::Sprite sp,sp2;

        /*Window*/
        private:

            //Dimensions of the size
            const int x,y;

            //Speed of the window
            const int speed_y;

        /* Constructors, copy control */

        /*Constructors*/
        public:

            //Complete constructor
            Background(sf::Texture &itexture,int ix, int iy, int ispeed);

        /* Methods */

        /*Drawing*/
        public:

            //Draw the ship
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        /*Update*/
        public:

            //Move the sprites
            void update();
    };
}

#endif // _EFFECTS_H_
