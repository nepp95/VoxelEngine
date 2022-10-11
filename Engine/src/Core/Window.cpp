#include "pch.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace VoxelEngine
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		CORE_ERROR("GLFW Error: (%) %", error, description);
	}

	Window::Window(const WindowSpecification& specification)
		: m_specification(specification)
	{
		// TODO: Refactor when logging fixed
		CORE_INFO("Creating window of size %x%", m_specification.Width, m_specification.Height);

		// Init GLFW
		int success{glfwInit()};
		CORE_ASSERT(success, "Failed to initialize GLFW!");

		// Set callback for GLFW errors
		glfwSetErrorCallback(GLFWErrorCallback);

		// Create window
		#ifdef V_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
		#endif

		m_window = glfwCreateWindow(m_specification.Width, m_specification.Height, m_specification.Title.c_str(),
		                            nullptr, nullptr);
		glfwMakeContextCurrent(m_window);

		// Setup glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CORE_ASSERT(status, "Could not initialize Glad!");

		// Log computer specs
		CORE_INFO("GPU Info:");
		CORE_INFO("\tVendor: %", glGetString(GL_VENDOR));
		CORE_INFO("\tRenderer: %", glGetString(GL_RENDERER));
		CORE_INFO("\tVersion: %", glGetString(GL_VERSION));

		CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 6),
		            "Engine requires at least OpenGL version 4.6!");

		SetVSync(m_specification.VSync);

		// Event Callback
		glfwSetWindowUserPointer(m_window, &m_callback);

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			EventCallbackFn& callback = *(EventCallbackFn*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			callback(event);
		});

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			EventCallbackFn& callback = *(EventCallbackFn*)glfwGetWindowUserPointer(window);
			WindowResizeEvent event(width, height);
			callback(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			EventCallbackFn& callback = *(EventCallbackFn*) glfwGetWindowUserPointer(window);
			
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					callback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					callback(event);
					break;
				}

				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					callback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
		{
			EventCallbackFn& callback = *(EventCallbackFn*) glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			callback(event);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			EventCallbackFn& callback = *(EventCallbackFn*) glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					callback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					callback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			EventCallbackFn& callback = *(EventCallbackFn*) glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float) xOffset, (float) yOffset);
			callback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			EventCallbackFn& callback = *(EventCallbackFn*) glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float) xPos, (float) yPos);
			callback(event);
		});
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Shutdown()
	{
		// Don't use OpenGL destroy methods in destructors because of unexpected behaviour.
		// Source: https://www.khronos.org/opengl/wiki/Common_Mistakes#RAII_and_hidden_destructor_calls
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_window);
	}

	void Window::SetVSync(bool enabled)
	{
		// This is not a holy grail. The GPU can still refuse to VSync (or not). In most cases this is because of user settings.
		// So errors can be difficult to reproduce.
		glfwSwapInterval(enabled);

		m_specification.VSync = enabled;
	}
}
