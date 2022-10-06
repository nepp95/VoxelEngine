#include "pch.h"
#include "UUID.h"

namespace VoxelEngine
{
	static std::random_device s_randomDevice;
	static std::mt19937_64 s_engine(s_randomDevice());
	static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

	UUID::UUID()
		: m_UUID(s_uniformDistribution(s_engine))
	{}

	UUID::UUID(uint64_t uuid)
		: m_UUID(uuid)
	{}

	UUID::UUID(const UUID& other)
		: m_UUID(other.m_UUID)
	{}
}