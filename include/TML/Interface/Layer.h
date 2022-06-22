#pragma once
#include <TML/Interface/BaseComponent.h>

namespace tml::Interface
{
    class TML_API Layer : public Drawable
    {
    public:
        Layer() = default;
        Layer(Layer&& layer) noexcept;
        void Attach(BaseComponent* rootNode) noexcept;
        void Detach(BaseComponent* component) noexcept;
        void Raise(BaseComponent* component) noexcept;
        void ClearFocused() noexcept;
        void Update(Event& event) const noexcept;
        void SetEnabled(bool enabled) noexcept;
        [[nodiscard]] bool IsEnabled() const noexcept;

    protected:
        void OnDraw(RenderTarget* renderer, Texture*) noexcept final;

    protected:
        std::deque<BaseComponent*> m_processingQueue;
        std::unique_ptr<BaseComponent> m_rootNode = nullptr;
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