#include <TML/Interface/InterfaceComponent.h>
#include <TML/IO/Input.h>
#include <iostream>

using namespace tml::Interface;

BaseComponent::BaseComponent()
: m_mousePos(0), m_pColor(WHITE), m_sColor(0xc7c7c7ff), m_activeColor(0x4d8be4ff)
{
    m_state.Enabled = true;
}

BaseComponent::~BaseComponent()
{
    delete m_child;
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
    if(m_child == nullptr)
    {
        m_child = component;
        m_child->m_name = name;
        m_child->m_parent = this;
    }
    else
    {
        m_child->AddChild(component, name);
    }
}

const BaseComponent* BaseComponent::FindChild(const std::string& name) const
{
    if(m_name == name)
        return this;
    else
        return m_child->FindChild(name);
}

const BaseComponent* BaseComponent::GetHead() const
{
    if(m_parent)
        return m_parent->GetHead();
    return this;
}

bool BaseComponent::ContainsPoint(const Vector2 &p)
{
    return (p.x > m_pos.x && p.x < (m_pos.x + m_size.x)
         && p.y > m_pos.y && p.y < (m_pos.y + m_size.y));
}

// Goes through the linked list drawing everything.
void BaseComponent::Update(float dt)
{
    if(m_state.Enabled)
    {
        Draw();
        if(m_child && m_child->m_state.Enabled)
            m_child->Update(dt);
        else
            _Update(dt);
    }
}

// Goes from the end of the linked list to the head updating everything.
void BaseComponent::_Update(float dt)
{
    const bool oldMouseOver = m_eventStatus.MouseOver;
    const bool mouseDown = Mouse::ButtonDown(Mouse::Left);
    if(!m_child || !m_child->m_state.Enabled)
    {
        // TODO: Fix events checking
        m_mousePos = Mouse::GetPosition();

        m_eventStatus.MouseOver   = ContainsPoint(m_mousePos);
        m_eventStatus.MouseEnter  = (!m_eventStatus.MouseEnter && m_eventStatus.MouseOver);
        m_eventStatus.MouseExit   = (oldMouseOver && !m_eventStatus.MouseOver);
        m_eventStatus.Click       = (!m_eventStatus.MouseDown && m_eventStatus.MouseOver && mouseDown);
        m_eventStatus.MouseDown   = (m_eventStatus.MouseOver && mouseDown);
        m_eventStatus.LostFocus   = (m_state.Focused && (!m_eventStatus.MouseOver && mouseDown));
        m_eventStatus.GainedFocus = (!m_state.Focused && (m_eventStatus.MouseOver && m_eventStatus.Click));
    }
    else
    {
        m_mousePos = m_child->m_mousePos;

        m_eventStatus.MouseOver   = !m_child->m_eventStatus.MouseOver && ContainsPoint(m_child->m_mousePos);
        m_eventStatus.MouseEnter  = (!m_eventStatus.MouseEnter && m_eventStatus.MouseOver);
        m_eventStatus.MouseExit   = (oldMouseOver && !m_eventStatus.MouseOver);
        m_eventStatus.Click       = (!m_child->m_eventStatus.Click && !m_eventStatus.MouseDown && m_eventStatus.MouseOver && mouseDown);
        m_eventStatus.MouseDown   = (m_eventStatus.MouseOver && !m_child->m_eventStatus.MouseDown && mouseDown);
        m_eventStatus.LostFocus   = (m_state.Focused && (!m_eventStatus.MouseOver && mouseDown));
        m_eventStatus.GainedFocus = (!m_state.Focused && (m_eventStatus.MouseOver && m_eventStatus.MouseDown));
    }

    if(m_eventStatus.Click)
    {
        OnMouseClick(m_mousePos);
        if(m_onClickFunc.IsNotNull())
            m_onClickFunc(this);
    }
    else if(m_eventStatus.MouseDown)
    {
        OnMouseDown(m_mousePos);
        if(m_onMouseDownFunc.IsNotNull())
            m_onMouseDownFunc(this);
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
        OnMouseHover();
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

    OnUpdate(dt);
    if(m_onUpdate.IsNotNull())
        m_onUpdate(this);
    if(m_parent)
        m_parent->_Update(dt);
}

// Override these in derived classes to add internal functionality.
void BaseComponent::OnMouseClick(const Vector2& mp){}
void BaseComponent::OnMouseDown(const Vector2& mousePos){}
void BaseComponent::OnMouseHover(){}
void BaseComponent::OnMouseEnter(){}
void BaseComponent::OnMouseExit(){}
void BaseComponent::OnUpdate(float){}
void BaseComponent::OnFocused() {}
void BaseComponent::OnFocusLost() {}