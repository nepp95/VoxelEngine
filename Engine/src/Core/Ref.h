#pragma once

namespace VoxelEngine
{
	// Ref counter class
	class RefCounted
	{
	public:
		void IncrementRefCount() const;
		void DecrementRefCount() const;

		uint32_t GetRefCount() const { return m_refCount; }

	private:
		mutable uint32_t m_refCount{ 0 };
	};

	template<typename T>
	class Ref
	{
	public:
		// Constructors
		// Default constructor
		Ref()
			: m_instance(nullptr)
		{}

		// Arg is nullptr
		Ref(std::nullptr_t n)
			: m_instance(nullptr)
		{}
		
		// Class of base type ref counted arg
		Ref(T* instance)
			: m_instance(instance)
		{
			static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");
			IncrementRef();
		};

		// Other Ref class arg
		Ref(const Ref<T>& other)
			: m_instance(other.m_instance)
		{
			IncrementRef();
		}

		// Convert to T2
		template<typename T2>
		Ref(const Ref<T2>& other)
		{
			m_instance = (T*) other.m_instance;
			IncrementRef();
		}

		// Destructor, decreases ref count
		~Ref()
		{
			DecrementRef();
		}; 

		// If object
		operator bool() { return m_instance != nullptr; }
		operator bool() const { return m_instance != nullptr; }

		// Pointer to object
		T* operator->() { return m_instance; }
		const T* operator->() const { return m_instance; }

		// Dereferenced object
		T& operator*() { return *m_instance; }
		const T& operator*() const { return *m_instance; }

		// Equals
		bool operator==(const Ref<T>& other) const
		{
			return m_instance == other.m_instance;
		}

		// Not equals
		bool operator!=(const Ref<T>& other) const
		{
			return !(*this == other);
		}

		// Assignment with nullptr
		Ref& operator=(std::nullptr_t)
		{
			DecrementRef();
			m_instance = nullptr;
			return *this;
		}

		// Assignment with other ref object
		Ref& operator=(const Ref<T>& other)
		{
			other.IncrementRef();
			DecrementRef();

			m_instance = other.m_instance;
			return *this;
		}

		template<typename T2>
		Ref& operator=(const Ref<T2>& other)
		{
			other.IncrementRef();
			DecrementRef();

			m_instance = other.m_instance;
			return *this;
		}

		// Cast to T2
		template<typename T2>
		Ref<T2> As() const
		{
			return Ref<T2>(*this);
		}

		// Raw pointer
		T* Raw() { return m_instance; }
		const T* Raw() const { return m_instance; }

		// Create refs
		template<typename... Args>
		static Ref<T> Create(Args&&... args)
		{
			return Ref<T>(new T(std::forward<Args>(args)...));
		}

		//  Increment/decrement
		void IncrementRef() const
		{
			if (m_instance)
				m_instance->IncrementRefCount();
		}

		void DecrementRef() const
		{
			if (m_instance)
			{
				m_instance->DecrementRefCount();

				if (m_instance->GetRefCount() == 0)
				{
					delete m_instance;
					m_instance = nullptr;
				}
			}
		}

	private:
		friend class Ref;
		mutable T* m_instance;
	};
}