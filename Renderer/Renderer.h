#pragma once

#include <Windows.h>
#include <iostream>
#include <algorithm>

#include <chrono>
#include <thread>
#include <tuple>
#include <vector>
#include <array>

#include "../Utils/Math.h"
#include "DepthBuffer.h"
#include "Camera.h"


#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 200

using pixelBuffer = char*;
using colorBuffer = WORD*;
static auto winHand = GetStdHandle(STD_OUTPUT_HANDLE);

struct Display {

	depthBuffer depthBuff;
	pixelBuffer pixelBuff;
	colorBuffer colorBuff;

	size_t framebufferSize;
	int w;
	int h;

	Display(int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT) 
		: depthBuff(width, height), pixelBuff(new char[width*height]), colorBuff(new WORD[width*height])

	{
		framebufferSize = width * height;
		w = width;
		h = height;
		std::fill(colorBuff, colorBuff + framebufferSize, FOREGROUND_INTENSITY);
	}

	~Display() { delete[] pixelBuff; delete[] colorBuff; }
} ;

static DWORD written;
static auto hand = GetStdHandle(STD_OUTPUT_HANDLE);
static depthBuffer dp(SCREEN_WIDTH, SCREEN_HEIGHT);

enum Colors : WORD {
	Light = FOREGROUND_INTENSITY,
	Red = FOREGROUND_RED,
	LightRed = Red | Light,
	Green = FOREGROUND_GREEN,
	LightGreen = Green | Light,
	Blue = FOREGROUND_BLUE,
	LightBlue = Blue | Light,
	Yellow = Red | Green,
	LightYellow = Yellow | Light,
	Cyan = Green | Blue,
	LightCyan = Cyan | Light ,
	Magenta = Red | Blue, 
	LightMagenta = Magenta | Light, 
	White = Blue | Red | Green
};

static char table[11] = {
	'@', '#', 'S', '%', '?', '*', '+', ';', ':', ',', '.'
};

/*
* LOST BUT NEVER FORGOTTEN
constexpr int64_t RED_CLEAR_CHAR = 0x2e6d31333b305b1b;
constexpr int64_t WHITE_CLEAR_CHAR = 0x2e6d39333b305b1b;
*/


void renderPixelBuffer(pixelBuffer buff, size_t bufferSize) {

	WriteConsoleA(hand, buff, bufferSize, &written, NULL);
}

void renderColorBuffer(colorBuffer cbuff, size_t bufferSize)
{
	WriteConsoleOutputAttribute(hand, cbuff, bufferSize, { 0,0 }, &written);
}

// TODO change this function
void preventResize(pixelBuffer buff, int bufferWidth=SCREEN_WIDTH, int bufferHeight=SCREEN_HEIGHT) {


	auto op = [&](int i) { buff[(i + 1) * (SCREEN_WIDTH) - 1] = '\n'; } ;

	// Compile time unrolling
	[&]<std::size_t...p>(std::index_sequence<p...>) 
	{
		(op(p), ...);
	} (std::make_index_sequence<SCREEN_HEIGHT - 1>{});

}


/* Resets the screenbuffer and puts the cursor position at 0,0 */
void clearScreenBuffer(pixelBuffer buffer, colorBuffer cbuff, size_t buffSize) 
{
	std::fill(buffer, (buffer + buffSize), ' ') ;
	std::fill(cbuff, (cbuff + buffSize), FOREGROUND_INTENSITY);
	SetConsoleCursorPosition(winHand, { 0,0 });
}

void clearDepth(depthBuffer& dBuff) {
	dp.clear();
}

void setPixelChar(char* buffer,int width, int x, int y, char c) 
{
	if (static_cast<unsigned>(y * width + x) > SCREEN_HEIGHT * SCREEN_HEIGHT) return;
	buffer[y * width + x] = c;
}

void setPixelCharWithDepth(pixelBuffer buffer, depthBuffer& dBuff, int width, int height, int x, int y, char c, float depth)
{

	int linearIndex = y * width + x;
	if (static_cast<unsigned long>(linearIndex) > height*width) return;
	if (depth < dBuff.getAt(x, y)) {
		dBuff.setAt(x, y, depth);
		buffer[linearIndex] = c;
	}
}


