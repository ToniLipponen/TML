#include <TML/Interface/UIRoot.h>
#include <TML/System/Clock.h>

namespace tml::Interface
{
    void UIRoot::Attach(BaseComponent* rootNode) noexcept
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

    void UIRoot::Detach(BaseComponent* component) noexcept
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

    void UIRoot::Raise(BaseComponent *component) noexcept
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

    void UIRoot::ClearFocused() noexcept
    {
        for(auto* i : m_processingQueue)
        {
            i->m_state.Focused = false;
        }
    }

    void UIRoot::Update(Event &event) const noexcept
    {
        static Clock clock;
        const double delta = clock.Reset();

        if(!m_processingQueue.empty())
        {
            /// This needs to be done here & not in item->ProcessEvents(event, delta);
            /// Because this event should not be missed even if the component is disabled.
            if(event.type == EventType::WindowResized)
            {
                for(auto* i : m_processingQueue)
                {
                    i->CallUIFunc("WindowResized", event);
                }
            }

            for(auto* i : m_processingQueue)
            {
                if(i->Enabled())
                {
                    i->ProcessEvents(event, delta);
                }
            }
        }
    }

    void UIRoot::OnDraw(RenderTarget *renderer, Texture *) noexcept
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