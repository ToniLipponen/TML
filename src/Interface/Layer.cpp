#include <TML/Interface/Layer.h>
#include <TML/System/Clock.h>

namespace tml::Interface
{
    Layer::Layer(Layer&& layer) noexcept
    {
        std::swap(m_processingQueue, layer.m_processingQueue);
        m_enabled = layer.m_enabled;
    }

    void Layer::Attach(BaseComponent* rootNode) noexcept
    {
        std::function<void(BaseComponent*)> registerFunc;

        registerFunc = [&](BaseComponent* node) noexcept -> void
        {
            node->m_root = this;
            m_processingQueue.push_front(node);

            Event e{};
            node->CallUIFunc("Attached", e);

            for(auto& i : node->m_children)
            {
                registerFunc(i.get());
            }
        };

        registerFunc(rootNode);
        m_roots.emplace_back(rootNode);
    }

    void Layer::Detach(BaseComponent* component) noexcept
    {
        std::function<void(BaseComponent*)> unregisterFunc;

        unregisterFunc = [&](BaseComponent* node) noexcept -> void
        {
            const auto end = m_processingQueue.end();
            const auto beg = m_processingQueue.begin();
            const auto it = std::find(beg, end, node);

            if(it != end)
            {
                node->m_root = nullptr;
                m_processingQueue.erase(it);
            }

            for(auto& i : node->m_children)
            {
                unregisterFunc(i.get());
            }
        };

        unregisterFunc(component);

        for(int i = 0; i < m_roots.size(); i++)
        {
            if(m_roots.at(i).get() == component)
            {
                m_roots.erase(m_roots.begin() + i);
                break;
            }
        }
    }

    void Layer::Focus(BaseComponent *component) noexcept
    {
        for(auto* i : m_processingQueue)
        {
            if(i == component)
            {
                if(!i->m_state.Focused)
                {
                    i->m_state.Focused = true;

                    Event e{};
                    i->CallUIFunc("GainedFocus", e);
                }
            }
            else
            {
                i->UnFocus();
            }
        }
    }

    void Layer::ClearFocused() noexcept
    {
        for(auto* i : m_processingQueue)
        {
            i->UnFocus();
        }
    }

    void Layer::ClearDragged() noexcept
    {
        for(auto* i : m_processingQueue)
        {
            i->m_state.Dragged = false;
        }
    }

    void Layer::ClearMouseOver() noexcept
    {
        for(auto* i : m_processingQueue)
        {
            i->m_state.MouseOver = false;
        }
    }

    void Layer::Update(Event &event) noexcept
    {
        static Clock clock;
        const double delta = clock.Reset();

        if(!m_processingQueue.empty())
        {
            if(event.type == Event::WindowResized)
            {
                for(auto* i : m_processingQueue)
                {
                    i->CallUIFunc("WindowResized", event);
                }
            }

            HandleEvent(event);
        }
    }

    void Layer::SetEnabled(bool enabled) noexcept
    {
        m_enabled = enabled;
    }

    bool Layer::IsEnabled() const noexcept
    {
        return m_enabled;
    }

    void Layer::OnDraw(RenderTarget *renderer, Texture *) noexcept
    {
        if(m_enabled)
        {
            static Clock clock;

            Event event{};
            event.update.delta = clock.Reset();

            auto beg = m_processingQueue.rbegin();
            auto end = m_processingQueue.rend();

            for(auto i = beg; i != end; i++)
            {
                auto* item = *i;

                if(item->Enabled())
                {
                    item->pDraw(*renderer);
                    item->CallUIFunc("Drawn", event);
                }
            }
        }
    }

    void Layer::HandleEvent(Event &event) noexcept
    {
        if(m_enabled)
        {
            for(auto* i : m_processingQueue)
            {
                if(i->Enabled() && i->HandleEvent(event))
                {
                    event = {};
                    break;
                }
            }

            if(event.type == Event::MouseButtonReleased)
            {
                ClearFocused();
            }
        }
    }
}