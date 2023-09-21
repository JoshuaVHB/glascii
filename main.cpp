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


	Camera orthocam(OrthographicProjection{-3,3,3,-3,0.1,100});
	auto vp = orthocam.getViewProjMatrix();


	while (true) {

		updateInputs();
		interpreteInputs();
		if (ESC_KEY_STATE) return 0;
		if (Z_KEY_STATE) 
			camera.move({0,1,0});
		if (S_KEY_STATE) 
			camera.move({0,-1,0});


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

