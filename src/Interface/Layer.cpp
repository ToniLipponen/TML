#include <TML/Interface/Layer.h>
#include <TML/System/Clock.h>

namespace tml::Interface
{
    Layer::Layer(Layer&& layer) noexcept
    {
        std::swap(m_processingQueue, layer.m_processingQueue);
        std::swap(m_rootNode, layer.m_rootNode);
        m_enabled = layer.m_enabled;
    }

    void Layer::Attach(BaseComponent* rootNode) noexcept
    {
        std::function<void(BaseComponent*)> registerFunc;

        registerFunc = [&](BaseComponent* node) noexcept -> void
        {
            node->m_root = this;
            m_processingQueue.push_front(node);

            for(auto& i : node->m_children)
            {
                registerFunc(i.get());
            }
        };

        registerFunc(rootNode);
        m_rootNode = std::unique_ptr<BaseComponent>(rootNode);
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
    }

    void Layer::Raise(BaseComponent *component) noexcept
    {
        std::function<void(BaseComponent*)> raiseFunc;

        raiseFunc = [&](BaseComponent* node) noexcept -> void
        {
            const auto end = m_processingQueue.end();
            const auto beg = m_processingQueue.begin();
            const auto it = std::find(beg, end, node);
            auto value = *it;

            if(it != end)
            {
                m_processingQueue.erase(it);
                m_processingQueue.push_front(value);
            }

            for(auto& i : node->m_children)
            {
                raiseFunc(i.get());
            }
        };

        raiseFunc(component);
    }

    void Layer::ClearFocused() noexcept
    {
        for(auto* i : m_processingQueue)
        {
            i->m_state.Focused = false;
        }
    }

    void Layer::Update(Event &event) const noexcept
    {
        static Clock clock;
        const double delta = clock.Reset();

        if(!m_processingQueue.empty())
        {
            if(event.type == EventType::WindowResized)
            {
                for(auto* i : m_processingQueue)
                {
                    i->CallUIFunc("WindowResized", event);
                }
            }

            if(m_enabled)
            {
                for(auto* i : m_processingQueue)
                {
                    if(i->Enabled())
                    {
                        i->ProcessEvents(event, delta);
                    }
                }
            }
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
}