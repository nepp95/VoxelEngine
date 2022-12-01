#pragma once

extern "C" {
	using MonoClass = struct _MonoClass;
	using MonoObject = struct _MonoObject;
	using MonoMethod = struct _MonoMethod;
}

namespace VoxelEngine
{
	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);

	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		friend class ScriptClass;
	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int paramCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

	private:
		std::string m_classNamespace;
		std::string m_className;

		MonoClass* m_monoClass{ nullptr };
	};
}
