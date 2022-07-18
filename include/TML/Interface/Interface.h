#pragma once
#include <TML/Interface/Component.h>

namespace tml::Interface
{
    class TML_API Interface : public Drawable
    {
    public:
        Interface() = default;
        Interface(Interface&& other) noexcept;
        void Attach(Component* rootNode) noexcept;
        void Detach(Component* component) noexcept;
        void Focus(Component* component) noexcept;
        void ClearFocused() noexcept;
        void ClearDragged() noexcept;
        void ClearMouseOver() noexcept;
        void Update(Event& event) noexcept;
        void SetEnabled(bool enabled) noexcept;
        [[nodiscard]] bool IsEnabled() const noexcept;

    protected:
        void OnDraw(RenderTarget* renderer, Texture*) noexcept final;
        void HandleEvent(Event& event) noexcept;

    protected:
        std::vector<std::unique_ptr<Component>> m_roots;
        std::deque<Component*> m_processingQueue;
        bool m_enabled = true;

    private:
        using Drawable::GetColor;
        using Drawable::SetColor;
        using Transformable::GetPosition;
        using Transformable::GetSize;
        using Transformable::GetOrigin;
        using Transformable::GetRotation;
        using Transformable::GetCenter;
        using Transformable::Move;
        using Transformable::SetPosition;
        using Transformable::SetSize;
        using Transformable::Rotate;
        using Transformable::SetRotation;
        using Transformable::SetOrigin;
        using Transformable::ApplyOriginToPosition;
    };
}