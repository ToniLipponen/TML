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

            for(auto& child : m_children)
                delete child.second;
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
                    m_children.push_front(std::pair<unsigned long, BaseComponent*>(s_hash(std::to_string(m_children.size())), component));
                }
                else
                {
                    component->m_parent = this;
                    m_children.push_front({ s_hash(name), component });
                }
            }
        }

        BaseComponent* BaseComponent::FindComponent(const std::string& name)
        {
            const auto nameHash = s_hash(name);
            return FindComponent(nameHash);
        }

        BaseComponent* BaseComponent::FindComponent(unsigned long nameHash)
        {
            if(m_children.empty())
                return nullptr;

            for(auto& i : m_children)
                if(nameHash == i.first)
                    return i.second;

            for(auto i : m_children)
            {
                auto child_result = i.second->FindComponent(nameHash);
                if (child_result)
                    return child_result;
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
            for(auto& i : m_children)
                i.second->Raise();
        }

        void BaseComponent::ProcessEvents(Event& event)
        {
            switch(event.type)
            {
                default:
                {
                    if(event.type != Event::EventType::Null)
                        OnEvent(event);
                }
                case Event::EventType::MouseButtonPressed:
                    CallUIFunc("MouseDown", event);
                    CallUIFunc("iMouseDown", event);
                    break;

                case Event::EventType::MouseButtonReleased:
                    if(m_state.MouseDown != -1)
                    {
                        CallUIFunc("Click", event);
                        CallUIFunc("iClick", event);
                    }
                    m_state.MouseDown = -1;

                    break;

                case Event::EventType::MouseMoved:
                    m_state.MouseOver = ContainsPoint({event.mouseMove.x, event.mouseMove.y});
                    CallUIFunc("MouseMoved", event);
                    CallUIFunc("iMouseMoved", event);
                    break;

                case Event::EventType::MouseWheelScrolled:
                    CallUIFunc("MouseScroll", event);
                    CallUIFunc("iMouseScroll", event);
                    break;

                case Event::EventType::KeyPressed:
                    CallUIFunc("KeyPressed", event);
                    CallUIFunc("iKeyPressed", event);
                    break;

                case Event::EventType::KeyReleased:
                    CallUIFunc("KeyReleased", event);
                    CallUIFunc("iKeyReleased", event);
                    break;

                case Event::EventType::TextEntered:
                    CallUIFunc("TextEntered", event);
                    CallUIFunc("iTextEntered", event);
                    break;

                case Event::EventType::Null:
                    break;
            }
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
                auto functions = m_listeners.at(name);
                for(auto function : functions)
                {
                    if(function(this, event))
                        return true;
                }
                return true;
            }
            return false;
        }

        // By default, these don't do anything. You can override these in derived classes to add your own functionality.
        void BaseComponent::OnEvent(Event& event){}
        void BaseComponent::OnUpdate(double dt){}
    }
}