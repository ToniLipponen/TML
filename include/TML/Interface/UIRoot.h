#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml::Interface
{
    class UIRoot : public Drawable
    {
    public:
        UIRoot() noexcept = default;
        void Attach(BaseComponent* rootNode) noexcept;
        void Detach(BaseComponent* component) noexcept;
        void Raise(BaseComponent* component) noexcept;
        void ClearFocused() noexcept;
        void Update(Event& event) const noexcept;

    protected:
        void OnDraw(RenderTarget* renderer, Texture*) noexcept final;

    protected:
        std::deque<BaseComponent*> m_processingQueue;
        std::unique_ptr<BaseComponent> m_rootNode;

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