#include <TML/Interface/Components/NumericInput.h>
#include <TML/System/Clipboard.h>

namespace tml::Interface
{
    template<typename T>
    NumericInput<T>::NumericInput(int32_t x, int32_t y, uint32_t w, uint32_t h, T value) noexcept
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
        m_hSizePolicy = SizePolicy::Expand;

        AddListener("MouseDown", [&](BaseComponent* c, Event& e)
        {
            if(m_state.MouseOver)
            {
                m_state.MouseDown = static_cast<char>(e.mouseButton.button);
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
                switch(e.key.value)
                {
                    case Keyboard::Key::Zero:
                    case Keyboard::Key::One:
                    case Keyboard::Key::Two:
                    case Keyboard::Key::Three:
                    case Keyboard::Key::Four:
                    case Keyboard::Key::Five:
                    case Keyboard::Key::Six:
                    case Keyboard::Key::Seven:
                    case Keyboard::Key::Eight:
                    case Keyboard::Key::Nine:
                        m_valueStr.insert(m_cursorIndex, 1, char(e.key.code));
                        m_cursorIndex++;
                        m_value = StringToType<T>(m_valueStr);
                        break;
                    case Keyboard::Key::Backspace:
                        if(m_valueStr.length() == 0)
                            m_value = 0;
                        else
                        {
                            m_valueStr.pop_back();
                            if(m_valueStr.length() == 0)
                                m_value = 0;
                            else
                                m_value = StringToType<T>(m_valueStr);
                            m_cursorIndex = Math::Clamp<uint32_t>(m_cursorIndex--, 0, m_valueStr.length());
                        }
                        break;
                    case Keyboard::Key::Period:
                        if(std::is_floating_point<T>().value && m_valueStr.find('.') == m_valueStr.npos)
                        {
                            m_valueStr.insert(m_cursorIndex, 1, '.');
                            m_cursorIndex++;
                            m_value = StringToType<T>(m_valueStr + "0");
                        }
                        break;
                    case Keyboard::Key::LeftArrow:
                        m_cursorIndex = Math::Clamp<int32_t>(--m_cursorIndex, 0, m_valueStr.length());
                        break;
                    case Keyboard::Key::RightArrow:
                        m_cursorIndex = Math::Clamp<int32_t>(++m_cursorIndex, 0, m_valueStr.length());
                        break;
                    case Keyboard::Key::UpArrow:
                        Increment();
                        break;
                    case Keyboard::Key::DownArrow:
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
    void NumericInput<T>::SetValue(T value) noexcept
    {
        m_value = value;
        m_valueStr = std::to_string(m_value);
        m_text.SetString(m_valueStr);
    }

    template<typename T>
    T NumericInput<T>::GetValue() const noexcept
    {
        return m_value;
    }

    template<typename T>
    void NumericInput<T>::Increment() noexcept
    {
        m_value += m_increment;
        m_valueStr = std::to_string(m_value);
        m_cursorIndex = m_valueStr.length();
        m_text.SetString(m_valueStr);
    }

    template<typename T>
    void NumericInput<T>::Decrement() noexcept
    {
        m_value -= m_increment;
        m_valueStr = std::to_string(m_value);
        m_cursorIndex = m_valueStr.length();
        m_text.SetString(m_valueStr);
    }

    template<typename T>
    void NumericInput<T>::pDraw(RenderTarget& target) noexcept
    {
        const auto cursorX = Math::Clamp<float>(m_pos.x + m_text.GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x);
        const Vector2i pos = m_pos + Vector2i(1,1);
        const Vector2i size = m_size - Vector2i(1,1);

        target.DrawRect(pos, size, m_pColor);
        target.SetBounds(pos, size);
        target.Draw(m_text);

        if(m_state.Focused)
        {
            target.DrawLine(Vector2f{cursorX, m_pos.y + (m_size.y / 10.0f)}, {cursorX, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, Color::Black, 0);
            target.ResetBounds();
            target.DrawGrid(pos, size, 1, 1, m_activeColor, 1);
        }
        else
        {
            target.ResetBounds();
            target.DrawGrid(pos, size, 1, 1, m_sColor, 1);
        }
    }

    template class NumericInput<int64_t>;
    template class NumericInput<uint64_t>;
    template class NumericInput<int32_t>;
    template class NumericInput<uint32_t>;
    template class NumericInput<int16_t>;
    template class NumericInput<uint16_t>;
    template class NumericInput<int8_t>;
    template class NumericInput<uint8_t>;
    template class NumericInput<float>;
    template class NumericInput<double>;
}