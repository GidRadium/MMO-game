#include "game_data.h"
#include <iostream>

namespace gr
{
    namespace data
    {

        void Cell::move(sf::Vector2f moving_to, float dist, int field_size)
        {
            float L = sqrt((moving_to.x - x)*(moving_to.x - x) + (moving_to.y - y)*(moving_to.y - y));
            if (L == 0) return;
            x += dist*(moving_to.x - x) / L;
            y += dist*(moving_to.y - y) / L;
            if(x > field_size/2) x = field_size/2;
            if(x < -field_size/2) x = -field_size/2;
            if(y > field_size/2) y = field_size/2;
            if(y < -field_size/2) y = -field_size/2;
        }
        void Player::update(sf::Vector2f moving_to, float dt, float speed, int field_size)
        {
            for(int i=0; i<cells.size(); i++)
                cells[i].move(moving_to, dt*speed, field_size);
            last_moving_to = moving_to;
        }
        void Player::update(float dt, float speed, int field_size)
        {
            for(int i=0; i<cells.size(); i++)
                cells[i].move(last_moving_to, dt*speed, field_size);
        }
        void Game::update(float dt)
        {
            for(int i=0; i<players.size(); i++)
                players[i].update(dt, 2000, field_size);
        }


    }
}