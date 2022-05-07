#include "gui_mmo.h"

namespace gr 
{
    namespace gui 
    {

        void DrawingField::onResize() {

        }
        void DrawingField::render() {
            sf::RenderTexture renderer;
            renderer.create(local_bounds_.width, local_bounds_.height);
            sf::CircleShape circle;
            circle.setFillColor(sf::Color::Green);
            circle.setRadius(local_bounds_.width);
            circle.setPosition(sf::Vector2f(0, 0));
            renderer.draw(circle);
            renderer.display();
            sprite_.setTexture(renderer.getTexture(), true); // error
        }

    }
}