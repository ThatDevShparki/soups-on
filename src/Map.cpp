#include "Map.h"
#include <iostream>
#include <fstream>
#include<sstream>
#include <stdexcept>

Map::Map() = default;

Map::Map(const std::vector<std::string>& layerPaths)
{
	init(layerPaths);
}


void Map::init(const std::vector<std::string>& layerPaths)
{
	if (layerPaths.empty())
	{
		std::cerr << "expected non-empty list of layer paths" << std::endl;
		throw std::invalid_argument("layerPaths should be non-empty");
	}

	// read and verify layers
	std::vector<std::vector<int>> rawLayerData;
	std::vector<Vec2>             rawLayerSizes;

	for (auto& path: layerPaths)
	{
		auto& [layerData, layerSize] = readLayerData(path);
		rawLayerData.push_back(layerData);
		rawLayerSizes.push_back(layerSize);
	}

	// verify layers sizes are consistent
	Vec2 layerSize = rawLayerSizes[0];
	for (const auto& s: rawLayerSizes)
	{
		if (s != layerSize)
		{
			std::cerr << "expected consistently sized layers" << std::endl;
			throw std::invalid_argument("layers should be the same size");
		}
	}

	// update data and internal attributes
	m_width      = size_t(layerSize.x);
	m_height     = size_t(layerSize.y);
	m_layerCount = layerPaths.size();

	for (const auto& ldata: rawLayerData)
	{
		for (const auto& e: ldata)
		{
			m_data.push_back(e);
		}
	}

	std::cout << "Loaded map with size (" + std::to_string(m_width) + ", " +
				 std::to_string(m_height) + ") and with " +
				 std::to_string(m_layerCount) + " layers (" +
				 std::to_string(m_data.size()) + " elements)" << std::endl;
}

const MapLayerData Map::readLayerData(const std::string& layerPath)
{
	// read and verify file
	std::vector<std::vector<int>> rawData;
	std::ifstream                 file(layerPath);

	if (!file.is_open())
	{
		std::cerr << "Could not load map layer from path: " + layerPath << std::endl;
		throw std::invalid_argument("file could not be loaded");
	}

	for (std::string row; std::getline(file, row);)
	{
		std::vector<int>   rowData;
		std::istringstream ss(row);

		for (std::string e; std::getline(ss, e, ',');)
		{
			rowData.push_back(std::stoi(e));
		}

		rawData.push_back(rowData);
	}

	file.close();

	// initialize and verify map sizes
	size_t height = rawData.size();
	if (height == 0)
	{
		std::cerr << "Received empty map from path: " + layerPath << std::endl;
		throw std::invalid_argument("empty map is invalid");
	}

	size_t width = rawData[0].size();
	if (width == 0)
	{
		std::cerr << "Received empty map from path: " + layerPath << std::endl;
		throw std::invalid_argument("empty map is invalid");
	}

	for (auto& row: rawData)
	{
		if (row.size() != width)
		{
			std::cerr << "Received inconsistent map from path: " + layerPath
					  << std::endl;
			throw std::invalid_argument("inconsistent map is invalid");
		}
	}


	// transpose raw data to a single list of all data
	std::vector<int> data;

	for (int j = int(height) - 1; j >= 0; j--)
	{
		for (int i = 0; i < width; i++)
		{
			data.push_back(rawData[j][i]);
		}
	}

	std::cout << "Loaded layer from " + layerPath + " with size (" +
				 std::to_string(width) + ", " + std::to_string(height) + ") (" +
				 std::to_string(data.size()) + " elements)"
			  << std::endl;

	return { data, { float(width), float(height) }};
}


size_t Map::indexOf(const Vec2& pos, size_t layer) const
{
	return (
		(m_width * m_height * layer)
		+ size_t(pos.y) * m_width
		+ size_t(pos.x)
	);
}

int Map::at(const Vec2& pos, size_t layer) const
{
	size_t index = indexOf(pos, layer);

	if (index >= m_data.size())
	{
		throw std::invalid_argument("pos and layer exceed maximum allowed coords");
	}
	return m_data.at(index);
}


void Map::print() const
{
	int index = 0;

	std::vector<std::string> pstrings;

	for (int l = 0; l < m_layerCount; l++)
	{
		for (int j = 0; j < m_height; j++)
		{
			std::stringstream row;
			for (int          i = 0; i < m_width; i++)
			{
				row << std::setw(10)
					<< std::to_string(m_data[index]) + " (" + std::to_string(index) +
					   ") ";
				index++;
			}
			pstrings.push_back(row.str());
		}
		pstrings.push_back("");
	}

	std::reverse(pstrings.begin(), pstrings.end());

	for (const auto& s: pstrings)
	{
		std::cout << s << std::endl;
	}
}


size_t Map::width() const
{
	return m_width;
}

size_t Map::height() const
{
	return m_height;
}

size_t Map::layerCount() const
{
	return m_layerCount;
}