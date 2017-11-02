#include <cereal/archives/binary.hpp>
#include <fstream>
#include <istream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include "ImageLoad.h"
#include <iostream>
#include <filesystem>

namespace fs = std::experimental::filesystem;

int main()
{
	// iterate over all folders and serialize each
	for (auto & sub_dir : fs::directory_iterator("data/"))
	{
		std::ofstream out(sub_dir.path().filename().string() + ".data", std::ios::out | std::ios::binary);
		int zero = 0;
		if (out.is_open())
		{
			cereal::BinaryOutputArchive ar(out);
			// set begining to zeros
			out.seekp(0);
			for (int i = 0; i < 100; i++)
				ar(zero);
			out.seekp(0);

			// serialize the file
			std::string path = sub_dir.path().string();
			int width, height;
 			int count = 100 * sizeof(int), i = 0;
			for (auto & p : fs::directory_iterator(path))
			{
				out.seekp(i * sizeof(int));
				i++;
				ar(count);
				BYTE* pic = hiraeth_serializer::load_image(p.path().string().c_str(), &width, &height);
				int dataSize = (sizeof(pic) / sizeof(pic[0])) * width * height;
				out.seekp(count);
				ar(width, height, dataSize, cereal::binary_data(pic, dataSize));
				count += sizeof(width) + sizeof(height) + sizeof(dataSize) + dataSize;

				std::cout << p << std::endl;
			}

			out.close();
		}
		std::cout << "finished serializing " << sub_dir << std::endl;
	}
	std::cout << "finished";
	return 0;
}