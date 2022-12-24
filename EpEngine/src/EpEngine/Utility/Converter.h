#pragma once

#include "EpEngine/Scene/Components.h"

namespace EpEngine
{
	struct Converter
	{
		// Delete constructors/destructor
		Converter() = delete;
		Converter(const Converter&) = delete;
		~Converter() = delete;

		/************************************************************************/
		/* Box2D                                                                */
		/************************************************************************/
		static b2BodyType RigidBodyTypeToBox2DBodyType(RigidBodyComponent::BodyType bodyType);

		/************************************************************************/
		/* Components                                                           */
		/************************************************************************/
		static std::string RigidBodyTypeToString(RigidBodyComponent::BodyType type);
		static RigidBodyComponent::BodyType RigidBodyTypeFromString(const std::string& type);
	};
}