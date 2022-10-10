#pragma once

#include "Asset/Asset.h"

namespace VoxelEngine
{
	enum class BlockType
	{
		Air = 0,
		Coal,
		Dirt,
		Glass,
		Gold,
		Grass,
		Redstone,
		Sand,
		Stone,
		Tnt,
		Water,
		Wood
	};

	struct BlockData
	{
		AssetHandle TextureHandle{ 0 };
		BlockType Type;

		BlockData()
			: Type(BlockType::Air)
		{}

		BlockData(BlockType type)
			: Type(type)
		{}
	};

	class Block
	{
	public:
		Block(const BlockData& data);

		BlockType GetBlockType() const { return m_blockData.Type; }
		void SetBlockType(BlockType type) { m_blockData.Type = type; }

	private:
		BlockData m_blockData;
	};
}