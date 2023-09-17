#pragma once

#include "Math.h"
#include <vector>

// TODO make Vertex a template parameter for undefined layouts
struct Vertex 
{
	Math::Vec3<float> position;
	Math::Vec3<float> normal{1,0,0};


};

typedef unsigned int Index;

struct Mesh {

	std::vector<Vertex> vertices;
	std::vector<Index> indices;

};