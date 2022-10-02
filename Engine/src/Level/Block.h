#pragma once

namespace VoxelEngine
{
	enum class BlockType
	{
		Air = 0,
		Dirt,
		Grass,
		Sand,
	};

	struct BlockData
	{
		BlockType Type;

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