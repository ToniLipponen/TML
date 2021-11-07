#pragma once
#include <TML/Vector2.h>

namespace tml
{
    namespace Interface
    {

        class Object
        {
        public:
            Object()
            : m_pos(0), m_size(0){}

            Object(const Vector2& position, const Vector2& size)
            : m_pos(position), m_size(size){}

            Object(i32 x, i32 y, i32 w, i32 h)
            : m_pos(x,y), m_size(w,h){}

            void SetPosition(const Vector2& position)
            {
                m_pos = position;
                OnMoved();
            }
            void SetSize(const Vector2& size)
            {
                m_size = size;
                OnResized();
            }
            inline Vector2 GetPosition() const
            {
                return m_pos;
            }
            inline Vector2 GetSize() const
            {
                return m_size;
            }

        protected:
            virtual void OnResized(){}
            virtual void OnMoved(){}
            Vector2 m_pos;
            Vector2 m_size;
        };
    }
}