#pragma once

#include "../Utils/Math.h"
#include "../Utils/Vertex.h"

#include <vector>

struct Cube {

	/*
	
	
0.0000 1.0000 0.0000
0.0000 0.0000 1.0000
-1.0000 0.0000 0.0000
0.0000 -1.0000 0.0000
1.0000 0.0000 0.0000
0.0000 0.0000 -1.0000
	
	*/

	std::vector<Vertex> vertices =
	{
		{{-0.5 ,- 0.5, - 0.5}	,Math::Vec3<float>{-0.5 ,-0.5, -0.5}.normalize()	}, // 1
		{{-0.5 , -0.5, 0.5	}	,Math::Vec3<float>{-0.5 , -0.5, 0.5	}.normalize()	}, // 2
		{{-0.5 ,0.5, -0.5	}	,Math::Vec3<float>{-0.5 ,0.5, -0.5	}.normalize()	}, // 3
		{{-0.5 , 0.5, 0.5	}	,Math::Vec3<float>{-0.5 , 0.5, 0.5	}.normalize()	}, // 4
		{{ 0.5, -0.5, -0.5}	,	 Math::Vec3<float>{ 0.5, -0.5, -0.5}.normalize()	}, // 5
		{{ 0.5, -0.5, 0.5	}	,Math::Vec3<float>{ 0.5, -0.5, 0.5	}.normalize()	}, // 6 
		{{ 0.5,  0.5, -0.5	}	,Math::Vec3<float>{ 0.5,  0.5, -0.5	}.normalize()	}, // 7
		{{ 0.5,  0.5, 0.5	}	,Math::Vec3<float>{ 0.5,  0.5, 0.5	}.normalize()	}  // 8
	};

	// todo -1
	std::vector<Index> indices = {

		// Downface
		1,2,5,
		5,2,6,

		// Front face
		1,3,5,
		5,3,7,

		// Left
		1,2,4,
		1,4,3,

		// Top
		3,4,8,
		3,8,7,

		// Right

		5,7,8,
		5,8,6,

		// backface

		2,8,6,
		2,4,8
	};


};