#include "pch.h"
#include "UUID.h"

#include "Utility/Random.h"

namespace VoxelEngine
{
	UUID::UUID()
		: m_UUID(Utility::GenerateRandomUInt64())
	{}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{}

	UUID::UUID(const UUID& other)
		: m_UUID(other.m_UUID)
	{}
}