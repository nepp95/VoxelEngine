#include "pch.h"
#include "Ref.h"

namespace VoxelEngine
{
	// RefCounted
	void RefCounted::IncrementRefCount() const
	{
		++m_refCount;
	}

	void RefCounted::DecrementRefCount() const
	{
		--m_refCount;
	}
}