#pragma once
class Tile
{
public:
	Tile();
	Tile(int x, int y, int z);

	bool operator ==(const Tile &other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	int x, y, z;
};

