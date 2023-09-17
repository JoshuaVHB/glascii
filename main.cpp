#include "glascii.h"

int main() {


	constexpr int width = 200;
	constexpr int height = 200;


	Console::changeZoom(2,3);
	Console::setTerminalScreenResolution(width, height);


	Display disp(width, height);
	FPSCounter fps;
	OrthographicCamera camera;
	Cube cube;

	camera.setTarget({ 0,0,0 });
	camera.updateCam(0);

	std::vector<Vertex> v = cube.vertices;
	std::vector<Index> i = cube.indices;

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

		renderMesh(disp, camera, cube);

		preventResize(disp.pixelBuff);

		renderPixelBuffer(disp.pixelBuff, disp.framebufferSize);
		renderColorBuffer(disp.colorBuff, disp.framebufferSize);
		Sleep(1);


	}

	return 0;


}

