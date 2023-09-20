#pragma once


#include "../Utils/Math.h"
#include "../Utils/Vertex.h"

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
		if (y * width + x < size)
			return buff[y * width + x];
		return 0.0f;
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