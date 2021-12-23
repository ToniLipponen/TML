#include <TML/Interface/Components/NumericInput.h>
#include <TML/Utilities/Utilities.h>
#include <TML/Graphics/Renderer.h>
#include <TML/IO/Clipboard.h>

extern tml::Text* DEFAULT_TEXT;
namespace tml
{
    namespace Interface
    {
        template<typename T>
        NumericInput<T>::NumericInput(i32 x, i32 y, ui32 w, ui32 h, T value)
        {
            m_pos = Vector2i(x,y);
            m_size = Vector2i(w,h);
            m_value = value;
            m_valueStr = std::to_string(m_value);
            m_cursorIndex = m_valueStr.length();

            Vector2i buttonPos = m_pos;
            buttonPos.x += m_size.x - m_size.y / 2;
            m_incrementButton = new Button(buttonPos.x, buttonPos.y,        h/2, h/2, "+");
            m_decrementButton = new Button(buttonPos.x, buttonPos.y + h/2,  h/2, h/2, "-");
            AddChild(m_decrementButton);
            AddChild(m_incrementButton);

            m_incrementButton->SetTextSize(h * 0.8);
            m_decrementButton->SetTextSize(h * 0.8);
            m_incrementButton->AddListener("Click",[&](BaseComponent* c, Event& e)
            {
                if(c->GetState().MouseOver)
                    ((NumericInput *) c->GetParent())->Increment();
            });
            m_decrementButton->AddListener("Click",[&](BaseComponent* c, Event& e)
            {
                if(c->GetState().MouseOver)
                    ((NumericInput *)c->GetParent())->Decrement();
            });
            AddListener("Click", [&](BaseComponent* c, Event& e)
            {

            });
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
        void NumericInput<T>::OnUpdate(double dt)
        {

        }

        template<typename T>
        void NumericInput<T>::OnMoved()
        {
            auto buttonPos = m_pos;
            buttonPos.x += m_size.x - m_size.y / 2;

            ((Object*)m_incrementButton)->SetSize({m_size.y / 2, (m_size.y / 2)});
            ((Object*)m_decrementButton)->SetSize({m_size.y / 2, (m_size.y / 2)});
            ((Object*)m_incrementButton)->SetPosition(buttonPos);
            ((Object*)m_decrementButton)->SetPosition({buttonPos.x, buttonPos.y + (m_size.y / 2)});
        }

        template<typename T>
        void NumericInput<T>::OnResized()
        {
            auto buttonPos = m_pos;
            buttonPos.x += m_size.x - m_size.y / 2;
            ((Object*)m_incrementButton)->SetSize(Vector2i{m_size.y / 2, (m_size.y / 2)});
            ((Object*)m_decrementButton)->SetSize({m_size.y / 2, (m_size.y / 2)});
            ((Object*)m_incrementButton)->SetPosition(buttonPos);
            ((Object*)m_decrementButton)->SetPosition({buttonPos.x, buttonPos.y + (m_size.y / 2)});
        }


        template<typename T>
        void NumericInput<T>::Draw()
        {
            DEFAULT_TEXT->SetPosition(m_pos);
            DEFAULT_TEXT->SetSize(m_size.y * 0.8f);
            DEFAULT_TEXT->SetString(m_valueStr.substr(0, m_cursorIndex));
            DEFAULT_TEXT->SetString(m_valueStr);

            const Vector2i pos = m_pos + Vector2i(1,1);
            const Vector2i size = m_size - Vector2i(2,2);
            const float cursorX = Math::Clamp<int>(pos.x + DEFAULT_TEXT->GetDimensions().x + 2, m_pos.x, m_pos.x + m_size.x);

            Renderer::DrawRect(m_pos, m_size, m_pColor);
            Renderer::SetBounds(m_pos, m_size);
            Renderer::DrawText(m_valueStr, m_pos + Vector2i(0, m_size.y * 0.2f), m_size.y * 0.8f, BLACK);

            if(m_state.Focused)
            {
                Renderer::DrawLine(Vector2f{cursorX, m_pos.y + (m_size.y / 10.0f)}, {cursorX, m_pos.y + m_size.y - (m_size.y / 10.f)}, 2, BLACK, 0);
                Renderer::ResetBounds();
                Renderer::DrawGrid(pos, size - Vector2i(m_size.y/2, 0), 1, 1, m_activeColor, 1);
            }
            else
            {
                Renderer::ResetBounds();
                Renderer::DrawGrid(pos, size - Vector2i(m_size.y/2, 0), 1, 1, m_sColor, 1);
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