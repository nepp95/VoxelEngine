#include "pch.h"
#include "Ref.h"

namespace VoxelEngine
{
	void RefCounted::Increment() const
	{
		++m_refCount;
	}

	void RefCounted::Decrement() const
	{
		--m_refCount;
	}
}