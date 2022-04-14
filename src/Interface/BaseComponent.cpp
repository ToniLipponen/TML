#include <TML/Interface/BaseComponent.h>
#include <TML/Graphics/Renderer.h>
#include <TML/System/Clock.h>
#include <string>
#include <algorithm>
#include <iostream>

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
        : m_pColor(Color::White), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff), m_pos(x,y), m_size(w,h), m_originalSize(w,h), m_parent(nullptr)
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
            Event e{};
            CallUIFunc("LostFocus", e);
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
                if(name.empty())
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
                            return false;
                        });
                        return false;
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
                            return false;
                        });
                        return false;
                    });
                }
                Event e{};
                CallUIFunc("ChildAdded", e);
            }
        }

        bool BaseComponent::RemoveChild(const std::string& id)
        {
            auto* ptr = FindComponent(id);
            if(ptr == nullptr)
                return false;

            return RemoveChild(ptr);
        }

        bool BaseComponent::RemoveChild(BaseComponent *component)
        {
            if(component == nullptr)
                return false;

            if(!m_children.empty())
            {
                for(size_t i = 0; i < m_children.size(); i++)
                {
                    if(m_children.at(i) == component)
                    {
                        m_children.erase(m_children.begin() + i);
                        delete component;
                        Event e{};
                        CallUIFunc("ChildRemoved", e);
                        return true;
                    }
                }
            }

            for(auto* i : m_children)
            {
                if(i->RemoveChild(component))
                {
                    Event e{};
                    CallUIFunc("ChildRemoved", e);
                    return true;
                }
            }

            return false;
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
            return Math::PointInRect(p, m_pos, m_size, 0);
        }

        void BaseComponent::Raise()
        {
            RemoveFromProcessStack(this);
            AddToProcessStack(this);
            for(auto* i : m_children)
                i->Raise();
        }

        void BaseComponent::ForEachChild(const std::function<bool(BaseComponent *)> &function)
        {
            if(!m_children.empty())
            {
                for(int64_t i = m_children.size()-1; i >= 0; i--)
                {
                    if(function(m_children.at(i)))
                        break;
                }
            }
        }

        void BaseComponent::SetPosition(const Vector2i &position)
        {
            if(position != m_pos)
            {
                m_pos = position;
                Event e{};
                e.type = Event::InterfaceMoved;
                e.pos.x = position.x;
                e.pos.y = position.y;
                CallUIFunc("Moved", e);
            }
        }

        void BaseComponent::SetSize(const Vector2i &size)
        {
            m_size = size;
            Event e{};
            e.type = Event::InterfaceResized;
            e.size.w = size.x;
            e.size.h = size.y;
            CallUIFunc("Resized", e);
        }

        void BaseComponent::ProcessEvents(Event& event, double dt)
        {
            CallUIFunc("Any", event);
            switch(event.type)
            {
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
                    m_state.MouseOver = ContainsPoint({event.pos.x, event.pos.y});
                    CallUIFunc("MouseMoved", event);
                    if(m_state.MouseOver && !oldMouseOver)
                        CallUIFunc("MouseEnter", event);
                    else if(!m_state.MouseOver && oldMouseOver)
                        CallUIFunc("MouseExit", event);
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

                default:
                    break;
            }
        }

        void BaseComponent::Update(Event& event)
        {
            static Clock clock;
            const double delta = clock.Reset();

            if(event.type == Event::EventType::WindowResized)
                CallUIFunc("WindowResized", event);

            if(!m_state.Enabled)
                return;

            if(!m_processStack.empty())
            {
                /// This needs to be done here & not in item->ProcessEvents(event, delta);
                /// Because this event should not be missed even if the component is disabled.
                if(event.type == Event::EventType::WindowResized)
                {
                    CallUIFunc("WindowResized", event);
                    for(i64 i = m_processStack.size() - 1; i >= 0; --i)
                    {
                        auto* item = m_processStack.at(i);
                        item->CallUIFunc("WindowResized", event);
                    }
                }

                for(i64 i = m_processStack.size() - 1; i >= 0; --i)
                {
                    auto* item = m_processStack.at(i);
                    if(item->Enabled())
                        item->ProcessEvents(event, delta);
                }
            }
            ProcessEvents(event, delta);
        }

        void BaseComponent::Draw(Renderer& renderer)
        {
            pDraw(renderer);
            for(auto* i : m_processStack)
            {
                if(i->Enabled())
                    i->pDraw(renderer);
            }
        }

        void BaseComponent::SetSizePolicy(SizePolicy horizontal, SizePolicy vertical)
        {
            m_hSizePolicy = horizontal;
            m_vSizePolicy = vertical;
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
                if(c->Focused())
                    c->UnFocus();
                c->ForEachChild([](BaseComponent* c2)
                {
                    if(c2->Focused())
                        c2->UnFocus();

                    return false;
                });
                return false;
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
                if(!processStack.empty())
                {
                    for(i64 i = 0; i < processStack.size(); ++i)
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
}