void setPixelWithColor(
	const Display& display,
	int x, int y, char c,
	float depth,
	Colors color) {

	int linearIndex = y * display.w + x;
	if (static_cast<unsigned long>(linearIndex) > display.framebufferSize) return;
	if (depth < display.depthBuff.getAt(x, y)) {
		dp.setAt(x, y, depth);
		display.pixelBuff[linearIndex] = c;
		display.colorBuff[linearIndex] = (WORD)color;
	}

}

Math::Vec3<float> getWeights(Math::uVec2 p, Math::uVec2 v1, Math::uVec2 v2, Math::uVec2 v3) {

	float factor = 1.f/static_cast<float>((v2.v - v3.v) * (v1.u - v3.u) + (v3.u - v2.u) * (v1.v - v3.v));
	float W_1 = (v2.v - v3.v) * (p.u - v3.u) + (v3.u - v2.u) * (p.v - v3.v);
	float W_2 = (v3.v - v1.v) * (p.u - v3.u) + (v1.u - v3.u) * (p.v - v3.v);
	W_1 *= factor;
	W_2 *= factor;
	float W_3 = 1.f - W_1 - W_2;
	return { W_1, W_2, W_3 };

}

// bresenhams
void drawLine(char* buffer, Math::uVec2 a, Math::uVec2 b, int width) 
{
	bool isHorizontal = abs(b.u - a.u) > abs(b.v - a.v);

	if (isHorizontal) {

		bool swap = (b.u < a.u);
		if (swap) std::swap(a, b);

		for (int x = a.u; x < b.u; x++) 
		{
			int y = a.v + ((x - a.u)*(b.v - a.v)) / (b.u - a.u);
			setPixelChar(buffer, width, x, y, '@');

		}

	}
	else {
		bool swap = (b.v < a.v);
		if (swap) std::swap(a, b);

		for (int y = a.v; y < b.v; y++)
		{
			int x = a.u + ((y - a.v) * (b.u - a.u)) / (b.v - a.v);
			setPixelChar(buffer, width, x, y, '@');

		}
	}
}

void drawWireframeTriangle(char* buffer, int width, Math::uVec2 a, Math::uVec2 b, Math::uVec2 c) {

	drawLine(buffer, a, b, width);
	drawLine(buffer, b, c, width);
	drawLine(buffer, c, a, width);

}


/*
Thanks bisqwit
https://www.youtube.com/watch?v=PahbNFypubE& 
*/
// TODO CHANGE ALL OF THIS ! WHY IS THE SUN IN THIS ?? 
void drawFilledTriangle(Display& disp,
	Math::uVec2 a, Math::uVec2 b, Math::uVec2 c,
	std::array<Math::Vec3<float>, 3> normals,
	Math::Vec3<float> depths = { 0,0,0 }, bool drawOutline = false
)

