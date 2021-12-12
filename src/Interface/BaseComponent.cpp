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
            for(auto i = 0; i < s_processStack.size() - 1; i++)
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
            for(auto i = 0; i < s_processStack.size() - 1; ++i)
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

        void BaseComponent::ProcessEvents(const Vector2i& mp, bool& mouseDown, Events& evnt)
        {
            evnt.Drag = (m_event.MouseDown && mouseDown) || evnt.Drag;
            if(!evnt.Drag)
            {
                const auto oldMouseOver = m_event.MouseOver;
                m_event.MouseOver     = (!evnt.MouseOver && ContainsPoint(mp))                  && !evnt.MouseOver;
                m_event.MouseEnter    = (!oldMouseOver && m_event.MouseOver)                    && !evnt.MouseEnter;
                m_event.MouseExit     = (oldMouseOver && !m_event.MouseOver)                    && !evnt.MouseExit;
                m_event.Click         = (m_event.MouseOver && !m_event.MouseDown && mouseDown)  && !evnt.Click;
                m_event.MouseDown     = (m_event.MouseOver && mouseDown)                        && !evnt.MouseDown;
                m_event.GainedFocus   = (!m_state.Focused && m_event.Click)                     && !evnt.GainedFocus;

                if(m_event.MouseDown)
                    m_event.Drag = true;
                else if(!mouseDown)
                    m_event.Drag = false;

                evnt.MouseOver   = m_event.MouseOver    || evnt.MouseOver;
                evnt.MouseEnter  = m_event.MouseEnter   || evnt.MouseEnter;
                evnt.MouseExit   = m_event.MouseExit    || evnt.MouseExit;
                evnt.Click       = m_event.Click        || evnt.Click;
                evnt.MouseDown   = m_event.MouseDown    || evnt.MouseDown;
                evnt.LostFocus   = m_event.LostFocus    || evnt.LostFocus;
                evnt.GainedFocus = m_event.GainedFocus  || evnt.GainedFocus;
                evnt.Drag        = m_event.MouseDown    || evnt.Drag;
            }
            m_event.LostFocus = (m_state.Focused && !m_event.MouseOver && mouseDown);
        }

        void BaseComponent::Update()
        {
            static Clock clock;
            const double delta = clock.Reset();
            const Vector2i mousePos = Mouse::GetPosition();
            bool click = Mouse::ButtonDown(Mouse::Left);
            Events event = { false };
            Renderer::ResetCamera();

            if(!m_parent)
            {
                for(auto i = s_processStack.rbegin(); i != s_processStack.rend(); ++i)
                    (*i)->ProcessEvents(mousePos, click, event);

                for(auto i : s_processStack)
                    i->p_Update(delta, mousePos);
            }
        }

        void BaseComponent::p_Update(double dt, const Vector2i& mp)
        {
            if(!m_state.Enabled)
                return;

            OnUpdate(dt);
            Draw();
            if(m_event.Click)
            {
                Raise();
                m_event.Click = false;
                OnMouseClick(mp);
                if(m_onClickFunc)
                    m_onClickFunc(this);
            }
            else if(m_event.MouseDown)
            {
                OnMouseDown(mp);
                if(m_onMouseDownFunc)
                    m_onMouseDownFunc(this);
            }
            if(m_event.MouseEnter)
            {
                m_event.MouseEnter = false;
                OnMouseEnter();
                if(m_onMouseEnterFunc)
                    m_onMouseEnterFunc(this);
            }
            else if(m_event.MouseExit)
            {
                m_event.MouseExit = false;
                OnMouseExit();
                if(m_onMouseExitFunc)
                    m_onMouseExitFunc(this);
            }
            if(m_event.MouseOver)
            {
                OnMouseHover(mp);
                if(m_onHoverFunc)
                    m_onHoverFunc(this);
            }
            if(m_event.GainedFocus)
            {
                m_event.GainedFocus = false;
                m_state.Focused = true;
                OnFocused();
                if(m_onFocused)
                    m_onFocused(this);
            }
            if(m_event.LostFocus)
            {
                m_event.LostFocus = false;
                m_state.Focused = false;
                OnFocusLost();
                if(m_onFocusLost)
                    m_onFocusLost(this);
            }
            if(m_event.Drag)
            {
                OnMouseDrag(mp);
                if(m_onDragFunc)
                    m_onDragFunc(this);
            }

            if(m_onUpdate)
                m_onUpdate(this);
//            for(auto& i : m_children)
//                i.second->p_Update(dt, mp);
        }

        // Override these in derived classes to add internal event based functionality.
        void BaseComponent::OnMouseClick(const Vector2i& mousePos){}
        void BaseComponent::OnMouseDown(const Vector2i& mousePos){}
        void BaseComponent::OnMouseHover(const Vector2i& mousePos){}
        void BaseComponent::OnMouseEnter(){}
        void BaseComponent::OnMouseExit(){}
        void BaseComponent::OnMouseDrag(const Vector2i& mousePos){}
        void BaseComponent::OnUpdate(double dt){}
        void BaseComponent::OnFocused(){}
        void BaseComponent::OnFocusLost(){}
    }
}