#include <TML/Interface/Components/NumericInput.h>
#include <TML/System/Clipboard.h>

namespace tml
{
    namespace Interface
    {
        template<typename T>
        NumericInput<T>::NumericInput(i32 x, i32 y, ui32 w, ui32 h, T value)
        : BaseComponent(x,y,w,h)
        {
            m_value = value;
            m_valueStr = std::to_string(m_value);
            m_cursorIndex = m_valueStr.length();
            m_text.SetPosition(m_pos);
            m_text.SetColor(Color::Black);
            m_text.SetString(m_valueStr);
            m_text.SetSize(h);
            const Vector2i textSize = m_text.GetDimensions();
            m_text.SetPosition(m_pos + Vector2i(0, m_size.y / 2.f - textSize.y / 2));
            m_hSizePolicy = Expand;

            AddListener("MouseDown", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseOver)
                {
                    m_state.MouseDown = e.mouseButton.button;
                    e = Event{};
                }
                else
                    UnFocus();
            });
            AddListener("Click", [&](BaseComponent* c, Event& e)
            {
                if(m_state.MouseOver)
                {
                    Focus();
                    Raise();
                    m_cursorIndex = m_valueStr.length();
                    e = Event{};
                }
                else
                    UnFocus();
            });

            AddListener("KeyPressed", [&](BaseComponent* c, Event& e)
            {
                if(m_state.Focused)
                {
                    switch(e.key.code)
                    {
                        case Keyboard::KEY_0:
                        case Keyboard::KEY_1:
                        case Keyboard::KEY_2:
                        case Keyboard::KEY_3:
                        case Keyboard::KEY_4:
                        case Keyboard::KEY_5:
                        case Keyboard::KEY_6:
                        case Keyboard::KEY_7:
                        case Keyboard::KEY_8:
                        case Keyboard::KEY_9:
                            m_valueStr.insert(m_cursorIndex, 1, char(e.key.code));
                            m_cursorIndex++;
                            m_value = StringToType<T>(m_valueStr);
                            break;
                        case Keyboard::KEY_BACKSPACE:
                            if(m_valueStr.length() == 0)
                                m_value = 0;
                            else
                            {
                                m_valueStr.pop_back();
                                if(m_valueStr.length() == 0)
                                    m_value = 0;
                                else
                                    m_value = StringToType<T>(m_valueStr);
                                m_cursorIndex = Math::Clamp<ui32>(m_cursorIndex--, 0, m_valueStr.length());
                            }
                            break;
                        case Keyboard::KEY_PERIOD:
                            if(std::is_floating_point<T>().value && m_valueStr.find('.') == m_valueStr.npos)
                            {
                                m_valueStr.insert(m_cursorIndex, 1, '.');
                                m_cursorIndex++;
                                m_value = StringToType<T>(m_valueStr + "0");
                            }
                            break;
                        case Keyboard::KEY_LEFT:
                            m_cursorIndex = Math::Clamp<i32>(--m_cursorIndex, 0, m_valueStr.length());
                            break;
                        case Keyboard::KEY_RIGHT:
                            m_cursorIndex = Math::Clamp<i32>(++m_cursorIndex, 0, m_valueStr.length());
                            break;
                        case Keyboard::KEY_UP:
                            Increment();
                            break;
                        case Keyboard::KEY_DOWN:
                            Decrement();
                            break;
                        default:
                            break;

                    }
                    m_text.SetString(m_valueStr);
                    e = Event{};
                }
            });
            AddListener("Moved", [&](BaseComponent* c, Event& e)
            {
                m_text.SetPosition(e.pos.x, e.pos.y);
            });
            AddListener("Resized", [&](BaseComponent* c, Event& e)
            {
                m_text.SetSize(e.size.h);
                const Vector2i textSize = m_text.GetDimensions();
                m_text.SetPosition(m_pos + Vector2i(0, m_size.y / 2.f - textSize.y / 2));
            });
        }

        template<typename T>
        void NumericInput<T>::SetValue(T value)
        {
            m_value = value;
            m_valueStr = std::to_string(m_value);
            m_text.SetString(m_valueStr);
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
            m_cursorIndex = m_valueStr.length();
            m_text.SetString(m_valueStr);
        }

        template<typename T>
        void NumericInput<T>::Decrement()
        {
            m_value -= m_increment;
            m_valueStr = std::to_string(m_value);
            m_cursorIndex = m_valueStr.length();
            m_text.SetString(m_valueStr);
        }

        template<typename T>
        void NumericInput<T>::pDraw(Renderer &window)
        {
            const auto cursorX = Math::Clamp<float>(m_pos.x + m_text.GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x);
            const Vector2i pos = m_pos + Vector2i(1,1);
            const Vector2i size = m_size - Vector2i(1,1);

            window.DrawRect(pos, size, m_pColor);
            window.SetBounds(pos, size);
            window.Draw(m_text);

            if(m_state.Focused)
            {
                window.DrawLine(Vector2f{cursorX, m_pos.y + (m_size.y / 10.0f)}, {cursorX, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, Color::Black, 0);
                window.ResetBounds();
                window.DrawGrid(pos, size, 1, 1, m_activeColor, 1);
            }
            else
            {
                window.ResetBounds();
                window.DrawGrid(pos, size, 1, 1, m_sColor, 1);
            }
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
}