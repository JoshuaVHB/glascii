#pragma once

#include "Math.h"

struct Vertex 
{
	Math::Vec3<float> position;
	Math::Vec3<float> normal{1,0,0};


};

typedef unsigned int Index;