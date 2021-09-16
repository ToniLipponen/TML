#include <TML/Interface/InterfaceComponent.h>
#include <TML/Input.h>

// Sets a given bit
#define SETBIT(value, bit, state) (value ^= (-state ^ value) & (1UL << bit))
// Checks whether a bit is set or not
#define BITSET(value, bit) ((value & bit) > 0)

using namespace tml::Interface;

BaseComponent::BaseComponent()
{
    m_pColor = 0x777777ff;
    m_sColor = 0x333333ff;
    SETBIT(m_state, 1, 1);
}

BaseComponent::~BaseComponent()
{
    for(auto& i : m_children)
        delete i.second;
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
//    if(component->m_relPos == Vector2(0,0))
//    {
//
//    }
    m_children.insert(std::pair<std::string, BaseComponent*>(_name, component));
}

void BaseComponent::Update(BaseComponent* parent)
{
    if(!BITSET(m_state, Enabled))
        return;
    if(!parent)
    {
        m_mousePos = Mouse::GetPosition();
        m_mouseClicked = Mouse::ButtonClicked(Mouse::Left);

        bool oldMouseOver = BITSET(m_eventStatus, MouseOver);
        SETBIT(m_eventStatus, 5, ContainsPoint(m_mousePos));
        SETBIT(m_eventStatus, 3, (!BITSET(m_eventStatus, MouseEnter) && BITSET(m_eventStatus, MouseOver)));
        SETBIT(m_eventStatus, 4, (oldMouseOver && !BITSET(m_eventStatus, MouseOver)));
        SETBIT(m_eventStatus, 2, (BITSET(m_eventStatus, MouseOver) && m_mouseClicked));
    }
    else
    {
        bool oldMouseOver = BITSET(m_eventStatus, MouseOver);
        SETBIT(m_eventStatus, 5, ContainsPoint(parent->m_mousePos));
        SETBIT(m_eventStatus, 3, (!BITSET(m_eventStatus, MouseEnter) && BITSET(m_eventStatus, MouseOver)));
        SETBIT(m_eventStatus, 4, (oldMouseOver && !BITSET(m_eventStatus, MouseOver)));
        SETBIT(m_eventStatus, 2, (BITSET(m_eventStatus, MouseOver) && parent->m_mouseClicked));
    }
    if(BITSET(m_eventStatus, Click) && m_onClickFunc.IsNotNull())
        m_onClickFunc(this);

    if(BITSET(m_eventStatus, MouseEnter) && m_onMouseEnterFunc.IsNotNull())
        m_onMouseEnterFunc(this);

    if(BITSET(m_eventStatus, MouseExit) && m_onMouseExitFunc.IsNotNull())
        m_onMouseExitFunc(this);

    if(BITSET(m_eventStatus, MouseOver) && m_onHoverFunc.IsNotNull())
        m_onHoverFunc(this);

    if(m_onUpdate.IsNotNull())
        m_onUpdate(this);
    Draw();
    for(auto& i : m_children)
    {
        i.second->Update(this);
        i.second->Draw();
    }
}
//
//Vector2 BaseComponent::GetChildPosition(BaseComponent* child)
//{
//    return Vector2(0,0);
//}