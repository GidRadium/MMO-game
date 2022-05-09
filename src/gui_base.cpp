#include "gui_base.h"
//#include <iostream>

namespace gr 
{
    namespace gui 
    {

        void Object::linkObject(Object* object)
        {
            linked_objects_.push_back(object);
        }

        bool Object::needToRender()
        {
            if(need_to_render_) return true;
            for(auto object : linked_objects_)
                if(object->needToRender())
                    need_to_render_ = true;
            return false;
        }

        void Object::updateGlobalBounds(sf::Vector2i global_parent_position)
        {
            global_bounds_ = local_bounds_;
            global_bounds_.left += global_parent_position.x;
            global_bounds_.top += global_parent_position.y;

            global_parent_position = sf::Vector2i(global_bounds_.left, global_bounds_.top);
            for(auto object : linked_objects_)
                object->updateGlobalBounds(global_parent_position);
        }

        void Object::setSize(sf::Vector2i size)
        {
            if(local_bounds_.width != size.x || local_bounds_.height != size.y)
            {
                local_bounds_.width = size.x;
                local_bounds_.height = size.y;
                global_bounds_.width = size.x;
                global_bounds_.height = size.y;

                need_to_render_ = true;
                onResize();
            }
        }

        void Object::setLocalPosition(sf::Vector2i local_position)
        {
            if(local_bounds_.left != local_position.x || local_bounds_.top != local_position.y)
            {
                sf::Vector2i change(
                    local_position.x - local_bounds_.left, 
                    local_position.y - local_bounds_.top);
                
                local_bounds_.left = local_position.x;
                local_bounds_.top = local_position.y;
                
                updateGlobalBounds(sf::Vector2i(
                    global_bounds_.left + change.x,
                    global_bounds_.top + change.y));
            }
        }


        sf::Vector2i Object::getSize() const
        {
            return sf::Vector2i(local_bounds_.width, local_bounds_.height);
        }

        sf::Vector2i Object::getLocalPosition() const
        {
            return sf::Vector2i(local_bounds_.left, local_bounds_.top);
        }

        sf::Vector2i Object::getGlobalPosition() const
        {
            return sf::Vector2i(global_bounds_.left, global_bounds_.top);
        }

        void Object::setObjectView(sf::RenderTarget& target) const
        {
            sf::View v;
            v.reset(sf::FloatRect(0, 0, getSize().x, getSize().y));
            int wx = target.getSize().x, wy = target.getSize().y;
	        v.setViewport(sf::FloatRect(
                (float)global_bounds_.left / wx, 
                (float)global_bounds_.top / wy, 
                (float)global_bounds_.width / wx, 
                (float)global_bounds_.height / wy));
            target.setView(v);
            /*
            std::cout 
                << (float)local_bounds_.left << " "
                << (float)local_bounds_.top << " "
                << (float)local_bounds_.width << " "
                << (float)local_bounds_.height << "\n";
            std::cout 
                << (float)global_bounds_.left << " "
                << (float)global_bounds_.top << " "
                << (float)global_bounds_.width << " "
                << (float)global_bounds_.height << "\n";
            std::cout 
                << (float)global_bounds_.left / wx << " "
                << (float)global_bounds_.top / wy << " "
                << (float)global_bounds_.width / wx << " "
                << (float)global_bounds_.height / wy << "\n";
            */
        }



    }
}