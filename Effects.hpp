#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#include "SFML/Graphics.hpp"

#include <sstream>

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

    class Value_HUD : public sf::Drawable
    {
        /*  Config */

        /*Val*/
        private:

            static constexpr int DEF_VAL=0;

        /* Members */

        /*Text*/
        private:

            //Font
            const sf::Font &ft;

        /*Value*/
        private:

            //HP value
            int val;

        /*Prefix*/
        private:

            const std::string prefix;

        /*Position*/
        private:

            int x,y;

        /* Constructors, copy control */

        /*Constructors*/
        public:

            //Complete constructor
            Value_HUD(const sf::Font &ift, int ix, int iy, const std::string &iprefix,int ival=DEF_VAL);

        /* Methods */

        /*Update*/
        public:

            //Set the value
            void update(int ival);

        /*Drawing*/
        public:

            //Draw the HUD
            void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    };
}

#endif // _EFFECTS_H_
