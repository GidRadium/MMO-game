#pragma once

#include "gui_base.h"

namespace gr 
{
    namespace gui 
    {

        class DrawingField : public Object {
        public:
            //int diameter = 3;
            sf::Texture picture;
            void onResize() override;
            void render() override;
            

        };

    }
}