#pragma once

#include "../Utils/Math.h"
#include "../Utils/Vertex.h"

#include <vector>

struct Cube : Mesh {

	Cube() {

		vertices =
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

		indices = {

			// Downface
			0,1,4,
			4,1,5,

			// Front face
			0,2,4,
			4,2,6,

			// Left
			0,1,3,
			0,3,2,

			// Top
			2,3,7,
			2,7,6,

			// Right

			4,6,7,
			4,7,5,

			// backface

			1,7,5,
			1,3,7
		};
	}
};