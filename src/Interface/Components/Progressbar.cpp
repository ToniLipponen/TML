#include <TML/Interface/Components/Progressbar.h>

namespace tml
{
    namespace Interface
    {
        Progressbar::Progressbar(int32_t x, int32_t y, uint32_t w, uint32_t h)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(w,h);
            m_hSizePolicy = Expand;
            m_text.SetColor(Color::Black);
        }

        float Progressbar::GetValue() const noexcept
        {
            return m_value;
        }

        void Progressbar::SetValue(float value) noexcept
        {
            m_value = Math::Clamp(value, 0.0f, 1.0f);
        }

        void Progressbar::SetTextColor(const Color &color) noexcept
        {
            m_text.SetColor(color);
        }

        void Progressbar::pDraw(Renderer &window)
        {
            m_text.SetString(std::to_string(uint32_t(m_value * 100)) + "%");
            m_text.SetSize(m_size.y / 2);
            m_text.SetPosition({m_pos.x + (m_size.x / 2) - (m_text.GetDimensions().x / 2), static_cast<float>(m_pos.y + m_size.y / 4)});

            window.DrawRect(m_pos, m_size, m_pColor);
            window.DrawRect(m_pos, {m_size.x * m_value, static_cast<float>(m_size.y)}, m_activeColor);
            window.DrawGrid(m_pos, m_size, 1, 1, m_sColor);
            window.Draw(m_text);
        }
    }
}