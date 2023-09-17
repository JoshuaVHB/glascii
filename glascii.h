#ifndef Math_IMPL_H
#define Math_IMPL_H

#include <iostream>
#include <assert.h>
#include <cmath>

namespace Math {



	template <typename T=double>
	struct Vec2 
	{
		T u;
		T v;

		Vec2() : u(0.f), v(0.f) {}
		Vec2(T a, T b) : u(a), v(b) {}

		Vec2 operator*(const Vec2& rhs)
		{
			return Vec2{ u * rhs.u, v * rhs.v };
		}

		Vec2 operator+(const Vec2& rhs)
		{
			return Vec2{ u + rhs.u, v + rhs.v };
		}
		Vec2 operator-(const Vec2& rhs)
		{
			return Vec2{ u - rhs.u, v - rhs.v };
		}
		Vec2 operator/(const Vec2& rhs)
		{
			return Vec2{ u / rhs.u, v / rhs.v };
		}

		Vec2 normalize() 
		{
			assert(u && v);
			float n = u * u + v * v;
			u = u / n;
			v = v / n;
			return { u,v };
		}


	};

	template <typename T=double>
	struct Vec3
	{
		T x;
		T y;
		T z;

		Vec3() : x(0.f), y(0.f), z(0.f) {}
		Vec3(T a, T b, T c): x(a), y(b), z(c) {}
		Vec3(const Vec3<T>& other) { x = other.x; y = other.y; z = other.z; }

		Vec3 operator*(const Vec3& rhs) 
		{
			return Vec3{ x * rhs.x, y * rhs.y, z * rhs.z };
		}

		template<typename C>
		Vec3 operator*(C v)
		{
			return Vec3{ x * v, y * v, z *v };
		}

		Vec3 operator+(const Vec3& rhs)
		{
			return Vec3{ x + rhs.x, y + rhs.y, z + rhs.z };
		}

		Vec3 operator-(const Vec3& rhs)
		{
			return Vec3{ x - rhs.x, y - rhs.y, z - rhs.z };
		}

		Vec3 operator/(const Vec3& rhs)
		{
			return Vec3{ x / rhs.x, y / rhs.y, z / rhs.z };
		}

		void operator+=(const Vec3& rhs) 
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
		}

		float length() { return sqrt(x * x + y * y + z * z); };

		Vec3 normalize()
		{
			float n = length();
			assert(n!=0);
			x = static_cast<double>(x) / n;
			y = static_cast<double>(y) / n;
			z = static_cast<double>(z) / n;

			return { x,y,z };
		}





	};

	typedef Vec3<int> uVec3;
	typedef Vec2<int> uVec2;

	template<typename T>
	Math::Vec3<T> cross(const Math::Vec3<T>& lhs, const Math::Vec3<T>& rhs) {
		return {

			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x

		};
	}

	template<typename T>
	T dot(const Math::Vec3<T>& lhs, const Math::Vec3<T>& rhs) {
			return {
				lhs.x * rhs.x +
				lhs.y * rhs.y +
				lhs.z * rhs.z
		};
	}

	template<typename T>
	T dot(const Math::Vec2<T>& lhs, const Math::Vec2<T>& rhs) {
		return {
				lhs.u * rhs.u +
				lhs.v * rhs.v 
		};
	}

}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Math::Vec3<T>& vec)
{
	os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Math::Vec2<T>& vec)
{
	os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
	return os;
}

// -- Others

namespace Math {


	inline Math::Vec3<float> rot(Math::Vec3<float> vec, float angle)
	{

		float    fSinAngle = std::sin(angle);
		float    fCosAngle = std::cos(angle);

		return Math::Vec3<float>{vec.x, vec.y* fCosAngle + vec.z * fSinAngle, vec.y * -fSinAngle + vec.z * fCosAngle};
	}

	inline Math::Vec3<float> getWeights(Math::uVec2 p, Math::uVec2 v1, Math::uVec2 v2, Math::uVec2 v3) {

		float factor = 1.f / static_cast<float>((v2.v - v3.v) * (v1.u - v3.u) + (v3.u - v2.u) * (v1.v - v3.v));
		float W_1 = (v2.v - v3.v) * (p.u - v3.u) + (v3.u - v2.u) * (p.v - v3.v);
		float W_2 = (v3.v - v1.v) * (p.u - v3.u) + (v1.u - v3.u) * (p.v - v3.v);
		W_1 *= factor;
		W_2 *= factor;
		float W_3 = 1.f - W_1 - W_2;
		return { W_1, W_2, W_3 };

	}
}
#endif
#ifndef Vertex_IMPL_H
#define Vertex_IMPL_H

#include <vector>

// TODO make Vertex a template parameter for undefined layouts
struct Vertex 
{
	Math::Vec3<float> position;
	Math::Vec3<float> normal{1,0,0};


};

