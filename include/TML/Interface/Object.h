#pragma once
#include <TML/Utilities/Vector2.h>

namespace tml
{
    namespace Interface
    {
        class Object
        {
        public:
            Object() : m_pos(0), m_size(0){}

            Object(const Vector2i& position, const Vector2i& size) : m_pos(position), m_size(size){}

            Object(i32 x, i32 y, i32 w, i32 h) : m_pos(x,y), m_size(w,h){}

            void SetPosition(const Vector2i& position)
            {
                m_pos = position;
                OnMoved();
            }

            void SetSize(const Vector2i& size)
            {
                m_size = size;
                OnResized();
            }

            inline Vector2i GetPosition() const
            {
                return m_pos;
            }

            inline Vector2i GetSize() const
            {
                return m_size;
            }

        protected:
            virtual void OnResized(){}
            virtual void OnMoved(){}
            Vector2i m_pos;
            Vector2i m_size;
        };
    }
}