#include "pch.h"
#include "Converter.h"

#include "box2d/box2d.h"

namespace VoxelEngine
{
	b2BodyType Converter::RigidBodyTypeToBox2DBodyType(RigidBodyComponent::BodyType bodyType)
	{
		if (bodyType == RigidBodyComponent::BodyType::Static)		return b2_staticBody;
		if (bodyType == RigidBodyComponent::BodyType::Dynamic)		return b2_dynamicBody;
		if (bodyType == RigidBodyComponent::BodyType::Kinematic)	return b2_kinematicBody;

		VE_CORE_ERROR("Unknown body type!");
		return b2_staticBody;
	}

	std::string Converter::RigidBodyTypeToString(RigidBodyComponent::BodyType type)
	{
		if (type == RigidBodyComponent::BodyType::Static)			return "Static";
		if (type == RigidBodyComponent::BodyType::Dynamic)			return "Dynamic";
		if (type == RigidBodyComponent::BodyType::Kinematic)		return "Kinematic";
	}

	RigidBodyComponent::BodyType Converter::RigidBodyTypeFromString(const std::string& type)
	{
		if (type == "Static")										return RigidBodyComponent::BodyType::Static;
		if (type == "Dynamic")										return RigidBodyComponent::BodyType::Dynamic;
		if (type == "Kinematic")									return RigidBodyComponent::BodyType::Kinematic;
	}
}