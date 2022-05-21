#pragma once

#include "gui_base.h"
#include "../common/game_data.h"

namespace gr 
{
    namespace gui 
    {

        class Chat : public Object 
        {
        public:
            void onResize() override;
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        };
        
        class Camera 
        {
        public:

            void replace(sf::Vector2f new_position);
            void zoom(sf::Event::MouseWheelScrollEvent scroll_event);
            void setView(sf::RenderTarget& target, sf::IntRect global_bounds) const;
            //void updateView(sf::RenderTarget& target, sf::IntRect global_bounds);
        private:
            //sf::View view_;
            float x = 0, y = 0;
            float scroll = 1;
            float k_scroll = 1.2f;
        };

        class GameWorld : public Object
        {
        public:
            void updateCamera();
            void onResize() override;
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
            void updateData(data::Game new_game_data);
            void setPlayerID(uint32_t player_id);
            sf::Vector2f getUnderMouseCoords(sf::RenderWindow& target);


            Camera cam_;
            //float field_size = 13250;
        //private:
            uint32_t player_id_;
            data::Game game_data_;
            sf::Texture background_texture;
            sf::Texture skin;
        };

        class MainScene : public Scene
        {
        public:
            void init(sf::RenderTarget& target);
            void handleEvent(sf::Event event, sf::RenderTarget& target) override;
            void onResize() override;
            

            Chat chat_;
            GameWorld game_world_;
            

        protected:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        };

        

        

    }
}