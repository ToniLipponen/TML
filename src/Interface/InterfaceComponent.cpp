#include <TML/Interface/InterfaceComponent.h>
#include <TML/Input.h>

// Sets a given bit
#define SETBIT(value, bit, state) (value ^= (-state ^ value) & (1UL << bit))
// Checks whether a bit is set or not
#define BITSET(value, bit) ((value & bit) > 0)

using namespace tml::Interface;
BaseComponent* BaseComponent::ActiveComponent = nullptr;
BaseComponent::BaseComponent()
{
    m_pColor = 0x777777ff;
    m_sColor = 0x333333ff;
    SETBIT(m_state, 1, 1);
}

BaseComponent::~BaseComponent()
{
//    for(auto& i : m_children)
//        delete i.second;
}

void BaseComponent::Focus()
{
    SETBIT(m_state, 2, 1);
}

void BaseComponent::UnFocus()
{
    SETBIT(m_state, 2, 0);
}

void BaseComponent::Enable()
{
    SETBIT(m_state, 1, 1);
}

void BaseComponent::Disable()
{
    SETBIT(m_state, 1, 0);
}

void BaseComponent::AddChild(BaseComponent *component, const std::string &name)
{
    std::string _name = name;
    if(component == nullptr)
        return;
    if(name.empty())
        _name = std::to_string(random());
    if(m_child.second == nullptr)
    {
        m_child = std::pair<std::string, BaseComponent*>(_name, component);
        m_child.second->m_parent = std::pair<std::string, BaseComponent*>(_name, this);
    }
    else
    {
        m_child.second->AddChild(component, _name);
    }
}

// Todo
const BaseComponent* BaseComponent::FindChild(const std::string& name) const
{
//    if(m_children.find(name) != m_children.end())
//        return m_children.at(name);
    return nullptr;
}
bool BaseComponent::ContainsPoint(const Vector2 &p)
{
    return (p.x > m_absPos.x && p.x < (m_absPos.x + m_absSize.x)
            && p.y > m_absPos.y && p.y < (m_absPos.y + m_absSize.y));
}
void BaseComponent::Update(float dt)
{
    // Find the end of the linked list
    Draw();
    if(m_child.second)
        m_child.second->Update(dt);
    else
        _Update(dt);
}

void BaseComponent::_Update(float dt)
{
    if(!BITSET(m_state, Enabled))
        return;
    if(!m_child.second)
    {
        // TODO: Fix events checking
        m_mousePos = Mouse::GetPosition();
        m_mouseClicked = Mouse::ButtonClicked(Mouse::Left);

        bool oldMouseOver = BITSET(m_eventStatus, MouseOver);
        SETBIT(m_eventStatus, 4, ContainsPoint(m_mousePos));
        SETBIT(m_eventStatus, 2, (!BITSET(m_eventStatus, MouseEnter) && BITSET(m_eventStatus, MouseOver)));
        SETBIT(m_eventStatus, 3, (oldMouseOver && !BITSET(m_eventStatus, MouseOver)));
        SETBIT(m_eventStatus, 1, (BITSET(m_eventStatus, MouseOver) && m_mouseClicked));
    }
    else
    {
        m_mousePos = m_child.second->m_mousePos;
        m_mouseClicked = m_child.second->m_mouseClicked;
        bool oldMouseOver = BITSET(m_eventStatus, MouseOver);
        SETBIT(m_eventStatus, 4, ContainsPoint(m_child.second->m_mousePos));
        SETBIT(m_eventStatus, 2, (!BITSET(m_eventStatus, MouseEnter) && BITSET(m_eventStatus, MouseOver)));
        SETBIT(m_eventStatus, 3, (oldMouseOver && !BITSET(m_eventStatus, MouseOver)));
        SETBIT(m_eventStatus, 1, (BITSET(m_eventStatus, MouseOver) && m_child.second->m_mouseClicked));
    }

    if(BITSET(m_eventStatus, Click))
    {
        ActiveComponent = this;
        OnMouseClick(m_mousePos);
        if(m_onClickFunc.IsNotNull())
            m_onClickFunc(this);
    }

    if(BITSET(m_eventStatus, MouseEnter))
    {
        OnMouseEnter();
        if(m_onMouseEnterFunc.IsNotNull())
            m_onMouseEnterFunc(this);
    }

    if(BITSET(m_eventStatus, MouseExit))
    {
        OnMouseExit();
        if(m_onMouseExitFunc.IsNotNull())
            m_onMouseExitFunc(this);
    }

    if(BITSET(m_eventStatus, MouseOver))
    {
        OnMouseHover();
        if(m_onHoverFunc.IsNotNull())
            m_onHoverFunc(this);
    }

    OnUpdate(dt);
    if(m_onUpdate.IsNotNull())
        m_onUpdate(this);
    if(m_parent.second)
        m_parent.second->_Update(dt);
}

// Override these in derived classes to add internal functionality.
void BaseComponent::OnMouseClick(const Vector2& mp){}
void BaseComponent::OnMouseHover(){}
void BaseComponent::OnMouseEnter(){}
void BaseComponent::OnMouseExit(){}
void BaseComponent::OnUpdate(float){}