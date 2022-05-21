#pragma once
//#include <SFML/System.hpp>
#include <SFML/System/Vector2.hpp>
//#include <SFML/System/String.hpp>
//#include "Vector2.hpp"
#include <vector>
#include <string>

namespace gr
{

    namespace data
    {

        struct Cell
        {
            int64_t mass = 0;
            int64_t x = 0;
            int64_t y = 0;
            void move_to(sf::Vector2f moving_to, float dist, int field_size);
        };

        struct Player
        {
            uint32_t id;
            std::string name;
            std::vector<Cell> cells;
            sf::Vector2f last_moving_to = sf::Vector2f(0, 0);
            void update(sf::Vector2f moving_to, float dt, float speed, int field_size);
            void update(float dt, float speed, int field_size);

        };
        
        struct Game
        {
            
            std::string room_name;
            int field_size = 30000;
            std::vector<Player> players;
            void update(float dt);
        };
        
    }
}