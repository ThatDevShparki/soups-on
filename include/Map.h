#pragma once

#include "SFML/Graphics.hpp"
#include "Vec2.h"
#include <map>
#include <string>
#include <vector>
#include <tuple>

typedef std::tuple<std::vector<int>, Vec2> MapLayerData;

class Map
{
	size_t m_width      = 0;
	size_t m_height     = 0;
	size_t m_layerCount = 0;

	// 1D array of m_width * m_height * m_layerCount length
	std::vector<int> m_data;

	void init(const std::vector<std::string>& layerPaths);

	[[nodiscard]] size_t indexOf(const Vec2& pos, size_t layer) const;
	[[nodiscard]] static const MapLayerData
	readLayerData(const std::string& layerPath);

public:

	Map();
	Map(const std::vector<std::string>& layerPaths);

	[[nodiscard]] const std::vector<int>& data() const;

	[[nodiscard]] std::vector<int> at(const Vec2& pos) const;
	[[nodiscard]] std::vector<int> at(unsigned int i, unsigned int j) const;
	[[nodiscard]] int at(const Vec2& pos, size_t layer) const;

	void addLayer(const std::string& layerPath);
	void addLayer(const std::string& layerPath, int index);

	void print() const;

	[[nodiscard]] size_t width() const;
	[[nodiscard]] size_t height() const;
	[[nodiscard]] size_t layerCount() const;
};
