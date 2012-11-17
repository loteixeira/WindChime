// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef VERTEX_H_
#define VERTEX_H_

class Vertex
{
public:
	float u, v;
	float red, green, blue, alpha;
	float nx, ny, nz;
	float x, y, z;

	Vertex()
	{
		u = 0.0f;
		v = 0.0f;
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;
		alpha = 1.0f;
		nx = 0.0f;
		ny = 0.0f;
		nz = 0.0f;
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vertex(const Vertex& other)
	{
		u = other.u;
		v = other.v;
		red = other.red;
		green = other.green;
		blue = other.blue;
		alpha = other.alpha;
		nx = other.nx;
		ny = other.ny;
		nz = other.nz;
		x = other.x;
		y = other.y;
		z = other.z;
	}

	Vertex operator=(const Vertex& other)
	{
		u = other.u;
		v = other.v;
		red = other.red;
		green = other.green;
		blue = other.blue;
		alpha = other.alpha;
		nx = other.nx;
		ny = other.ny;
		nz = other.nz;
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	bool operator==(Vertex& other)
	{
		return u == other.u && v == other.v && red == other.red && green == other.green && blue == other.blue && alpha == other.alpha && nx == other.nx && ny == other.ny && nz == other.nz && x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(Vertex& other)
	{
		return u != other.u || v != other.v || red != other.red || green != other.green || blue != other.blue || alpha != other.alpha || nx != other.nx || ny != other.ny || nz != other.nz || x != other.x || y != other.y || z != other.z;
	}
};

#endif /* VERTEX_H_ */