typedef unsigned int Index;

struct Mesh {

	std::vector<Vertex> vertices;
	std::vector<Index> indices;

};
#endif
#ifndef Fpscounter_IMPL_H
#define Fpscounter_IMPL_H

#include <chrono>
#include <thread>

inline long long nanoTime()
{
	using namespace std::chrono;
	return duration_cast<nanoseconds>(high_resolution_clock::now().time_since_epoch()).count();
}


struct FPSCounter {

	double start = nanoTime();
	double totalTime = 0;
	double elapsed{}, curr{};
	unsigned int FPS = 0;
	int framecount = 0;
	float deltaTime = 0;
	
	
	void step() {

		curr = nanoTime();
		elapsed = (curr - start) / 1E9;
		start = curr;
		FPS = 1 / elapsed;
		framecount++;
		totalTime += elapsed;
	}
	
	float computeAvgFPS() {
		return (framecount / totalTime);
	}

};

#endif
#ifndef Console_IMPL_H
#define Console_IMPL_H

#include <Windows.h>
#include <string>




namespace Console {

	struct WIN_SIZE {
		int w = 250;
		int h = 250;
	}static s_WindowSize;

	std::pair<int, int> getTerminalSize() {

		CONSOLE_SCREEN_BUFFER_INFOEX consolesize;

		consolesize.cbSize = sizeof(consolesize);

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		GetConsoleScreenBufferInfoEx(hConsole, &consolesize);


		int columns = consolesize.srWindow.Right - consolesize.srWindow.Left + 1;
		int rows = consolesize.srWindow.Bottom - consolesize.srWindow.Top + 1;

		return { columns, rows };

	}

	void changeZoom(int fontX, int fontY) {

		HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
		HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_FONT_INFOEX orig, current;

		current.cbSize = sizeof(CONSOLE_FONT_INFOEX);

		COORD FontSize;

		FontSize.X = fontX;
		FontSize.Y = fontY;

		current.dwFontSize = FontSize;
		current.FontWeight = 600;
		current.FontFamily = FF_ROMAN;


		if (GetCurrentConsoleFontEx(hStdin, FALSE, &orig))
			std::cout << "Got\n";
		else
			std::cout << GetLastError();

		if (SetCurrentConsoleFontEx(hStdout, FALSE, &current))
			std::cout << "\nSet";
		else
			std::cout << std::endl << GetLastError();
	}

	void setTerminalScreenResolution(int width, int height)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFOEX consolesize;
		consolesize.cbSize = sizeof(consolesize);

		GetConsoleScreenBufferInfoEx(hConsole, &consolesize);
		COORD c;
		c.X = width;
		c.Y = height;
		consolesize.dwSize = c;


		s_WindowSize.h = height;
		s_WindowSize.w = width;

		consolesize.srWindow.Left = 0;
		consolesize.srWindow.Right = width;
		consolesize.srWindow.Top = 0;
		consolesize.srWindow.Bottom = height;


		SetConsoleScreenBufferInfoEx(hConsole, &consolesize);
		SetConsoleScreenBufferSize(hConsole, c);
		SetConsoleActiveScreenBuffer(hConsole);

		CONSOLE_CURSOR_INFO     cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = FALSE; // set the cursor visibility
		SetConsoleCursorInfo(hConsole, &cursorInfo);

		system("cls");

		std::pair<int, int> winSize = getTerminalSize();



	}

	void setFullScreen() {

		system("mode con COLS=700");
		ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
		SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);


	}

	void setTitle(const std::string& str) {
		std::basic_stringstream<wchar_t> ss;
		ss << str.c_str();
		SetConsoleTitleW(ss.str().c_str());
	}

}
#endif
#ifndef Camera_IMPL_H
#define Camera_IMPL_H




class OrthographicCamera {

	Math::Vec3<float> UP{ 0.f, 1.f, 0.f };

public:

	void lookAt(const Math::Vec3<float>& target) {
		m_forward = Math::Vec3<float>({ target.x - m_position.x, target.y - m_position.y, target.z - m_position.z }).normalize();
		m_left = cross(m_forward, UP).normalize();
		m_up = cross(m_forward, m_left).normalize();
	}

	void lookAtTarget() {
		m_forward = Math::Vec3<float>({ m_target.x - m_position.x, m_target.y - m_position.y, m_target.z - m_position.z }).normalize();
		m_left = cross(m_forward, UP).normalize();
		m_up = cross(m_forward, m_left).normalize();
	}

	Math::Vec2<float> toCamCoords(Math::Vec3<float> p) const {
		return {
		  dot(p, m_left) - dot(m_position, m_left),
		  dot(p, m_up) - dot(m_position, m_up),
		};
	};

