#include <TML/Interface/InterfaceComponent.h>
#include <TML/IO/Input.h>
#include <iostream>
#include <map>
#include <stack>
#include <functional>

namespace tml::Interface
{
    std::hash<std::string> BaseComponent::s_hash = std::hash<std::string>();

    BaseComponent::BaseComponent()
    : m_pColor(WHITE), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff), m_parent(nullptr)
    {
        m_state.Enabled = true;
    }

    BaseComponent::BaseComponent(BaseComponent* parent)
    : m_pColor(WHITE), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff), m_parent(parent)
    {
        m_state.Enabled = true;
        m_parent->AddChild(this);
    }

    BaseComponent::~BaseComponent()
    {
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

    void BaseComponent::AddChild(BaseComponent *component, const std::string &name)
    {
        if(component)
        {
            if(name == "")
            {
                component->m_parent = this;
                m_children.push_front({s_hash(std::to_string(m_children.size())), component});
            }
            else
            {
                component->m_parent = this;
                m_children.push_front({s_hash(name), component});
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
        const auto result = std::find_if(m_children.begin(), m_children.end(), [nameHash](std::pair<unsigned long, BaseComponent*> p){return p.first == nameHash;});
        if(result == m_children.end())
            for(auto i : m_children)
            {
                auto child_result = i.second->FindComponent(nameHash);
                if(child_result)
                    return child_result;
            }
        else
            return result->second;

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

    bool BaseComponent::ContainsPoint(const Vector2 &p)
    {
        return (p > m_pos && p < m_pos + m_size);
    }

    void BaseComponent::ProcessEvents(const Vector2& mp, bool mouseDown, Events &childEvents)
    {
        if(!m_eventStatus.Drag || !childEvents.Drag)
        {
            const auto oldMouseOver   = m_eventStatus.MouseOver;
            m_eventStatus.MouseOver   = (!childEvents.MouseOver && ContainsPoint(mp) && !childEvents.MouseOver);
            m_eventStatus.MouseEnter  = (!oldMouseOver && !childEvents.MouseEnter && m_eventStatus.MouseOver);
            m_eventStatus.MouseExit   = (oldMouseOver && !childEvents.MouseExit && !m_eventStatus.MouseOver);
            m_eventStatus.Click       = (!m_eventStatus.MouseDown && !m_eventStatus.Click && m_eventStatus.MouseOver && !childEvents.Click && mouseDown);
            m_eventStatus.MouseDown   = (m_eventStatus.MouseOver && !childEvents.MouseDown && mouseDown);
            m_eventStatus.GainedFocus = (!m_state.Focused && (m_eventStatus.MouseOver && !childEvents.GainedFocus && m_eventStatus.MouseDown));

            if(m_eventStatus.MouseDown)
                m_eventStatus.Drag = true;
            else if(!mouseDown)
                m_eventStatus.Drag = false;

            childEvents.MouseOver   = m_eventStatus.MouseOver   || childEvents.MouseOver;
            childEvents.MouseEnter  = m_eventStatus.MouseEnter  || childEvents.MouseEnter;
            childEvents.MouseExit   = m_eventStatus.MouseExit   || childEvents.MouseExit;
            childEvents.Click       = m_eventStatus.Click       || childEvents.Click;
            childEvents.MouseDown   = m_eventStatus.MouseDown   || childEvents.MouseDown;
            childEvents.LostFocus   = m_eventStatus.LostFocus   || childEvents.LostFocus;
            childEvents.GainedFocus = m_eventStatus.GainedFocus || childEvents.GainedFocus;
            childEvents.Drag        = m_eventStatus.MouseDown   || childEvents.Drag;
        }
        m_eventStatus.LostFocus   = (m_state.Focused && !m_eventStatus.MouseOver && mouseDown);
    }


    void BaseComponent::Poll(const Vector2& mp, bool mouseDown, Events& events)
    {
        for(auto child : m_children)
        {
            if(child.second->m_state.Enabled)
            {
                child.second->Poll(mp, mouseDown, events);
            }
        }
        ProcessEvents(mp, mouseDown, events);
    }

    void BaseComponent::Update(float dt)
    {
        const Vector2 mousePos = Mouse::GetPosition();
        const bool click = Mouse::ButtonDown(Mouse::Left);
        Events event = {false};
        Poll(mousePos, click, event);
        GetRoot()->p_Update(dt, mousePos);
    }

    void BaseComponent::p_Update(float dt, const Vector2& mp)
    {
        if(!m_state.Enabled)
            return;
        Draw();
        if(m_eventStatus.Click)
        {
            m_eventStatus.Click = false;
            OnMouseClick(mp);
            if(m_onClickFunc.IsNotNull())
                m_onClickFunc(this);
        }
        else if(m_eventStatus.MouseDown)
        {
            OnMouseDown(mp);
            if(m_onMouseDownFunc.IsNotNull())
                m_onMouseDownFunc(this);
        }
        if(m_eventStatus.MouseEnter)
        {
            m_eventStatus.MouseEnter = false;
            OnMouseEnter();
            if(m_onMouseEnterFunc.IsNotNull())
                m_onMouseEnterFunc(this);
        }
        else if(m_eventStatus.MouseExit)
        {
            m_eventStatus.MouseExit = false;
            OnMouseExit();
            if(m_onMouseExitFunc.IsNotNull())
                m_onMouseExitFunc(this);
        }
        if(m_eventStatus.MouseOver)
        {
            OnMouseHover(mp);
            if(m_onHoverFunc.IsNotNull())
                m_onHoverFunc(this);
        }
        if(m_eventStatus.GainedFocus)
        {
            m_eventStatus.GainedFocus = false;
            m_state.Focused = true;
            OnFocused();
            if(m_onFocused.IsNotNull())
                m_onFocused(this);
        }
        if(m_eventStatus.LostFocus)
        {
            m_eventStatus.LostFocus = false;
            m_state.Focused = false;
            OnFocusLost();
            if(m_onFocusLost.IsNotNull())
                m_onFocusLost(this);
        }
        if(m_eventStatus.Drag)
        {
            OnMouseDrag(mp);
            if(m_onDragFunc.IsNotNull())
                m_onDragFunc(this);
        }

        OnUpdate(dt);
        if(m_onUpdate.IsNotNull())
            m_onUpdate(this);
        for(auto& i : m_children)
            i.second->p_Update(dt, mp);
    }

    // Override these in derived classes to add internal functionality.
    void BaseComponent::OnMouseClick(const Vector2& mp){}
    void BaseComponent::OnMouseDown(const Vector2& mousePos){}
    void BaseComponent::OnMouseHover(const Vector2& mousePos){}
    void BaseComponent::OnMouseEnter(){}
    void BaseComponent::OnMouseExit(){}
    void BaseComponent::OnMouseDrag(const Vector2& mousePos){}
    void BaseComponent::OnUpdate(float){}
    void BaseComponent::OnFocused() {}
    void BaseComponent::OnFocusLost() {}
}