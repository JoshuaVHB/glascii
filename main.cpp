#include <iostream>

#include "Utils/Math.h"
#include "Utils/FPSCounter.h"

#include "Renderer/Renderer.h"
#include "Renderer/Shapes.h"
#include "Renderer/Console.h"
#include "Renderer/Camera.h"


#include <algorithm>
#include <sstream>
#include <chrono>
#include <array>
#include <vector>
#include <thread>


int main() {

	//----------------------------------------- RUSH 1 -----------------------------------------//


	//----------------------------------------- RENDERING -----------------------------------------//

	constexpr int width = SCREEN_WIDTH;
	constexpr int height = SCREEN_WIDTH;

	bool COLORS_MODE = true;
	int framebufferSize = (COLORS_MODE) ? width * height * 8: width * height;

	Console::changeZoom(2,2);
	Console::setTerminalScreenResolution(width, height);

	char *buff = new char[framebufferSize];

	OrthographicCamera camera;
	FPSCounter fps;
	Cube cube;

	camera.setTarget({ 0,0,0 });
	camera.updateCam(0);

	std::vector<Vertex> v = cube.vertices;
	std::vector<Index> i = cube.indices;
	std::transform(i.begin(), i.end(), i.begin(), [](Index i) {return i - 1; });

	std::ios::sync_with_stdio(false); // increase output stream speed

	while (true) {


		// -- Update

		fps.step();
		camera.updateCam(fps.elapsed);
		Console::setTitle("FPS:" + std::to_string(fps.FPS));

		// -- Render

		clearScreenBuffer(buff, COLORS_MODE);
		clearDepth();
		
		renderMesh(buff, camera, v, i);

		preventResize(buff);
		renderBuffer(buff, framebufferSize);

	}
	
	system("pause");
	return 0;


}

