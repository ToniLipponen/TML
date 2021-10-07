#pragma once
#include <TML/Interface/Layout.h>

namespace tml::Interface
{
    template<ui32 Rows, ui32 Columns>
    class GridLayout : public Layout
    {
    public:
        GridLayout(i32 x, i32 y, ui32 w, ui32 h)
        {
            m_pos = Vector2(x,y);
            m_size = Vector2(w,h);
        }

    private:
        virtual void Draw()
        {

        }
        virtual void UpdateComponents()
        {
            const auto cw = (m_size.x / Columns);
            const auto ch = (m_size.y / Rows);
            ui32 row = 0;
            for(int i = 0; i < m_components.size(); i++)
            {
                if((i % Columns) == 0 && i != 0)
                    ++row;
                m_components.at(i)->SetSize({cw-2, ch-2});
                m_components.at(i)->SetPosition(m_pos + Vector2(((i % Columns)*cw) + 1, (row*ch) + 1));
            }
        }
    };
}