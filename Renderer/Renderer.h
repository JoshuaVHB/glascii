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

constexpr int64_t RED_CLEAR_CHAR = 0x2e6d31333b305b1b;
constexpr int64_t WHITE_CLEAR_CHAR = 0x2e6d39333b305b1b;


void renderBuffer(char* buffer, size_t size) {

	WriteConsoleA
	(hand, buffer, size, &written, NULL);
}

void renderColorBuffer(WORD* cbuff, size_t size) 
{
	WriteConsoleOutputAttribute(hand, cbuff, size, { 0,0 }, &written);
}

void preventResize(char* buffer, bool hasColors=false) {


	auto op = (hasColors) ?
		[](int i, char* buffer) { buffer[(i + 1) * (SCREEN_WIDTH * 8) - 1] = '\n'; } :
		[](int i, char* buffer) { buffer[(i + 1) * (SCREEN_WIDTH) - 1] = '\n'; } ;

	// Compile time unrolling
	[&]<std::size_t...p>(std::index_sequence<p...>) 
	{
		(op(p, buffer), ...);
	} (std::make_index_sequence<SCREEN_HEIGHT - 1>{});

}

void clear(char* buffer) {

}

/* Resets the screenbuffer and puts the cursor position at 0,0 */
void clearScreenBuffer(char* buffer, WORD* cbuff, bool hasColor=false) 
{
	std::fill(buffer, (buffer + SCREEN_HEIGHT * SCREEN_WIDTH), ' ') ;
	std::fill(cbuff, cbuff + SCREEN_HEIGHT * SCREEN_WIDTH, FOREGROUND_INTENSITY);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
}

void clearDepth() {
	dp.clear();
}

void setPixelChar(char* buffer,int width, int x, int y, char c) 
{
	if (static_cast<unsigned>(y * width + x) > SCREEN_HEIGHT * SCREEN_HEIGHT) return;
	buffer[y * width + x] = c;
}

void setPixelCharWithDepth(char* buffer, int width, int x, int y, char c, float depth)
{
	if (static_cast<unsigned long>(y * width + x) > SCREEN_WIDTH * SCREEN_HEIGHT) return;
	if (depth < dp.getAt(x, y)) {
		dp.setAt(x, y, depth);
		buffer[7+(y * width + x)] = c;
	}
}

void setPixelCharWithDepth_8(char* buffer, int width, int x, int y, char c, float depth)
{
	if (static_cast<unsigned long>(y * width + x) > SCREEN_WIDTH * SCREEN_HEIGHT) return;
	if (depth < dp.getAt(x, y)) {
		dp.setAt(x, y, depth);
		buffer[7 + (y * width + x) * 8] = c;
	}
}

void setPixelWithColor(char* buffer,  int width, int x, int y, char c, float depth, Colors color) {

	if (static_cast<unsigned long>(y * width + x) > SCREEN_WIDTH * SCREEN_HEIGHT * 8) return;
	if (depth < dp.getAt(x, y)) {
		dp.setAt(x, y, depth);

		buffer[(y * width + x) * 8] = '\033';
		buffer[1 + (y * width + x) * 8] = '[';
		buffer[2 + (y * width + x) * 8] = '0';
		buffer[3 + (y * width + x) * 8] = ';';
		buffer[4 + (y * width + x) * 8] = '3';
		buffer[5 + (y * width + x) * 8] = (char)(color) + '0';
		buffer[6 + (y * width + x) * 8] = 'm';
		buffer[7 + (y * width + x) * 8] = c;
	}

}
void setPixelWithColor2(char* buffer,WORD* cbuff, int width, int x, int y, char c, float depth, Colors color) {

	if (static_cast<unsigned long>(y * width + x) > SCREEN_WIDTH * SCREEN_HEIGHT) return;
	if (depth < dp.getAt(x, y)) {
		dp.setAt(x, y, depth);

		buffer[(y * width + x)] = c;
		cbuff[(y * width + x)] = (WORD)color;
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
void drawFilledTriangle(char* buffer, WORD* cbuff, int width,
	Math::uVec2 a, Math::uVec2 b, Math::uVec2 c,
	std::array<Math::Vec3<float>, 3> normals,
	Math::Vec3<float> depths = { 0,0,0 }, bool hasColor=true,bool drawOutline = false
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
			(hasColor) ?
				setPixelWithColor2(buffer, cbuff ,width, x, y, ch, d, randomColor) :
				setPixelCharWithDepth(buffer, width, x, y, ch, d);
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
		drawLine(buffer, a, b, width);
		drawLine(buffer, b, c, width);
		drawLine(buffer, c, a, width);	
	}
	
}


enum class RENDER_MODE { WIREFRAME, FILLED };

// todo add perspective
// todo clean
void renderMesh(char* buffer, WORD* cbuff, const OrthographicCamera& camera,
	const std::vector<Vertex>& vertices, const std::vector<Index>& indices, bool hasColors = false,
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
			drawFilledTriangle(buffer, cbuff ,SCREEN_WIDTH, p1, p2, p3, normals, depths, hasColors ):
			drawWireframeTriangle(buffer, SCREEN_WIDTH, p1, p2, p3);
	}

}