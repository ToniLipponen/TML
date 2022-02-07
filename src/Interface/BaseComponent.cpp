#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/Renderer.h>
#include <TML/IO/Input.h>
#include <TML/Utilities/Clock.h>
#include <string>
#include <algorithm>
#include "TML/Utilities/Utilities.h"

namespace tml
{
    namespace Interface
    {
        std::hash<std::string> BaseComponent::s_hash = std::hash<std::string>();

        BaseComponent::BaseComponent()
        : m_pColor(Color::White), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff), m_parent(nullptr)
        {
            m_state.Enabled = true;
        }

        BaseComponent::BaseComponent(i32 x, i32 y, ui32 w, ui32 h)
        : m_pColor(Color::White), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff), m_parent(nullptr), m_pos(x,y), m_size(w,h), m_originalSize(w,h)
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
            Event e{};
            CallUIFunc("GainedFocus", e);
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
            return Util::PointInRect(p, m_pos, m_size, 0);
        }

        void BaseComponent::Raise()
        {
            RemoveFromProcessStack(this);
            AddToProcessStack(this);
            for(auto* i : m_children)
                i->Raise();
        }

        void BaseComponent::SetPosition(const Vector2i &position)
        {
            if(position != m_pos)
            {
                m_pos = position;
                Event e{};
                e.type = Event::InterfaceMoved;
                e.move.x = position.x;
                e.move.y = position.y;
                CallUIFunc("Moved", e);
            }
        }

        void BaseComponent::SetSize(const Vector2i &size)
        {
            m_size = size;
            Event e{};
            e.type = Event::InterfaceResized;
            e.size.x = size.x;
            e.size.y = size.y;
            CallUIFunc("Resized", e);
        }

        void BaseComponent::ProcessEvents(Event& event, double dt)
        {
            switch(event.type)
            {
                default:
                    CallUIFunc("Any", event);

                case Event::EventType::MouseButtonPressed:
                    CallUIFunc("MouseDown", event);
                    break;

                case Event::EventType::MouseButtonReleased:
                    if(m_state.MouseDown != -1 && m_state.MouseOver)
                        CallUIFunc("Click", event);

                    CallUIFunc("MouseUp", event);
                    m_state.MouseDown = -1;
                    break;

                case Event::EventType::MouseMoved:
                {
                    const bool oldMouseOver = m_state.MouseOver;
                    m_state.MouseOver = ContainsPoint({event.mouseMove.x, event.mouseMove.y});
                    CallUIFunc("MouseMoved", event);
                    if(m_state.MouseOver && !oldMouseOver)
                        CallUIFunc("MouseEnter", event);
                }
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
            updateEvent.type = Event::EventType::InterfaceUpdate;
            updateEvent.update.delta = dt;
            CallUIFunc("InterfaceUpdate", updateEvent);
        }

        void BaseComponent::Update(Event& event, RenderWindow& window)
        {
            static Clock clock;
            const double delta = clock.Reset();
            if(!m_state.Enabled)
                return;
            window.ResetCamera();

            if(!m_processStack.empty())
            {
                for(i64 i = m_processStack.size() - 1; i >= 0; --i)
                {
                    auto* item = m_processStack.at(i);
                    if(item->Enabled())
                        item->ProcessEvents(event, delta);
                }
            }
            ProcessEvents(event, delta);
            Draw(window);
            for(auto* i : m_processStack)
            {
                if(i->Enabled())
                    i->Draw(window);
            }
        }

        bool BaseComponent::CallUIFunc(const std::string& name, Event &event)
        {
            if(m_listeners.find(name) != m_listeners.end())
            {
                const auto& functions = m_listeners.at(name);

                /// Iterating functions in reverse, so that the user added functions are called first.
                for(auto function = functions.rbegin(); function != functions.rend(); function++)
                {
                    (*function)(this, event);
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