#include "gui_mmo.h"
#include <iostream>

namespace gr 
{
    namespace gui 
    {

        void Chat::onResize() 
        {

        }
        
        void Chat::draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            setObjectView(target);

            sf::RectangleShape rs;
            rs.setFillColor(sf::Color::Red);
            rs.setSize(sf::Vector2f(local_bounds_.width, local_bounds_.height));
            rs.setPosition(sf::Vector2f(0, 0));
            target.draw(rs);
            sf::CircleShape circle;
            circle.setFillColor(sf::Color::Green);
            circle.setRadius(std::min(local_bounds_.width, local_bounds_.height) / 2);
            circle.setPosition(sf::Vector2f(0, 0));
            target.draw(circle);
        }

        
        void MainScene::init(sf::RenderTarget& target)
        {
            linkObject(&chat_);
            linkObject(&game_world_);
            setSize(sf::Vector2i(target.getSize().x, target.getSize().y));
            
            if (game_world_.background_texture.loadFromFile("res/background.png"))
            {
                game_world_.background_texture.setRepeated(true);
                //game_world_.background_texture.
                //std::cout << game_world_.background_texture.getSize().y << "\n";
            }
            else
            {
                //std::cout << "error: loadFromFile(res/background.png)" << "\n";
            }

        }

        void MainScene::handleEvent(sf::Event event, sf::RenderTarget& target)
        {
            if(event.type == sf::Event::Resized)
            {
                setSize(sf::Vector2i(target.getSize().x, target.getSize().y));
                
            }
            //else if(event)
            
            switch(event.type) 
            {
            case sf::Event::Resized :
                setSize(sf::Vector2i(target.getSize().x, target.getSize().y));
                break;
            case sf::Event::MouseWheelScrolled :
                if (/*game_world_.global_bounds_.contains(sf::Mouse::getPosition(target))*/ true)
                {
                    game_world_.cam_.zoom(event.mouseWheelScroll);
                    //std::cout << event.mouseWheelScroll.delta << " " << event.mouseWheelScroll.x << " " << event.mouseWheelScroll.y << "\n";
                }
                break;




            default :
                break;
            }


        }

        void MainScene::onResize()
        {
            chat_.setSize(sf::Vector2i(100, 200));
            chat_.setLocalPosition(sf::Vector2i(0, getSize().y - chat_.getSize().y));
            game_world_.setSize(sf::Vector2i(getSize().x, getSize().y));
            game_world_.setLocalPosition(sf::Vector2i(0, 0));
            updateGlobalBounds(sf::Vector2i(0, 0));
        }

        void MainScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.clear(sf::Color::Black);
            target.draw(game_world_);
            target.draw(chat_);
        }

        void Camera::replace(sf::Vector2f new_position)
        {
            x = new_position.x;
            y = new_position.y;
        }

        void Camera::zoom(sf::Event::MouseWheelScrollEvent scroll_event)
        {
            if (scroll_event.delta > 0) scroll /= k_scroll;
            else scroll *= k_scroll;
            // TO DO
        }

        void Camera::setView(sf::RenderTarget& target, sf::IntRect global_bounds) const 
        {
            sf::View v;
            v.reset(sf::FloatRect(
                x - global_bounds.width * scroll / 2, 
                y - global_bounds.height * scroll / 2, 
                global_bounds.width * scroll, 
                global_bounds.height * scroll));
            int wx = target.getSize().x, wy = target.getSize().y;
	        v.setViewport(sf::FloatRect(
                (float)global_bounds.left / wx, 
                (float)global_bounds.top / wy, 
                (float)global_bounds.width / wx, 
                (float)global_bounds.height / wy));
            target.setView(v);
        }

        void GameWorld::onResize()
        {
            //camera?
        }

        void GameWorld::updateCamera()
        {
            /*
            float x = 0, y = 0, sum = 0;
            size_t self_index = -1;
            for(int i=0; i<game_data_.players.size(); i++)
            {
                if(game_data_.players[i].id == player_id_){
                    self_index = i;
                    break;
                }
            }
            //if(self_index)
            */

            if (game_data_.players.size() && game_data_.players[0].cells.size())
                cam_.replace(sf::Vector2f(
                    game_data_.players[0].cells[0].x, 
                    game_data_.players[0].cells[0].y));
            else
                cam_.replace(sf::Vector2f(0, 0));
            

            //cam_.updateView(target, global_bounds_);
        }

        void GameWorld::draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            cam_.setView(target, global_bounds_);


            sf::RectangleShape field;
            field.setFillColor(sf::Color(30, 30, 30));
            field.setOrigin(game_data_.field_size/2, game_data_.field_size/2);
            field.setSize(sf::Vector2f(game_data_.field_size, game_data_.field_size));
            field.setPosition(0, 0);
            
            target.draw(field);
            //field.setTexture(&background_texture);
            //std::cout << background_texture.isRepeated();
            
            sf::Sprite sprite(background_texture);
            auto sz = background_texture.getSize();
            for(int x=0; x<game_data_.field_size; x+=sz.x){
                for(int y=0; y<game_data_.field_size; y+=sz.y){
                    sprite.setPosition(x - game_data_.field_size/2, y-game_data_.field_size/2);
                    target.draw(sprite);
                }
            }
            field.setFillColor(sf::Color(0, 0, 0, 0));
            field.setOutlineColor(sf::Color::Red);
            field.setOutlineThickness(50);
            target.draw(field);

            
            sf::CircleShape c;
            for(int i = 0; i < game_data_.players.size(); i++)
            {
                c.setFillColor(sf::Color::Green);
                for(int j=0; j < game_data_.players[i].cells.size(); j++)
                {
                    float radius = sqrt(game_data_.players[i].cells[j].mass);
                    c.setOrigin(radius, radius);
                    c.setPosition(sf::Vector2f(game_data_.players[i].cells[j].x, game_data_.players[i].cells[j].y));
                    c.setRadius(radius);
                    c.setOutlineThickness(1);
                    c.setOutlineColor(sf::Color::Black);
                    target.draw(c);
                }
                
            }

        }

        void GameWorld::updateData(data::Game new_game_data)
        {
            game_data_ = new_game_data;
            updateCamera();
        }

        void GameWorld::setPlayerID(uint32_t player_id)
        {
            player_id_ = player_id;
        }
        
        sf::Vector2f GameWorld::getUnderMouseCoords(sf::RenderWindow& target)
        {
            cam_.setView(target, global_bounds_);
            return target.mapPixelToCoords(sf::Mouse::getPosition(target));
        }


    }
}