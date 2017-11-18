#pragma once

#include <memory>
#include <cereal/archives/binary.hpp>
#include "ImageLoad.h"

struct TileUv
{
	vec2 UvPos;
	vec2 UvSize;
	template<class Archive>
	void serialize(Archive & ar)
	{
		ar(UvPos, UvSize);
	}
};

struct TileTextureData
{
	int width, height, texture_data_size;
	std::vector<TileUv> TilesUV;
	BYTE* pic;
	template<class Archive>
	void serialize(Archive & ar)
	{
		ar(width, height, texture_data_size, TilesUV);
		ar(cereal::binary_data(pic, texture_data_size));
	}
};

struct TextureData
{
	int width, height, texture_data_size;
	BYTE* pic;
	template<class Archive>
	void serialize(Archive & ar)
	{
		ar(width, height, texture_data_size);
		ar(cereal::binary_data(pic, texture_data_size));
	}
};