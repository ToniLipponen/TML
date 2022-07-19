#include <TML/Interface/Component.h>
#include <TML/Interface/Interface.h>
#include <TML/Graphics/RenderTarget.h>
#include <TML/System/Clock.h>
#include <string>
#include <algorithm>

namespace tml::Interface
{
    [[maybe_unused]] float Component::s_scale = 1.0f;
    [[maybe_unused]] float Component::s_animationSpeed = 5.f;
    [[maybe_unused]] Color Component::s_defaultPrimaryColor = Color(0x171717ff);
    [[maybe_unused]] Color Component::s_defaultSecondaryColor = Color(0x444444ff);
    [[maybe_unused]] Color Component::s_defaultActiveColor = Color(0xda0037ff);
    [[maybe_unused]] Color Component::s_defaultTextColor = Color(0xedededff);

    Component::Component() noexcept
    : m_pColor(s_defaultPrimaryColor),
      m_sColor(s_defaultSecondaryColor),
      m_activeColor(s_defaultActiveColor),
      m_textColor(s_defaultTextColor),
      m_minimumSize(0),
      m_maximumSize(10000),
      m_parent(nullptr),
      m_root(nullptr)
    {
        m_state.Enabled = true;
    }

    Component::Component(int32_t x, int32_t y, uint32_t w, uint32_t h) noexcept
    : m_pColor(s_defaultPrimaryColor),
      m_sColor(s_defaultSecondaryColor),
      m_activeColor(s_defaultActiveColor),
      m_textColor(s_defaultTextColor),
      m_minimumSize(0),
      m_maximumSize(10000),
      m_parent(nullptr),
      m_root(nullptr)
    {
        m_pos = Vector2f(x,y);
        m_size = Vector2f(w,h);
        m_state.Enabled = true;
    }

    Component::~Component() noexcept
    {

    }

    void Component::Focus() noexcept
    {
        if(m_root)
        {
            m_root->Focus(this);
        }
    }

    void Component::UnFocus() noexcept
    {
        if(m_state.Focused)
        {
            m_state.Focused = false;
            Event e{};
            CallUIFunc("LostFocus", e);
        }
    }

    void Component::Enable() noexcept
    {
        if(!m_state.Enabled)
        {
            m_state.Enabled = true;
            Event event{};
            CallUIFunc("Enabled", event);
        }
    }

    void Component::Disable() noexcept
    {
        if(m_state.Enabled)
        {
            m_state.Enabled = false;
            Event event{};
            CallUIFunc("Disabled", event);
        }
    }

    void Component::ToggleEnabled() noexcept
    {
        if(m_state.Enabled)
        {
            Disable();
        }
        else
        {
            Enable();
        }
    }

    Component::StateFlag Component::GetState() const noexcept
    {
        return m_state;
    }

    bool Component::Focused() const noexcept
    {
        return m_state.Focused;
    }

    bool Component::Enabled() const noexcept
    {
        if(!m_state.Enabled)
        {
            return false;
        }

        if(m_parent)
        {
            if(!m_parent->m_state.Enabled)
            {
                return false;
            }
            else
            {
                return m_parent->Enabled();
            }
        }

        return m_state.Enabled;
    }

    void Component::AddListener(const std::string& name, const EventCallback& callback) noexcept
    {
        m_listeners[name].push_back(callback);
    }

    void Component::AddChild(Component* component, const std::string& name) noexcept
    {
        if(component)
        {
            if(name.empty())
            {
                component->m_parent = this;
                component->m_hash = 0;
                component->m_id = name;
                m_children.emplace_back(component);
            }
            else
            {
                component->m_parent = this;
                component->m_hash = std::hash<std::string>{}(name);
                component->m_id = name;
                m_children.emplace_back(component);
            }

            Event e{};
            CallUIFunc("ChildAdded", e);
        }
    }

    bool Component::RemoveChild(const std::string& id) noexcept
    {
        auto* ptr = GetChild(id);

        if(ptr == nullptr)
        {
            return false;
        }

        return RemoveChild(ptr);
    }