{

	//COLOR randomColor = COLOR(rand() % (char)COLOR::Default);
	Colors randomColor = Colors(rand() % (int)Colors::White);
	Math::Vec3<float> surfaceNormal;
	Math::Vec3<float> sunPos = { 7,9,5 };
	// Sort the points
	using slope_data = std::pair<float/*start*/, float/*step*/>;
	auto MakeSlope = [](Math::uVec2 from, Math::uVec2 to, int numstep) -> slope_data {

		int begin = from.u; int end = to.u;
		float inv_step = 1.f / numstep;
		return { begin, (end - begin) * inv_step };
	};

	auto DrawScanline = [&](char ch, int y, slope_data& left, slope_data& right)
	{

		int x = left.first;
		int endx = right.first;

		for (; x < endx; ++x) {
			Math::Vec3<float> w = getWeights({ x,y }, a, b, c);
			surfaceNormal = (normals[0]*w.x)+ (normals[1] * w.y)+ (normals[2] * w.z);
			float d = Math::dot(depths, w);

			// -- Change this for color
			setPixelWithColor(disp, x, y, ch, d, Colors::Red);
		}
		left.first += left.second;
		right.first += right.second;

	};

	auto [x0, y0, x1, y1, x2, y2] = std::tuple_cat(
		std::tuple(a.u, a.v),
		std::tuple(b.u, b.v),
		std::tuple(c.u, c.v));


	if (std::tie(y1, x1) < std::tie(y0, x0)) {
		std::swap(x0, x1); std::swap(y0, y1);
		std::swap(a, b); std::swap(depths.x, depths.y);
		std::swap(normals[0], normals[1]);
	}


	if (std::tie(y2, x2) < std::tie(y0, x0)) {
		std::swap(x0, x2); std::swap(y0, y2); 
		std::swap(a, c);std::swap(depths.x, depths.z);
		std::swap(normals[0], normals[2]);
	}
	if (std::tie(y2, x2) < std::tie(y1, x1)) {
		std::swap(x1, x2); std::swap(y1, y2);
	std::swap(b, c);std::swap(depths.y, depths.z);
	std::swap(normals[1], normals[2]);
	}

	// Refuse to draw arealess triangles.
	if (int(y0) == int(y2)) return;

	// Determine whether the short side is on the left or on the right.
	bool shortside = (y1 - y0) * (x2 - x0) < (x1 - x0) * (y2 - y0); // false=left side, true=right side

	// Create two slopes: p0-p1 (short) and p0-p2 (long).
	// One of these is on the left side and one is on the right side.
	// At y = y1, the p0-p1 slope will be replaced with p1-p2 slope.
	// The type of these variables is whatever MakeSlope() returns.
	slope_data sides[2];

	// At this point, y2-y0 cannot be zero.
	sides[!shortside] = MakeSlope(a, c, y2 - y0); // Slope for long side

	// The main rasterizing loop. Note that this is intentionally designed such that
	// there's only one place where DrawScanline() is invoked. This will minimize the
	// chances that the compiler fails to inline the functor.
	surfaceNormal = (normals[0] + normals[1] + normals[2]).normalize();

	//randomColor = (Math::dot(surfaceNormal, { 0,1,0 }) > 0.75f) ? COLOR::green : COLOR::white;

	Math::Vec3<float> sunDir = (sunPos).normalize();
	float sunLight = max(0.f, Math::dot(sunDir, surfaceNormal));
	char character = table[9-int(sunLight * 9)];

	if (int(y0) < int(y1))
	{
		// Calculate the first slope for short side. The number of lines cannot be zero.
		sides[shortside] = MakeSlope(a, b, y1 - y0);
		for (int y = y0; y < int(y1); ++y)
		{
			DrawScanline(character, y, sides[0], sides[1]);
		}
	}
	if (int(y1) < int(y2))
	{
		// Calculate the second slope for short side. The number of lines cannot be zero.
		sides[shortside] = MakeSlope(b, c, y2 - y1);
		for (int y = y1; y < int(y2); ++y)
		{
			
			DrawScanline(character, y, sides[0], sides[1]);
		}
	}
	if (drawOutline) 
	{
		drawLine(disp.pixelBuff, b, c, disp.w);
		drawLine(disp.pixelBuff, a, b, disp.w);
		drawLine(disp.pixelBuff, c, a, disp.w);
	}
	
}


enum class RENDER_MODE { WIREFRAME, FILLED };

// todo add perspective
// todo clean
void renderMesh(Display& disp, const OrthographicCamera& camera,
	const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
	RENDER_MODE mode= RENDER_MODE::FILLED)
{

	auto camForward = camera.getForward();
	auto camPos = camera.getPosition();

	for (int id = 0; id < indices.size(); id += 3) {

		Index i1 = indices[id];
		Index i2 = indices[id + 1];
		Index i3 = indices[id + 2];

		Vertex v1 = vertices[i1];
		Vertex v2 = vertices[i2];
		Vertex v3 = vertices[i3];

		Math::uVec2 p1 = camera.toScreenCoords(camera.toCamCoords(v1.position));
		Math::uVec2 p2 = camera.toScreenCoords(camera.toCamCoords(v2.position));
		Math::uVec2 p3 = camera.toScreenCoords(camera.toCamCoords(v3.position));

		Math::Vec3<float> depths =
		{
			dot(v1.position, camForward) - dot(camPos, camForward),
			dot(v2.position, camForward) - dot(camPos, camForward),
			dot(v3.position, camForward) - dot(camPos, camForward),
		};
		std::array<Math::Vec3<float>, 3> normals = { v1.normal, v2.normal, v3.normal };

		(mode == RENDER_MODE::FILLED) ? 
			drawFilledTriangle(disp, p1, p2, p3, normals, depths):
			drawWireframeTriangle(disp.pixelBuff, disp.w, p1, p2, p3);
	}

}