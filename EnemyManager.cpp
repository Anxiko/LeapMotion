#include "EnemyManager.hpp"


namespace DSI
{
    /*
        Init
    */

    std::default_random_engine EnemyLinearController::re(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    std::default_random_engine EnemyManager::re(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    /*
        EnemyLinearController
    */

    /* Constructors, copy control */

    /*Constructoes*/

    //Complete constructor
    EnemyLinearController::EnemyLinearController(int ix, int iy, const Ship &iship, int ispeed, int iprob_firing)
    :IController(ix,iy),ship(iship),speed(ispeed),prob_firing(iprob_firing)
    {}

    /* Interface */

    /*Update*/

    //Update the controller (call each frame before getting data)
    void EnemyLinearController::update()
    {}

    /*Positions*/

    //Get X position (from 0 to x)
    int EnemyLinearController::getX() const
    {
        return ship.get_position().x;
    }

    //Get Y position (from 0 to y)
    int EnemyLinearController::getY() const
    {
        return ship.get_position().y +(ship.get_orientation()==Ship::ORIENTATION_UP? -speed:+speed);
    }

    /*Commands*/

    //Fire
    bool EnemyLinearController::fire() const
    {
        std::uniform_int_distribution<> dt(0,prob_firing);
        return !dt(re);
    }

    //Ability
    bool EnemyLinearController::ability() const
    {
        return false;
    }

    /*
        EnemyManager
    */

    /* Constructors, copy control */

    /*Constructor*/

    //Complete constructor
    EnemyManager::EnemyManager(int itarget_ships, const arrow::Rct &iscreen, sf::Texture &ilinear_texture, const LaserModel &ilinear_lm, EnergyT ilinear_energy, World &iworld)
    :number_ships(0),target_ships(itarget_ships),screen(iscreen),linear_texture(ilinear_texture),linear_lm(ilinear_lm),linear_energy(ilinear_energy),orientation(DEF_ORIENTATION),world(iworld)
    {}

    /* Methods */

    /*Array*/

    //Add a linear ship to the array
    void EnemyManager::add_linear()
    {
        std::uniform_int_distribution<> dt(0,screen.get_diagonal().x);//X position is random
        int x=dt(re);
        int y=screen.get_pos_corner().y-linear_texture.getSize().y+1;//Spawn just at the border, but inside the screen

        ships.emplace_back(arrow::Vct(x,y),linear_texture,orientation,world,linear_lm,linear_energy);
        IController *ptr_controller=new EnemyLinearController(screen.get_diagonal().x,screen.get_diagonal().y,ships.back());
        ships.back().set_controller(ptr_controller);

        ++number_ships;
    }

    //Add ships until the target is met
    void EnemyManager::add_ships()
    {
        while (number_ships<target_ships)
            add_linear();
    }

    /*Ships*/

    //Update all ships, remove those that are killed/out of screen
    int EnemyManager::update()
    {
        int victims=0;//Ships that die in this update

        for (auto it=ships.begin();it!=ships.end();)
        {
            if (it->is_alive()&&it->contact(screen))//Ship is alive and inside the screen
                (it++)->update();//Update it
            else//If not active
            {
                if (!(it->is_alive()))//Kill count
                    ++victims;
                it=ships.erase(it);//Delete it
                --number_ships;
            }
        }

        //Add ships back
        add_ships();

        //Return the number of ships that died
        return victims;
    }

    /*Drawing*/

    //Draw the ship
    void EnemyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (const auto &ship : ships)
            target.draw(ship,states);
    }

    /*Ability*/

    //Kill all ships. Return number of ships killed
    int EnemyManager::purge()
    {
        int rv=number_ships;
        ships.clear();
        number_ships=0;
        return rv;
    }
}
