#pragma once

#include <SFML/Graphics.hpp>
#include <vector>



namespace gr 
{
    namespace gui 
    {
        
        class Object
        {
        public:
            void linkObject(Object* object);
            bool needToRender();

            void updateGlobalBounds(sf::Vector2i parent_position);
            void setSize(sf::Vector2i size);
            void setLocalPosition(sf::Vector2i local_position);
            sf::Vector2i getSize();
            sf::Vector2i getLocalPosition();
            sf::Vector2i getGlobalPosition();

            virtual void render() = 0;
            virtual void onResize() = 0;

        protected:
            std::vector<Object*> linked_objects_;
            bool need_to_render_ = 1;
            sf::Sprite sprite_;
            sf::IntRect local_bounds_ = sf::IntRect(0, 0, 0, 0);
            sf::IntRect global_bounds_ = sf::IntRect(0, 0, 0, 0);
        };


        class Scene : public Object, public sf::Drawable
        {
        public:
            virtual void handleEvent(sf::Event) = 0;
            virtual void render() override = 0;
            virtual void onResize() override = 0;

        private:
		    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
        };

    }
}