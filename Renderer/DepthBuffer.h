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

public:

	void setAt(int x, int y, float v) {
		assert(y * width + x < size);
		buff[y * width + x] = v;
	}

	float getAt(int x, int y) {
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

		std::fill(buff, buff + (width * height), 1000.f);

	}

	depthBuffer(int w, int h) : width(w), height(h) { 
		size = width * height;
		buff = new float[size];
		std::fill(buff, buff + (width * height), 1000.f);
	}
	~depthBuffer() { delete[] buff; }

};