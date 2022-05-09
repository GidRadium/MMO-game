#pragma once

#include "gui_base.h"

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

        class MainScene : public Scene
        {
            //MainScene();
        public:
            void init(sf::RenderTarget& target);
            void handleEvent(sf::Event event, sf::RenderTarget& target) override;
            void onResize() override;
            

            Chat chat_;

        protected:
            void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        };

    }
}