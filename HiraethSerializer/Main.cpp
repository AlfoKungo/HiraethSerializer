#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>
#include <fstream>
#include <istream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <iostream>
#include <filesystem>
#include <array>
#include <string>>

#include "ImageLoad.h"
#include "map_data.h"
#include "tile_texture_data.h"

#define MAP_AMOUNT 100
#define TEXTURES_AMOUNT 100
#define TILES_AMOUNT 100
#define ADDRESSES_AMOUNT (TEXTURES_AMOUNT + MAP_AMOUNT + TILES_AMOUNT)
#define MAX_ADDRESSES 100

#define MAP_DATA_ADDRESSES_BEGIN 0
#define MAP_DATA_DATA_BEGIN (ADDRESSES_AMOUNT * sizeof(Address))
#define TEXTURES_ADDRESSES_BEGIN (MAP_AMOUNT * sizeof(Address))
#define TILES_ADDRESSES_BEGIN ((MAP_AMOUNT + TEXTURES_AMOUNT) * sizeof(Address))

#define USED_MAPS 2

namespace fs = std::experimental::filesystem;

const int zero = 0;

typedef int Address;


template<class DATA>
void serialize_data(std::vector<DATA> data, int addresses_begin, std::ofstream& out, cereal::BinaryOutputArchive& arout)
{

	int i = 0;
	Address addresses[MAX_ADDRESSES];
	std::fill_n(addresses, MAP_AMOUNT, 0);
	std::cout << "start serializing data at address " << out.tellp() << std::endl;
	for (auto & d : data)
	{
		addresses[i] = out.tellp();
		arout(d);
		i++;
	}
	out.seekp(addresses_begin);
	std::cout << "start serializing addresses at address " << out.tellp() << std::endl;
	for (const Address& address : addresses)
		arout(address);
}


int main()
{

	// iterate over all folders and serialize each
	for (auto & sub_dir : fs::directory_iterator("data/"))
	{
		std::ofstream out(sub_dir.path().filename().string() + ".data", std::ios::out | std::ios::binary);
		if (out.is_open())
		{
			cereal::BinaryOutputArchive arout(out);

			std::vector<MapData> v_mapData;
			v_mapData.reserve(100);
			for (auto & p : fs::directory_iterator(sub_dir.path() / "map_data"))
			{
				std::ifstream in(p.path(), std::ios::in);
				cereal::XMLInputArchive arin(in);
				MapData map_data = MapData{};
				arin(map_data);
				v_mapData.push_back(map_data);
			}
			out.seekp(MAP_DATA_DATA_BEGIN);
			serialize_data(v_mapData, MAP_DATA_ADDRESSES_BEGIN, out, arout);

			std::vector<TextureData> v_textureData;
			v_textureData.reserve(100);
			for (auto & p : fs::directory_iterator(sub_dir.path() / "textures"))
			{
				TextureData texture_data{};
				texture_data.pic = hiraeth_serializer::load_image(p.path().string().c_str(), &texture_data.width, &texture_data.height);
				texture_data.texture_data_size = (sizeof(texture_data.pic) / sizeof(texture_data.pic[0])) * texture_data.width * texture_data.height;
				v_textureData.push_back(texture_data);
			}
			out.seekp(0, out.end);
			serialize_data(v_textureData, TEXTURES_ADDRESSES_BEGIN, out, arout);

			std::vector<TileTextureData> v_tileTextureData;
			v_tileTextureData.reserve(100);
			for (auto & p : fs::directory_iterator(sub_dir.path() / "tiles"))
			{
				std::string path = p.path().string(), index = p.path().filename().string();
				std::string tile_name = path + "\\tile_" + index + ".png";
				std::string tile_data_name = path + "\\tile_data_" + index + ".xml";
				std::ifstream in(tile_data_name, std::ios::in);
				cereal::XMLInputArchive arin(in);
				TileTextureData Td{};

				arin(Td.TilesUV);
				Td.pic = hiraeth_serializer::load_image(tile_name.c_str(), &Td.width, &Td.height);
				Td.texture_data_size = (sizeof(Td.pic) / sizeof(Td.pic[0])) * Td.width * Td.height;
				v_tileTextureData.push_back(Td);
			}
			out.seekp(0, out.end);
			serialize_data(v_tileTextureData, TILES_ADDRESSES_BEGIN, out, arout);


			out.close();
		}
		std::cout << "finished serializing " << sub_dir << std::endl;
	}
	std::cout << "finished";
	std::cin.get();
	return 0;
}