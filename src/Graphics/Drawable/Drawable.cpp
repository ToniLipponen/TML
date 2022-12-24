#include <TML/Graphics/Drawable/Drawable.h>

namespace tml
{
	Color Drawable::SetColor(const Color& color) noexcept
	{
		/**
		 * Checking if m_color is already the same as color.
		 * Because this would otherwise invalidate the old vertex data.
		 */
		if(m_color != color)
		{
			m_updated = true;
			m_color = color;
		}

		return m_color;
	}

	Color Drawable::GetColor() const noexcept
	{
		return m_color;
	}
}