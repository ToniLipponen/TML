#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/Renderer.h>
#include <TML/IO/Input.h>
#include <TML/Utilities/Clock.h>
#include <string>
#include <algorithm>

namespace tml
{
    namespace Interface
    {
        std::hash<std::string> BaseComponent::s_hash = std::hash<std::string>();
        std::vector<BaseComponent*> BaseComponent::s_processStack = std::vector<BaseComponent*>();

        BaseComponent::BaseComponent()
        : m_pColor(WHITE), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff), m_parent(nullptr)
        {
            m_state.Enabled = true;
            s_processStack.push_back(this);
        }

        BaseComponent::BaseComponent(BaseComponent* parent)
        : m_pColor(WHITE), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff), m_parent(parent)
        {
            m_state.Enabled = true;
            m_parent->AddChild(this);
            s_processStack.push_back(this);
        }

        BaseComponent::~BaseComponent()
        {
            for(auto i = 0; i < s_processStack.size() - 1; ++i)
            {
                if(s_processStack.at(i) == this)
                {
                    s_processStack.erase(s_processStack.begin() + i);
                    break;
                }
            }

            for(auto* child : m_children)
                delete child;
        }

        void BaseComponent::Focus()
        {
            m_state.Focused = true;
        }

        void BaseComponent::UnFocus()
        {
            m_state.Focused = false;
        }

        void BaseComponent::Enable()
        {
            m_state.Enabled = true;
        }

        void BaseComponent::Disable()
        {
            m_state.Enabled = false;
        }

        void BaseComponent::ToggleEnabled()
        {
            m_state.Enabled = !m_state.Enabled;
        }

        bool BaseComponent::Enabled() const
        {
            if(!m_state.Enabled)
                return false;
            if(m_parent)
            {
                if(!m_parent->m_state.Enabled)
                    return false;
                else
                    return m_parent->Enabled();
            }
            return m_state.Enabled;
        }

        void BaseComponent::AddListener(const std::string& name, UIFunc callback)
        {
            m_listeners[name].push_back(callback);
        }

        void BaseComponent::AddChild(BaseComponent* component, const std::string& name)
        {
            if(component)
            {
                if(name == "")
                {
                    component->m_parent = this;
                    component->m_id = 0;
                    m_children.push_back(component);
                }
                else
                {
                    component->m_parent = this;
                    component->m_id = s_hash(name);
                    m_children.push_back(component);
                }
            }
        }

        BaseComponent* BaseComponent::FindComponent(const std::string& name)
        {
            const ui64 hash = s_hash(name);
            return FindComponent(hash);
        }

        BaseComponent* BaseComponent::FindComponent(ui64 hash)
        {
            if(m_children.size() == 0)
                return nullptr;

            for(auto* i : m_children)
            {
                if(i->m_id == hash)
                    return i;
            }
            for(auto* i : m_children)
            {
                auto* component = i->FindComponent(hash);
                if(component)
                    return component;
            }
            return nullptr;
        }

        BaseComponent* BaseComponent::GetParent()
        {
            if(m_parent)
                return m_parent;
            return nullptr;
        }

        BaseComponent* BaseComponent::GetRoot()
        {
            if(!m_parent)
                return this;
            return m_parent->GetRoot();
        }

        bool BaseComponent::ContainsPoint(const Vector2i& p)
        {
            return (p > m_pos && p < m_pos + m_size);
        }

        void BaseComponent::Raise()
        {
            for(auto i = 0; i < s_processStack.size(); i++)
            {
                auto item = s_processStack.at(i);
                if(item == this)
                {
                    s_processStack.erase(s_processStack.begin() + i);
                    s_processStack.push_back(item);
                    break;
                }
            }
            for(auto* i : m_children)
                i->Raise();
        }

        void BaseComponent::ProcessEvents(Event& event)
        {
            switch(event.type)
            {
                default:
                {
                    if(event.type != Event::EventType::Null)
                        CallUIFunc("Any", event);
                }
                case Event::EventType::MouseButtonPressed:
                    CallUIFunc("MouseDown", event);
                    break;

                case Event::EventType::MouseButtonReleased:
                    if(m_state.MouseDown != -1)
                    {
                        CallUIFunc("Click", event);
                    }
                    m_state.MouseDown = -1;
                    CallUIFunc("MouseUp", event);
                    break;

                case Event::EventType::MouseMoved:
                    m_state.MouseOver = ContainsPoint({event.mouseMove.x, event.mouseMove.y});
                    CallUIFunc("MouseMoved", event);
                    break;

                case Event::EventType::MouseWheelScrolled:
                    CallUIFunc("MouseScroll", event);
                    break;

                case Event::EventType::KeyPressed:
                    CallUIFunc("KeyPressed", event);
                    break;

                case Event::EventType::KeyReleased:
                    CallUIFunc("KeyReleased", event);
                    break;

                case Event::EventType::TextEntered:
                    CallUIFunc("TextEntered", event);
                    break;

                case Event::EventType::Null:
                    break;
            }
            CallUIFunc("Update", event);
        }

        void BaseComponent::Update(Event event)
        {
            static Clock clock;
            const double delta = clock.Reset();
            Renderer::ResetCamera();

            for(auto i = s_processStack.rbegin(); i != s_processStack.rend(); i++)
            {
                if((*i)->Enabled())
                    (*i)->ProcessEvents(event);
            }
            for(auto i : s_processStack)
            {
                if(i->Enabled())
                {
//                    i->ProcessEvents(event);
                    i->OnUpdate(delta);
                    i->Draw();
                }
            }
        }

        bool BaseComponent::CallUIFunc(const std::string& name, Event &event)
        {
            if(m_listeners.find(name) != m_listeners.end())
            {
                const auto& functions = m_listeners.at(name);
                for(auto& function : functions)
                {
                    function(this, event);
                }
                return true;
            }
            return false;
        }
    }
}