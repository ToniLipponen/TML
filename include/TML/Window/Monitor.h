#pragma once
#include <TML/System/String.h>
#include <TML/System/Math/Vector2.h>
#include <TML/Export.h>

namespace tml
{
    class TML_API Monitor
    {
	private:
		Monitor(void* handle)
		: m_handle(handle)
		{

		}

    public:
        Monitor() = default;

        [[nodiscard]] void* GetHandle() const noexcept;                 //!< Returns the GLFW handle of the monitor.
        [[nodiscard]] String GetName() const noexcept;                  //!< Returns the name of the monitor.
        [[nodiscard]] Vector2i GetSize() const noexcept;                //!< Returns the resolution of the monitor.
        [[nodiscard]] Vector2i GetPosition() const noexcept;            //!< Returns the position of the monitor within a multi-monitor surface.
        [[nodiscard]] Vector2i GetWorkAreaPosition() const noexcept;    //!< Returns the work area position.
        [[nodiscard]] Vector2i GetWorkAreaSize() const noexcept;        //!< Returns the available work area size.
        [[nodiscard]] Vector2i GetPhysicalSize() const noexcept;        //!< Returns the physical size of the monitor in millimeters.
        [[nodiscard]] float GetDiagonalSize() const noexcept;           //!< Returns the size of the monitor in inches.
        [[nodiscard]] float GetGamma() const noexcept;                  //!< Returns monitor gamma.
        [[nodiscard]] Vector2f GetContentScale() const noexcept;        //!< Returns the UI scale as a Vector2.
        void SetGamma(float gamma) noexcept;                            //!< Sets monitor gamma.

        friend TML_API std::vector<Monitor> GetMonitors() noexcept;
        friend TML_API Monitor GetPrimaryMonitor() noexcept;
		friend class Window;
        
    private:
        void* m_handle = nullptr;
        String m_monitorName;
        Vector2i m_size;
        Vector2i m_pos;
        Vector2i m_workAreaPos;
        Vector2i m_workAreaSize;
        Vector2i m_physicalSize;
        Vector2f m_contentScale;
        float m_gamma = 1;
    };

    TML_API std::vector<Monitor> GetMonitors() noexcept;
    TML_API Monitor GetPrimaryMonitor() noexcept;
}