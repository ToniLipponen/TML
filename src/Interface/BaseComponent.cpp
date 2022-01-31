#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/Renderer.h>
#include <TML/IO/Input.h>
#include <TML/Utilities/Clock.h>
#include <string>
#include <algorithm>
#include "TML/IO/Logger.h"

namespace tml
{
    namespace Interface
    {
        std::hash<std::string> BaseComponent::s_hash = std::hash<std::string>();

        BaseComponent::BaseComponent()
        : m_pColor(WHITE), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff), m_parent(nullptr)
        {
            m_state.Enabled = true;
        }

        BaseComponent::~BaseComponent()
        {
            RemoveFromProcessStack(this);
            for(auto* child : m_children)
                delete child;
        }

        void BaseComponent::Focus()
        {
            ClearFocused();
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
                    component->m_hash = 0;
                    component->m_id = name;
                    m_children.push_back(component);
                    AddToProcessStack(component);
                    component->ForEachChild([](BaseComponent* c)
                    {
                        c->AddToProcessStack(c);
                        c->ForEachChild([](BaseComponent* c2)
                        {
                            c2->AddToProcessStack(c2);
                        });
                    });
                }
                else
                {
                    component->m_parent = this;
                    component->m_hash = s_hash(name);
                    component->m_id = name;
                    m_children.push_back(component);
                    AddToProcessStack(component);
                    component->ForEachChild([](BaseComponent* c)
                    {
                        c->AddToProcessStack(c);
                        c->ForEachChild([](BaseComponent* c2)
                        {
                            c2->AddToProcessStack(c2);
                        });
                    });
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
            if(m_children.empty())
                return nullptr;

            for(auto* i : m_children)
            {
                if(i->m_hash == hash)
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
            RemoveFromProcessStack(this);
            AddToProcessStack(this);
            for(auto* i : m_children)
                i->Raise();
        }

        void BaseComponent::ProcessEvents(Event& event, double dt)
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
                        CallUIFunc("Click", event);
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
            Event updateEvent;
            updateEvent.type = Event::EventType::Update;
            updateEvent.update.delta = dt;
            CallUIFunc("Update", updateEvent);
        }

        void BaseComponent::Update(Event& event)
        {
            static Clock clock;
            const double delta = clock.Reset();
            if(!m_state.Enabled)
                return;
            Renderer::ResetCamera();

            for(auto i = m_processStack.rbegin(); i != m_processStack.rend(); i++)
            {
                if((*i)->Enabled())
                    (*i)->ProcessEvents(event, delta);
            }
            ProcessEvents(event, delta);
            Draw();
            for(auto* i : m_processStack)
            {
                if(i->Enabled())
                    i->Draw();
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

        void BaseComponent::ClearFocused()
        {
            GetRoot()->ForEachChild([](BaseComponent* c)
            {
                c->UnFocus();
                c->ForEachChild([](BaseComponent* c2)
                {
                    c2->UnFocus();
                });
            });
        }

        void BaseComponent::AddToProcessStack(BaseComponent* component)
        {
            if(component && component->GetRoot())
                component->GetRoot()->m_processStack.push_back(component);
        }

        void BaseComponent::RemoveFromProcessStack(BaseComponent* component)
        {
            if(component && component->GetRoot())
            {
                auto& processStack = component->GetRoot()->m_processStack;
                for(auto i = 0; i < processStack.size() - 1; ++i)
                {
                    if(processStack.at(i) == component)
                    {
                        processStack.erase(processStack.begin() + i);
                        break;
                    }
                }
            }
        }
    }
}