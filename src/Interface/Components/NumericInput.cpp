#include <TML/Interface/Components/NumericInput.h>
#include <TML/Drawable/Text.h>
#include <TML/Utilities/Utilities.h>
#include <TML/Renderer.h>
#include <TML/IO/Clipboard.h>

extern tml::Text* DEFAULT_TEXT;
namespace tml::Interface
{
    template<typename T>
    NumericInput<T>::NumericInput()
    {
        Object(0,0,0,0);
        m_value = 0;
    }

    template<typename T>
    NumericInput<T>::NumericInput(BaseComponent *parent, i32 x, i32 y, ui32 w, ui32 h, T value)
    {
        m_pos = Vector2(x,y);
        m_size = Vector2(w,h);
        m_value = value;
        m_valueStr = std::to_string(m_value);

        auto buttonPos = m_pos - Vector2(h/2, 0);
        buttonPos.x += m_size.x;
        auto inc = new Button(buttonPos.x, buttonPos.y, h/2, h/2, "+");
        auto dec = new Button(buttonPos.x, buttonPos.y + h/2, h/2, h/2, "-");
        AddChild(inc);
        AddChild(dec);
        m_size.x -= h/2;

        inc->SetTextSize(h * 0.8);
        dec->SetTextSize(h * 0.8);
        inc->SetOnClick({[](BaseComponent* c){((NumericInput*)c->GetParent())->Increment();}});
        dec->SetOnClick({[](BaseComponent* c){((NumericInput*)c->GetParent())->Decrement();}});
        if(parent)
            parent->AddChild(this);
    }

    template<typename T>
    void NumericInput<T>::SetValue(T value)
    {
        m_value = value;
        m_valueStr = std::to_string(m_value);
    }

    template<typename T>
    T NumericInput<T>::GetValue() const
    {
        return m_value;
    }

    template<typename T>
    void NumericInput<T>::Increment()
    {
        m_value += m_increment;
        m_valueStr = std::to_string(m_value);
        m_cursorIndex = m_valueStr.size();
    }

    template<typename T>
    void NumericInput<T>::Decrement()
    {
        m_value -= m_increment;
        m_valueStr = std::to_string(m_value);
        m_cursorIndex = m_valueStr.size();
    }

    template<typename T>
    void NumericInput<T>::OnMouseClick(const Vector2 &p)
    {
        Keyboard::BeginString();
    }

    template<typename T>
    void NumericInput<T>::OnUpdate(float dt)
    {
        m_repeatTimer = Util::Max(m_repeatTimer += dt, 0.11f);
        if(m_state.Focused)
        {
            if(m_repeatTimer > 0.1f)
            {
                if(Keyboard::IsKeyDown(Keyboard::KEY_LEFT_CONTROL)
                   && Keyboard::IsKeyDown(Keyboard::KEY_V)
                   && !Clipboard::IsEmpty())
                    SetValue(Util::StringToType<T>(Util::wstringToString(Clipboard::GetString())));
                else if(Keyboard::IsKeyDown(Keyboard::KEY_BACKSPACE)
                        && !m_valueStr.empty())
                {
                    m_valueStr.erase(Util::Clamp<ui32>(m_cursorIndex-1, 0, m_valueStr.length() -1), 1);
                    m_cursorIndex--;
                }
                else if(Keyboard::IsKeyDown(Keyboard::KEY_LEFT))
                {
                    m_cursorIndex--;
                }
                else if(Keyboard::IsKeyDown(Keyboard::KEY_RIGHT))
                {
                    m_cursorIndex++;
                }
                m_repeatTimer = 0;
            }
            else
            {
                if(Keyboard::IsKeyPressed(Keyboard::KEY_BACKSPACE) && !m_valueStr.empty())
                {
                    m_valueStr.erase(Util::Clamp<ui32>(m_cursorIndex-1, 0, m_valueStr.length() -1), 1);
                    m_cursorIndex--;
                }
                else if(Keyboard::IsKeyPressed(Keyboard::KEY_LEFT))
                {
                    m_cursorIndex--;
                }
                else if(Keyboard::IsKeyPressed(Keyboard::KEY_RIGHT))
                {
                    m_cursorIndex++;
                }
            }
            const auto str = Util::wstringToString(Keyboard::EndString());
            if(!str.empty())
            {
                char c = str.at(0);
                if constexpr(std::is_integral<T>::value)
                {
                    if(Util::InRange<char>(48, c, 57))
                    {
                        m_valueStr.insert(m_cursorIndex, 1, c);
                        m_cursorIndex += 1;
                    }
                }
                else
                {
                    if(Util::InRange<char>(48, c, 57) || (c == '.' && m_valueStr.find('.') == m_valueStr.npos))
                    {
                        m_valueStr.insert(m_cursorIndex, 1, c);
                        m_cursorIndex += 1;
                    }
                }
            }
//            m_value = Util::StringToType<T>(m_valueStr);
            m_cursorIndex = Util::Clamp<i32>(m_cursorIndex, 0, m_valueStr.size());
        }
        m_value = Util::StringToType<T>(m_valueStr);
    }


    template<typename T>
    void NumericInput<T>::Draw()
    {
        DEFAULT_TEXT->SetPosition(m_pos);
        DEFAULT_TEXT->SetSize(m_size.y);
        DEFAULT_TEXT->SetString(m_valueStr.substr(0, m_cursorIndex));
        const auto cursorX = Util::Clamp(m_pos.x + DEFAULT_TEXT->GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x);
        DEFAULT_TEXT->SetString(m_valueStr);

        Renderer::DrawRect(m_pos, m_size, m_pColor);
        Renderer::DrawTextCropped(m_valueStr, m_pos,m_size.y, BLACK, m_pos, m_pos + m_size);

        if(s_activeComponent == this)
        {
            Renderer::DrawLine({cursorX, m_pos.y + (m_size.y / 10.0f)}, {cursorX, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, BLACK, 0);
            Renderer::DrawGrid(m_pos, m_size, 1, 1, m_activeColor, 2);
        }
        else
            Renderer::DrawGrid(m_pos, m_size, 1, 1, m_sColor, 2);
    }

    template class NumericInput<i64>;
    template class NumericInput<ui64>;
    template class NumericInput<i32>;
    template class NumericInput<ui32>;
    template class NumericInput<i16>;
    template class NumericInput<ui16>;
    template class NumericInput<i8>;
    template class NumericInput<ui8>;
    template class NumericInput<float>;
    template class NumericInput<double>;
}