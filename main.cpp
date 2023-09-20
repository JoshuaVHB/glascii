#include <iostream>

#include "Utils/Math.h"
#include "Utils/FPSCounter.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shapes.h"
#include "Renderer/Console.h"
#include "Renderer/Camera.h"
#include "Renderer/Inputs.h"


#include <algorithm>
#include <sstream>
#include <chrono>
#include <array>
#include <vector>
#include <thread>
#include <streambuf>
#include <ostream>
#include <string>


int main() {


	constexpr int width = 200;
	constexpr int height = 200;


	Console::changeZoom(1,1);
	Console::setTerminalScreenResolution(width, height);


	Display disp(width, height);
	FPSCounter fps;
	OrthographicCamera camera;
	Cube cube;

	std::vector<Vertex> v = cube.vertices;
	std::vector<Index> i = cube.indices;


	Math::Mat4x4<float> mat
	{
		{23,10,3,4,
		8,5,6,10,
		6,4,2,1,
		8,7,6,5	}
	};
	Math::Mat4x4<float> mat2{
		{
			2,4,6,1,
			1,8,7,0,
			9,9,72,1,
			2,3,4,5
		}

	};

	auto mat3 = mat * mat2;
	auto mat4 = mat * 3.5f;
	auto mat5 = 3 * mat;

	Camera orthocam(OrthographicProjection{-5,5,5,-5,0.1,100});
	auto vp = orthocam.getViewProjMatrix();


	while (true) {

		updateInputs();
		interpreteInputs();
		if (ESC_KEY_STATE) return 0;

		fps.step();
		Console::setTitle("FPS:" + std::to_string(fps.FPS));
		camera.updateCam(fps.elapsed, true);


		clearScreenBuffer(disp.pixelBuff, disp.colorBuff, disp.framebufferSize);
		clearDepth(disp.depthBuff);

		renderMesh(disp, camera, cube);

		preventResize(disp.pixelBuff, width, height);

		renderPixelBuffer(disp.pixelBuff, disp.framebufferSize);
		renderColorBuffer(disp.colorBuff, disp.framebufferSize);
		Sleep(1);


	}

	
	return 0;


}