	Math::uVec2 toScreenCoords(Math::Vec2<float> p) const  {
		Math::uVec2 win = { Console::s_WindowSize.w, Console::s_WindowSize.h };
		return {
		  static_cast<int>(p.u * scaleFactor + win.u * .5f),
		  static_cast<int>(p.v * scaleFactor + win.v * .5f)
		};
	};

	void updateCam(float deltaTime ,bool animateCamera=true) {

		if (animateCamera) {
			t += deltaTime;
			float l = 5;
			m_position.x = m_target.x + std::cos(t) * l;
			m_position.y = 3;
			m_position.z = m_target.z + std::sin(t) * l;
		}
		// orthographic projection
		lookAt(m_target);
	};

	void move(const Math::Vec3<float>& delta) {
		m_position += delta;
	}

	void setTarget(Math::Vec3<float> target) { m_target = target; }

	Math::Vec3<float> getPosition() const { return m_position; }
	Math::Vec3<float> getForward() const { return m_forward; }

	void setScale(float f) { scaleFactor = f; }
	float scaleFactor = 100;

private:
	float t = 0;
	Math::Vec3<float> m_target{ 0, 0, 0 };
	Math::Vec3<float> m_left{ 0.f, 1.f, 0.f };
	Math::Vec3<float> m_up{ 0.f, 1.f, 0.f };
	Math::Vec3<float> m_position{ 0.f, 1.f, 0.f };
	Math::Vec3<float> m_forward{ 0.f, 1.f, 0.f };

};


#endif
#ifndef Depthbuffer_IMPL_H
#define Depthbuffer_IMPL_H



#include <numeric>
#include <assert.h>

class depthBuffer {

private:
	int width, height;
	int size;
	float* buff;

	static constexpr float MAX_DEPTH = 10000.F;
public:

	void setAt(int x, int y, float v) {
		assert(y * width + x < size);
		buff[y * width + x] = v;
	}

	float getAt(int x, int y) const {
		assert(y * width + x < size);
		return buff[y * width + x];
	}

	bool depthTest(int x, int y, float d) {

		assert(y * width + x < size);

		if (buff[y * width + x] > d) {
			buff[y * width + x] = d;
			return true;
		}
		return false;
	}

	void clear() {

		std::fill(buff, buff + (width * height), MAX_DEPTH);

	}

	depthBuffer(int w, int h) : width(w), height(h) { 
		size = width * height;
		buff = new float[size];
		clear();
	}
	~depthBuffer() { delete[] buff; }

};
#endif
#ifndef Inputs_IMPL_H
#define Inputs_IMPL_H

#include <Windows.h>


static bool Z_KEY_STATE = false;
static bool Q_KEY_STATE = false;
static bool S_KEY_STATE = false;
static bool D_KEY_STATE = false;
static bool ESC_KEY_STATE = false;

void updateInputs() {

	Z_KEY_STATE=GetKeyState(0x5A) & 0x8000;
	Q_KEY_STATE=GetKeyState(0x51) & 0x8000;
	S_KEY_STATE=GetKeyState(0x44) & 0x8000;
	D_KEY_STATE=GetKeyState(0x53) & 0x8000;
	ESC_KEY_STATE = GetKeyState(VK_ESCAPE) & 0x8000;

}

void interpreteInputs() {

	if (Z_KEY_STATE) {}
	if (Q_KEY_STATE) {}
	if (S_KEY_STATE) {}
	if (D_KEY_STATE) {}

}
#endif
#ifndef Shapes_IMPL_H
#define Shapes_IMPL_H


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
#endif
#ifndef Renderer_IMPL_H
#define Renderer_IMPL_H



#include <Windows.h>
#include <iostream>
#include <algorithm>

#include <chrono>
#include <thread>
#include <tuple>
#include <vector>
#include <array>



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
	dBuff.clear();
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
	Display& display,
	int x, int y, char c,
	float depth,
	Colors color) {

	int linearIndex = y * display.w + x;
	if (static_cast<unsigned long>(linearIndex) > display.framebufferSize) return;
	if (depth < display.depthBuff.getAt(x, y)) {
		display.depthBuff.setAt(x, y, depth);
		display.pixelBuff[linearIndex] = c;
		display.colorBuff[linearIndex] = (WORD)color;
	}

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
	randomColor = Colors::Green;
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
			Math::Vec3<float> w = Math::getWeights({ x,y }, a, b, c);
			surfaceNormal = (normals[0]*w.x)+ (normals[1] * w.y)+ (normals[2] * w.z);
			float d = Math::dot(depths, w);

			// -- Change this for color
			setPixelWithColor(disp, x, y, ch, d, randomColor);
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

void renderMesh(Display& disp, const OrthographicCamera& camera,
	const Mesh& mesh, RENDER_MODE mode = RENDER_MODE::FILLED)
{
	renderMesh(disp, camera, mesh.vertices, mesh.indices, mode);
}
#endif
