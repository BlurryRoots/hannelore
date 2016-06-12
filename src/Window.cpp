#include <Window.h>

using namespace blurryroots::hannelore;

std::vector<IGame*> WindowManager::m_handlers;
std::vector<GLFWmonitor*> WindowManager::m_secondary_monitors;
GLFWmonitor *WindowManager::m_primary_monitor;
std::vector<Window> WindowManager::m_windows;
bool WindowManager::m_is_glfw_initialized;
bool WindowManager::m_is_glew_initialized;
bool WindowManager::m_is_opengl_initialized;