    bool Component::RemoveChild(Component *component) noexcept
    {
        if(component == nullptr)
        {
            return false;
        }

        if(!m_children.empty())
        {
            for(size_t i = 0; i < m_children.size(); i++)
            {
                if(m_children.at(i).get() == component)
                {
                    m_children.erase(m_children.begin() + i);
                    Event e{};
                    CallUIFunc("ChildRemoved", e);
                    return true;
                }
            }
        }

        for(auto& i : m_children)
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

    bool Component::ContainsPoint(const Vector2i& p)
    {
        return Math::PointInRect(p, m_pos, m_size, 0);
    }

    void Component::ForEachChild(const std::function<bool(Component *)> &function) noexcept
    {
        if(!m_children.empty())
        {
            for(auto i = m_children.rbegin(); i != m_children.rend(); i++)
            {
                if(function(i->get()))
                {
                    return;
                }
            }
        }
    }

    /// Getters
    Vector2f Component::GetSize() const noexcept
    {
        return m_size;
    }

    Vector2f Component::GetPosition() const noexcept
    {
        return m_pos;
    }

    Vector2f Component::GetMaximumSize() const noexcept
    {
        return m_maximumSize;
    }

    Vector2f Component::GetMinimumSize() const noexcept
    {
        return m_minimumSize;
    }

    SizePolicy Component::GetHorizontalSizePolicy() const noexcept
    {
        return m_hSizePolicy;
    }

    SizePolicy Component::GetVerticalSizePolicy() const noexcept
    {
        return m_vSizePolicy;
    }

    Component* Component::GetParent() noexcept
    {
        return m_parent;
    }

    Component* Component::GetChild(const std::string& name) noexcept
    {
        const uint64_t hash = std::hash<std::string>{}(name);
        return GetChild(hash);
    }

    Component* Component::GetChild(uint64_t hash) noexcept
    {
        if(m_children.empty())
        {
            return nullptr;
        }

        for(auto& i : m_children)
        {
            if(i->m_hash == hash)
            {
                return i.get();
            }
        }

        for(auto& i : m_children)
        {
            auto* component = i->GetChild(hash);

            if(component)
            {
                return component;
            }
        }

        return nullptr;
    }

    Interface* Component::GetRoot() noexcept
    {
        return m_root;
    }

    uint64_t Component::GetHash() const noexcept
    {
        return m_hash;
    }

    const std::string& Component::GetID() const noexcept
    {
        return m_id;
    }

    /// Setters
    void Component::SetPosition(const Vector2i &position) noexcept
    {
        m_pos = position;
        Event e{};
        CallUIFunc("Moved", e);
    }

    void Component::SetPosition(int32_t x, int32_t y) noexcept
    {
        m_pos = Vector2f(static_cast<float>(x), static_cast<float>(y));
        Event e{};
        CallUIFunc("Moved", e);
    }

    void Component::SetSize(const Vector2i &size) noexcept
    {
        const auto oldSize = m_size;

        m_size.x = Math::Max<float>(size.x, m_minimumSize.x);
        m_size.y = Math::Max<float>(size.y, m_minimumSize.y);

        if(oldSize == m_size)
        {
            return;
        }

        Event e{};
        CallUIFunc("Resized", e);
    }

    void Component::SetSize(uint32_t w, uint32_t h) noexcept
    {
        SetSize({w, h});
    }

    void Component::SetSizePolicy(SizePolicy horizontal, SizePolicy vertical) noexcept
    {
        m_hSizePolicy = horizontal;
        m_vSizePolicy = vertical;
    }

    void Component::SetPrimaryColor(const Color& color) noexcept
    {
        m_pColor = color;
    }

    void Component::SetSecondaryColor(const Color& color) noexcept
    {
        m_sColor = color;
    }

    void Component::SetActiveColor(const Color& color) noexcept
    {
        m_activeColor = color;
    }

    void Component::SetTextColor(const Color& color) noexcept
    {
        m_textColor = color;
    }

    void Component::SetRoundness(float radius) noexcept
    {
        m_roundness = radius;
    }

    bool Component::CallUIFunc(const std::string& name, const Event& event) noexcept
    {
        if(m_listeners.find(name) != m_listeners.end())
        {
            bool handled = false;
            const auto& functions = m_listeners.at(name);

            /// Iterating functions in reverse, so that the user added functions are called first.
            for(auto function = functions.rbegin(); function != functions.rend(); function++)
            {
                if((*function)(this, event))
                {
                    handled = true;
                }
            }

            return handled;
        }

        return false;
    }

    bool Component::HandleEvent(const Event& event) noexcept
    {
        CallUIFunc("Any", event);

        switch(event.type)
        {
            case Event::MouseButtonPressed:
            {
                if(m_state.MouseOver)
                {
                    CallUIFunc("MouseDown", event);
                    return true;
                }
            } break;

            case Event::Clicked:
            {
                if(m_state.MouseOver)
                {
                    Focus();
                    return CallUIFunc("Click", event);
                }
            } break;

            case Event::MouseButtonReleased:
            {
                bool handled = false;

                if(CallUIFunc("MouseUp", event))
                {
                    handled = true;
                }

                m_state.Dragged = false;

                return handled;
            } break;

            case Event::MouseMoved:
            {
                bool handled = false;
                const bool oldMouseOver = m_state.MouseOver;
                m_state.MouseOver = ContainsPoint({event.pos.x, event.pos.y});

                if(!oldMouseOver && m_state.MouseOver)
                {
                    if(CallUIFunc("MouseEnter", event))
                    {
                        handled = true;
                    }
                }
                else if(oldMouseOver && !m_state.MouseOver)
                {
                    if(CallUIFunc("MouseExit", event))
                    {
                        handled = true;
                    }
                }

                if(m_state.MouseOver)
                {
                    if(CallUIFunc("MouseMoved", event))
                    {
                        handled = true;
                    }
                }

                return handled;
            } break;

            case Event::MouseWheelScrolled:
            {
                return CallUIFunc("MouseScroll", event);
            } break;

            case Event::MouseDragged:
            {
                if(ContainsPoint({event.drag.beginX, event.drag.beginY}))
                {
                    m_state.Dragged = true;
                }

                return (m_state.Dragged && CallUIFunc("Dragged", event));
            } break;

            case Event::KeyPressed:
            {
                return (m_state.Focused && CallUIFunc("KeyPressed", event));
            } break;

            case Event::KeyReleased:
            {
                return (m_state.Focused && CallUIFunc("KeyReleased", event));
            } break;

            case Event::TextEntered:
            {
                return CallUIFunc("TextEntered", event);
            } break;

            default:
                break;
        }
        return false;
    }

    void Component::SetGlobalAnimationSpeed(float speed) noexcept
    {
        s_animationSpeed = speed;
    }

    void Component::SetGlobalDefaultPrimaryColor(const Color& color) noexcept
    {
        s_defaultPrimaryColor = color;
    }

    void Component::SetGlobalDefaultSecondaryColor(const Color& color) noexcept
    {
        s_defaultSecondaryColor = color;
    }

    void Component::SetGlobalDefaultActiveColor(const Color& color) noexcept
    {
        s_defaultActiveColor = color;
    }

    void Component::SetGlobalDefaultTextColor(const Color& color) noexcept
    {
        s_defaultTextColor = color;
    }
}