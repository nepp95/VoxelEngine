#pragma once

#include <string>

int main(int argc, char** argv);

namespace VoxelEngine
{
	struct ApplicationCommandLineArgs
	{
		int Count{0};
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			// TODO: Assert index < count
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name{"Application"};
		std::string WorkingDirectory;

		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		~Application();

		const ApplicationSpecification& GetSpecification() const { return m_specification; }

		static Application& Get() { return *s_instance; }

	private:
		void Run();

	private:
		ApplicationSpecification m_specification;

		static Application* s_instance;
		friend int ::main(int argc, char** argv);
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}