#pragma once

#include <SFML/Graphics.hpp>
#include <vector>



namespace gr 
{
    namespace gui 
    {
        
        class Object : public sf::Drawable
        {
        public:
            // Link dependent object as a child
            void linkObject(Object* object);
            // Recursively checks if an object has been modified
            bool needToRender();
            // Recursively set all dependent objects as rendered
            void setAsRendered();
            // Updates global bounds based on local bounds
            void updateGlobalBounds(sf::Vector2i parent_position);
            // If size is changed updates local and global size, calls onResize()
            void setSize(sf::Vector2i size);
            // If smth is changed updates local and global position, calls updateGlobalBounds()
            void setLocalPosition(sf::Vector2i local_position);
            // Getters
            sf::Vector2i getSize() const;
            sf::Vector2i getLocalPosition() const;
            sf::Vector2i getGlobalPosition() const;
            // Change target's view to object's
            void setObjectView(sf::RenderTarget& target) const;

            // Should set/change children's local bounds
            virtual void onResize() = 0;

        protected:
            std::vector<Object*> linked_objects_;
            bool need_to_render_ = 1;
            sf::IntRect local_bounds_ = sf::IntRect(0, 0, 0, 0);
            sf::IntRect global_bounds_ = sf::IntRect(0, 0, 0, 0);

            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
        };


        class Scene : public Object
        {
        public:
            virtual void handleEvent(sf::Event event, sf::RenderTarget& target) = 0;
            //virtual void render() override = 0;

            virtual void onResize() override = 0;

        protected:
		    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
        };

    }
}