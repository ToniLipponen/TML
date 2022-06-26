#include <TML/Interface/Components/TextInput.h>
#include <TML/System/Clipboard.h>

using namespace tml::Interface;

TextInput::TextInput(uint32_t width, uint32_t height, int32_t x, int32_t y) noexcept
{
    m_pos = Vector2i(x,y);
    m_size = Vector2i(width, height);
    m_minimumSize = Vector2f(height);
    m_text.SetColor(s_defaultTextColor);
    m_text.SetSize(static_cast<float>(height) * 0.8f);
    m_cursorPos = Math::Clamp<float>(m_pos.x + 2, m_pos.x, m_pos.x + m_size.x - 4);

    if(width)
    {
        m_minimumSize.x = static_cast<float>(width);
    }

    m_hSizePolicy = SizePolicy::Expand;
    m_vSizePolicy = SizePolicy::Fixed;

    AlignText();
    InitListeners();
}

void TextInput::SetValue(const String& string) noexcept
{
    m_value = string;
    m_text.SetString(string);
    m_cursorIndex = string.length();
    m_cursorPos = m_text.GetDimensions().x;
}

void TextInput::SetTextColor(const Color &color) noexcept
{
    m_text.SetColor(color);
}

const tml::String& TextInput::GetValue() const noexcept
{
    return m_text.GetString();
}

void TextInput::SetReadOnly(bool readOnly) noexcept
{
    m_readOnly = readOnly;
}

void TextInput::AlignText() noexcept
{
    const auto textSize = m_text.GetDimensions();
    auto pos = GetPosition();
    pos.y += m_size.y / 2 - textSize.y / 2;
    pos.x += Math::Clamp<float>(m_roundness, 0, m_size.y / 2);
    m_text.SetPosition(pos);
}

void TextInput::InitListeners() noexcept
{
    static auto search_forwards = [&]() noexcept -> size_t
    {
        size_t index = m_cursorIndex;

        for(;index < m_value.size()-1; index++)
        {
            if(m_value.at(index) == ' ')
            {
                break;
            }
        }

        return index;
    };

    static auto search_backwards = [&]() noexcept -> size_t
    {
        if(m_cursorIndex == 0)
        {
            return (size_t) 0;
        }

        size_t index = m_cursorIndex - 1;

        for(;index > 0; index--)
        {
            if(m_value.at(index) == ' ')
            {
                break;
            }
        }

        return index;
    };

    AddListener("MouseDown", [&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            m_state.MouseDown = static_cast<char>(e.mouseButton.button);
            e = Event{};
        }
        else
        {
            UnFocus();
        }
    });

    AddListener("Click", [&](BaseComponent* c, Event& e)
    {
        if(m_state.MouseOver)
        {
            Focus();
            Raise();
            e = Event{};
        }
        else
        {
            UnFocus();
        }
    });

    AddListener("KeyPressed", [&](BaseComponent* c, Event& e)
    {
        if(m_state.Focused && !m_readOnly)
        {
            switch(e.key.value)
            {
                case Keyboard::Key::LeftArrow:
                {
                    if(e.key.control)
                    {
                        m_cursorIndex = search_backwards();
                    }
                    else
                    {
                        m_cursorIndex = Math::Clamp<int32_t>(--m_cursorIndex, 0, m_value.size());
                    }
                } break;

                case Keyboard::Key::RightArrow:
                {
                    if(e.key.control)
                    {
                        m_cursorIndex = search_forwards();
                    }
                    else
                    {
                        m_cursorIndex = Math::Clamp<int32_t>(++m_cursorIndex, 0, m_value.size());
                    }
                } break;

                case Keyboard::Key::Backspace:
                {
                    if(m_cursorIndex != 0)
                    {
                        if(e.key.control)
                        {
                            const auto index = Math::Max<int32_t>(search_backwards(), 0);
                            m_value.erase(m_value.begin() + index, m_value.begin() + m_cursorIndex);
                            m_cursorIndex -= (m_cursorIndex - index);
                        }
                        else
                        {
                            m_value.erase(m_value.begin() + m_cursorIndex - 1);
                            m_cursorIndex = Math::Clamp<int32_t>(--m_cursorIndex, 0, m_value.size());
                        }
                    }
                } break;

                case Keyboard::Key::Delete:
                {
                    if(m_cursorIndex != m_value.size() - 1)
                    {
                        if(e.key.control)
                        {
                            const uint32_t index = search_forwards();
                            m_value.erase(m_value.begin() + m_cursorIndex, m_value.begin() + index);
                        }
                        else
                        {
                            m_value.erase(m_value.begin() + m_cursorIndex);
                        }
                    }
                } break;

                case Keyboard::Key::V:
                {
                    if(e.key.control)
                    {
                        String str;

                        if(Clipboard::GetString(str))
                        {
                            m_value.insert(m_cursorIndex, str);
                            m_cursorIndex += str.length();
                        }
                    }
                } break;

                default:
                    break;
            }

            m_blinkTimer = 0;
            m_showLine = true;

            m_text.SetString(m_value.substr(0, m_cursorIndex));
            m_cursorPos = Math::Clamp<float>(m_pos.x + m_text.GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x - 4);
            m_text.SetString(m_value);
            AlignText();
            e = Event{};
        }
    });

    AddListener("TextEntered", [&](BaseComponent* c, Event& e)
    {
        if(m_state.Focused && !m_readOnly)
        {
            m_value.insert(m_value.begin() + m_cursorIndex, e.text.unicode);
            m_cursorIndex++;
            m_text.SetString(m_value.substr(0, m_cursorIndex));
            m_cursorPos = Math::Clamp<float>(m_pos.x + m_text.GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x - 4);
            m_text.SetString(m_value);
            AlignText();
            e = Event{};
        }
    });

    AddListener("Moved", [&](BaseComponent* c, Event& e)
    {
        AlignText();
    });

    AddListener("Resized", [&](BaseComponent* c, Event& e)
    {
        AlignText();
    });

    AddListener("Drawn", [&](BaseComponent* c, Event& e)
    {
        if(m_state.Focused)
        {
            m_blinkTimer += e.update.delta;

            if(m_blinkTimer > 1)
            {
                m_showLine = !m_showLine;
                m_blinkTimer = 0;
            }
        }

        if(m_state.MouseOver)
        {
            m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress + e.update.delta * s_animationSpeed, 0, 1);
        }
        else
        {
            m_borderAnimationProgress = Math::Clamp<double>(m_borderAnimationProgress - e.update.delta * s_animationSpeed, 0, 1);
        }

        if(m_state.Focused)
        {
            m_borderAnimationProgress = 1;
        }

        m_borderColor = Math::Lerp(m_sColor, m_activeColor, m_borderAnimationProgress);
    });
}

void TextInput::pDraw(RenderTarget& target) noexcept
{
    const auto clampedRounded = Math::Clamp<float>(m_roundness, 0, m_size.y / 2);
    target.DrawRect(m_pos, m_size, m_borderColor, clampedRounded);
    target.DrawRect(m_pos + Vector2f(1, 1), m_size - Vector2f(2, 2), m_pColor, clampedRounded);

    target.SetBounds(m_pos + Vector2f(clampedRounded, 0), m_size - Vector2f(clampedRounded * 2, 0));
    target.Draw(m_text);

    if(m_state.Focused && m_showLine && !m_readOnly)
    {
        target.DrawLine({m_cursorPos + clampedRounded, m_pos.y + (m_size.y / 10.0f)}, {m_cursorPos + clampedRounded, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, m_sColor, false);
    }

    target.ResetBounds();
}