#pragma once
#include <memory>

struct vec2 {
	float x, y;

	template<class Archive>
	void serialize(Archive & archive) { archive(x, y); }

	vec2& Add(const vec2& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	vec2& Subtract(const vec2& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	vec2& Multiply(const vec2& other)
	{
		x *= other.x;
		y *= other.y;

		return *this;
	}

	vec2& Divide(const vec2& other)
	{
		x /= other.x;
		y /= other.y;

		return *this;
	}

	vec2& Add(float value)
	{
		x += value;
		y += value;

		return *this;
	}

	vec2& Subtract(float value)
	{
		x -= value;
		y -= value;

		return *this;
	}

	vec2& Multiply(float value)
	{
		x *= value;
		y *= value;

		return *this;
	}

	vec2& Divide(float value)
	{
		x /= value;
		y /= value;

		return *this;
	}

	friend vec2 operator+(vec2 left, const vec2& right)
	{
		return left.Add(right);
	}

	friend vec2 operator-(vec2 left, const vec2& right)
	{
		return left.Subtract(right);
	}

	friend vec2 operator*(vec2 left, const vec2& right)
	{
		return left.Multiply(right);
	}

	friend vec2 operator/(vec2 left, const vec2& right)
	{
		return left.Divide(right);
	}

	friend vec2 operator+(vec2 left, float value)
	{
		return vec2{ left.x + value, left.y + value };
	}

	friend vec2 operator-(vec2 left, float value)
	{
		return vec2{left.x - value, left.y - value};
	}

	friend vec2 operator*(vec2 left, float value)
	{
		return vec2{left.x * value, left.y * value};
	}

	friend vec2 operator/(vec2 left, float value)
	{
		return vec2{left.x / value, left.y / value};
	}

	vec2& operator+=(const vec2& other)
	{
		return Add(other);
	}

	vec2& operator-=(const vec2& other)
	{
		return Subtract(other);
	}

	vec2& operator*=(const vec2& other)
	{
		return Multiply(other);
	}

	vec2& operator/=(const vec2& other)
	{
		return Divide(other);
	}

	vec2& operator+=(float value)
	{
		return Add(value);
	}

	vec2& operator-=(float value)
	{
		return Subtract(value);
	}

	vec2& operator*=(float value)
	{
		return Multiply(value);
	}

	vec2& operator/=(float value)
	{
		return Divide(value);
	}
};

struct TileData
{
	vec2 position;
	float scale;
	//vec2 uv_pos;
	//vec2 uv_size;
	unsigned int type;
	template<class Archive>
	void serialize(Archive & ar)
	{
		ar(CEREAL_NVP(position), CEREAL_NVP(scale), CEREAL_NVP(type));
	}
};
struct FootHoldData
{
	vec2 p1, p2;
	vec2 direction;
	template<class Archive>
	void serialize(Archive & ar)
	{
		ar(CEREAL_NVP(p1), CEREAL_NVP(p2), CEREAL_NVP(direction));
	}
};
struct PortalData
{
	vec2 position;
	int next_map;
	template<class Archive>
	void serialize(Archive & ar)
	{
		ar(CEREAL_NVP(position), CEREAL_NVP(next_map));
	}
};
struct Summon
{
	unsigned int monster_type;
	vec2 position;
	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(monster_type), CEREAL_NVP(position));
	}
};

FootHoldData HorizontalFootHold(vec2 p1, float length)
{
	return FootHoldData{ p1 - vec2{ 22.5f, -3 }, vec2{ p1.x + length + 22.5f, p1.y + 3 }, vec2{ 0, 1 } };
}
FootHoldData HorizontalFootHold(vec2 p1, vec2 p2)
{
	return FootHoldData{ p1 - vec2{22.5f, -3}, p2 + vec2{22.5f, 3 }, vec2{ 0, 1 } };
}

FootHoldData VerticalFootHold(vec2 p1, float length)
{
	return FootHoldData{ p1, vec2{p1.x, p1.y + length}, vec2{1, 0} };
}
FootHoldData VerticalFootHold(vec2 p1, vec2 p2)
{
	return FootHoldData{ p1, p2, vec2{ 1, 0 } };
}

struct MapData
{
	std::vector<TileData> Tiles;
	std::vector<FootHoldData> FootHolds;
	std::vector<PortalData> Portals;
	std::vector<Summon> Summons;
	int TileTexture = 1;
private:
	MapData() = default;
	friend class cereal::access;
	template<class A> void serialize(A& ar) {
		ar(CEREAL_NVP(Tiles),
			CEREAL_NVP(FootHolds),
			CEREAL_NVP(Portals),
			CEREAL_NVP(Summons),
			CEREAL_NVP(TileTexture));
	}
};