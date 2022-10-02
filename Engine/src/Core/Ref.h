#pragma once

namespace VoxelEngine
{
	// Ref counter class
	class RefCounted
	{
	public:
		void Increment() const;
		void Decrement() const;

		uint32_t GetRefCount() const { return m_refCount; }

	private:
		mutable uint32_t m_refCount{ 0 };
	};

	template<typename T>
	class Ref
	{
	public:
		Ref()
			: m_instance(nullptr)
		{}

		// Lookup
		Ref(std::nullptr_t n)
			: m_instance(nullptr)
		{}

		Ref(T* instance)
			: m_instance(instance)
		{
			static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");
			m_instance->Increment();
		}

	private:
		friend class Ref;
		mutable T* m_instance;
	};
}