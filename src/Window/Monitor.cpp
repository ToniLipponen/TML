#include <TML/Window/Monitor.h>
#include <cmath>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace tml
{
    void* Monitor::GetHandle() const noexcept
    {
        return m_handle;
    }

    String Monitor::GetName() const noexcept
    {
        return m_monitorName;
    }

    Vector2i Monitor::GetPosition() const noexcept
    {
        return m_pos;
    }

    Vector2i Monitor::GetSize() const noexcept
    {
        return m_size;
    }

    Vector2i Monitor::GetWorkAreaPosition() const noexcept
    {
        return m_workAreaPos;
    }

    Vector2i Monitor::GetWorkAreaSize() const noexcept
    {
        return m_workAreaSize;
    }

    Vector2i Monitor::GetPhysicalSize() const noexcept
    {
        return m_physicalSize;
    }

    float Monitor::GetDiagonalSize() const noexcept
    {
        return static_cast<float>(sqrt(pow(m_physicalSize.x, 2) + pow(m_physicalSize.y, 2)) / 2.54);
    }

    float Monitor::GetGamma() const noexcept
    {
        return m_gamma;
    }

    Vector2f Monitor::GetContentScale() const noexcept
    {
        return m_contentScale;
    }

    void Monitor::SetGamma(float gamma) noexcept
    {
        if(m_handle)
        {
            m_gamma = gamma;
            glfwSetGamma(static_cast<GLFWmonitor*>(m_handle), gamma);
        }
    }

    std::vector<Monitor> GetMonitors() noexcept
    {
        glfwInit();
        int monitorCount = 0;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

        std::vector<Monitor> monitorsVector;

        if(monitors)
        {
            for(int i = 0; i < monitorCount; i++)
            {
                Monitor monitor;

                monitor.m_handle = monitors[i];

                glfwGetMonitorWorkarea(monitors[i],
                                       &monitor.m_workAreaPos.x,
                                       &monitor.m_workAreaPos.y,
                                       &monitor.m_workAreaSize.x,
                                       &monitor.m_workAreaSize.y);

                glfwGetMonitorPos(monitors[i], &monitor.m_pos.x, &monitor.m_pos.y);

                glfwGetMonitorContentScale(monitors[i],
                                           &monitor.m_contentScale.x,
                                           &monitor.m_contentScale.y);

                glfwGetMonitorPhysicalSize(monitors[i],
                                           &monitor.m_physicalSize.x,
                                           &monitor.m_physicalSize.y);

                monitor.m_size = {monitor.m_workAreaSize.x + monitor.m_workAreaPos.x - monitor.m_pos.x,
                                  monitor.m_workAreaSize.y + monitor.m_workAreaPos.y - monitor.m_pos.y};

                monitor.m_monitorName.assign(glfwGetMonitorName(monitors[i]));

                monitorsVector.push_back(monitor);
            }
        }

        return monitorsVector;
    }

    Monitor GetPrimaryMonitor() noexcept
    {
        const auto monitors = GetMonitors();

        /// No monitors found.
        if(monitors.empty())
        {
            return {};
        }

        return monitors.front();
    }
}