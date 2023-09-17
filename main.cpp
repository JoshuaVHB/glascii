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


	constexpr int width = SCREEN_WIDTH;
	constexpr int height = SCREEN_WIDTH;

	bool COLORS_MODE = false;
	int framebufferSize = (COLORS_MODE) ? width * height * 8: width * height;

	Console::changeZoom(2,3);
	Console::setTerminalScreenResolution(width, height);

	char *buff = new char[framebufferSize];
	WORD* cbuff = new WORD[framebufferSize];

	Display disp(width, height);

	std::fill(cbuff, cbuff + framebufferSize, FOREGROUND_INTENSITY);

	FPSCounter fps;
	OrthographicCamera camera;
	Cube cube;

	camera.setTarget({ 0,0,0 });
	camera.updateCam(0);

	std::vector<Vertex> v = cube.vertices;
	std::vector<Index> i = cube.indices;
	std::transform(i.begin(), i.end(), i.begin(), [](Index i) {return i - 1; });

	unsigned int frames = 0;
	auto firstTime = nanoTime();
	auto lastSec = firstTime;

	while (true) {

		updateInputs();
		interpreteInputs();
		if (ESC_KEY_STATE) return 0;

		fps.step();
		Console::setTitle("FPS:" + std::to_string(fps.FPS));
		camera.updateCam(fps.elapsed, true);


		clearScreenBuffer(disp.pixelBuff, disp.colorBuff, disp.framebufferSize);
		clearDepth(disp.depthBuff);

		renderMesh(disp, camera, v, i);

		//preventResize(disp.pixelBuff);


		renderPixelBuffer(disp.pixelBuff, framebufferSize);
		renderColorBuffer(disp.colorBuff, framebufferSize);
		Sleep(1);


	}

	return 0;


}

