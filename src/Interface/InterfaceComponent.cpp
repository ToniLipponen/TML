#include <TML/Interface/InterfaceComponent.h>
#include <TML/IO/Input.h>
#include <iostream>
#include <map>
#include <stack>
#include <functional>


using namespace tml::Interface;
BaseComponent* BaseComponent::s_activeComponent = nullptr;
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

const BaseComponent* BaseComponent::FindComponent(const std::string& name) const
{
    const auto nameHash = s_hash(name);
    return FindComponent(nameHash);
}

const BaseComponent* BaseComponent::FindComponent(unsigned long nameHash) const
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

const BaseComponent* BaseComponent::GetParent() const
{
    if(m_parent)
        return m_parent;
    return nullptr;
}

bool BaseComponent::ContainsPoint(const Vector2 &p)
{
    return (p > m_pos && p < m_pos + m_size);
}

void BaseComponent::ProcessEvents(const Vector2& mp, bool& mouseDown, Events &events, const Events &childEvents)
{
    if(!childEvents.Drag)
    {
        const auto oldMouseOver   = events.MouseOver;
        events.MouseOver   = (!childEvents.MouseOver && ContainsPoint(mp));
        events.MouseEnter  = (!events.MouseEnter && events.MouseOver);
        events.MouseExit   = (oldMouseOver && !events.MouseOver);
        events.Click       = (!childEvents.Click && !events.MouseDown && events.MouseOver && mouseDown);
        events.MouseDown   = (events.MouseOver && !childEvents.MouseDown && mouseDown);

        if(events.MouseDown)
            events.Drag = true;
        else if(!mouseDown)
            events.Drag = false;
        events.LostFocus   = (m_state.Focused && (!events.MouseOver && mouseDown));
        events.GainedFocus = (!m_state.Focused && (events.MouseOver && events.MouseDown));
    }
}

void BaseComponent::Update(float dt)
{
    static bool s_clicked;
    static Vector2 s_mousePos;
    if(!m_parent)
    {
        if(m_state.Enabled)
        {
            s_clicked = Mouse::ButtonDown(Mouse::Left);
            s_mousePos = Mouse::GetPosition();
            Draw();
        }
    }
    if(!m_children.empty())
    {
        for (auto child = m_children.rbegin(); child != m_children.rend(); ++child)
            if(child->second->m_state.Enabled)
                child->second->Draw();
        bool enabled = false;
        for(auto child : m_children)
        {
            if(child.second->m_state.Enabled)
            {
                child.second->Update(dt);
                enabled = true;
            }
        }
        if(!enabled) // No children enabled. Start going up the tree.
            _Update(dt, s_mousePos, s_clicked, m_eventStatus);
    }
    else
    {
        _Update(dt, s_mousePos, s_clicked, m_eventStatus);
    }
}

void BaseComponent::_Update(float dt, const Vector2& mp, bool& clicked, const Events& childEvents)
{
    if(m_children.empty())
        ProcessEvents(mp, clicked, m_eventStatus, {0});
    else
        ProcessEvents(mp, clicked, m_eventStatus, childEvents);

    if(m_eventStatus.Click)
    {
        OnMouseClick(mp);
        if(m_onClickFunc.IsNotNull())
            m_onClickFunc(this);
        clicked = false;
        s_activeComponent = this;
    }
    else if(m_eventStatus.MouseDown)
    {
        OnMouseDown(mp);
        if(m_onMouseDownFunc.IsNotNull())
            m_onMouseDownFunc(this);
        clicked = false;
    }
    if(m_eventStatus.MouseEnter)
    {
        OnMouseEnter();
        if(m_onMouseEnterFunc.IsNotNull())
            m_onMouseEnterFunc(this);
    }
    if(m_eventStatus.MouseExit)
    {
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
        m_state.Focused = true;
        OnFocused();
        if(m_onFocused.IsNotNull())
            m_onFocused(this);
    }
    if(m_eventStatus.LostFocus)
    {
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
        clicked = false;
    }

    OnUpdate(dt);
    if(m_onUpdate.IsNotNull())
        m_onUpdate(this);
    if(m_parent)
        m_parent->_Update(dt, mp, clicked, m_eventStatus);
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