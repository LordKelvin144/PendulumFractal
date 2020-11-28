#pragma once
#include <math.h>
#include <iostream>
#include <list>
#include <vector>

class FractalSection;

struct BorderArea {
	static const int size = 1024;
	static const int box_dimension_count = 9;

	// (1: 0), (2: 1), (4: 2), (8: 3) ...
	// Box count for different sizes
	int boxSize[box_dimension_count] = {};
};

namespace FractalData {
	enum Type {
		flipFractal,
		size
	};

	struct Section {
		float cornerX, cornerY;
		float spanX, spanY;
	};
	struct InitialCondition {
		float m1, m2;
		float l1, l2;
		float g;
	};
};

struct Position {
	int x;
	int y;
	char dir; // Direction
	Position(int t_x, int t_y, int direction): x(t_x), y(t_y), dir(direction) {}
};

class Fractal {
public:
	Fractal(int resolution, int shaderType, FractalData::InitialCondition ic);

	unsigned long long countBoxes(unsigned char boxType);
	BorderArea countEdges(unsigned char boxType);
	double getCompassDimension(int compass_length);

private:
	void loadCurrentSection(int xIn, int yIn, unsigned char* generate);
	void shiftCurretSections(int shiftX, int shiftY);

	unsigned char getCharArr(int x, int y);

	std::list<Position> getCompassBorderCrossing();
	BorderArea getBorderArea(int border_area_start_x, int border_area_start_y, std::list<Position> border_crossings);

	const int CURRENT_SECTIONS_SIDE = 5;
	FractalSection** currentSections;

	int resolution, resolutionSectionCount, shaderType;
	int currentCornerX = 0, currentCornerY = 0;
	FractalData::InitialCondition ic;

	friend struct CompassStruct;
};

struct CompassStruct {
public:
	int start_x;
	int start_y;
	int current_x;
	int current_y;
	int grid_width;
	int grid_height;
	int last_compass_point_x;
	int last_compass_point_y;
	char direction = POINTING_RIGHT;
	int length = 5;
	int compass_count = 0;
	char spin = CLOCKWISE_SPIN;
	enum {
		CLOCKWISE_SPIN = 2,
		CLOCKWISE_SPIN_DIAGONAL = 1,
		COUNTER_CLOCKWISE_SPIN = -2,
		COUNTER_CLOCKWISE_SPIN_DIAGONAL = -1
	};
	enum {
		POINTING_UP,
		POINTING_UP_RIGHT,
		POINTING_RIGHT,
		POINTING_BOTTOM_RIGHT,
		POINTING_BOTTOM,
		POINTING_BOTTOM_LEFT,
		POINTING_LEFT,
		POINTING_UP_LEFT,
	};
	void getNextPoint(Fractal* fractal) {
		direction = (direction + 4 + spin) % 8;
		for (int i = 0; i < 8; i++) {
			int pointing_x_pos;
			int pointing_y_pos;
			if (direction == POINTING_UP_LEFT ||
				direction == POINTING_LEFT ||
				direction == POINTING_BOTTOM_LEFT) {
				pointing_x_pos = current_x - 1;
			}
			else if (direction == POINTING_UP_RIGHT ||
				direction == POINTING_RIGHT ||
				direction == POINTING_BOTTOM_RIGHT) {
				pointing_x_pos = current_x + 1;
			}
			else {
				pointing_x_pos = current_x;
			}

			if (direction == POINTING_UP_LEFT ||
				direction == POINTING_UP ||
				direction == POINTING_UP_RIGHT) {
				pointing_y_pos = current_y - 1;
			}
			else if (direction == POINTING_BOTTOM_LEFT ||
				direction == POINTING_BOTTOM ||
				direction == POINTING_BOTTOM_RIGHT) {
				pointing_y_pos = current_y + 1;
			}
			else {
				pointing_y_pos = current_y;
			}

			if (pointing_x_pos >= 0 && pointing_x_pos < grid_width &&
				pointing_y_pos >= 0 && pointing_y_pos < grid_height &&
				fractal->getCharArr(pointing_x_pos, pointing_y_pos) == 0) {
				current_x = pointing_x_pos;
				current_y = pointing_y_pos;
				return;
			}

			direction = (direction + spin + 8) % 8;
		}
	}
};

template<class T>
class TwoDArray {
public:
	TwoDArray(int width, int height): m_width(width), m_height(height) {
		//m_array.resize(m_width * m_height);
		//m_array.clear();
	}
	T* getRef(int x, int y) {
		return &m_array[x + m_width * y];
	}
protected:
	T m_array[BorderArea::size * BorderArea::size] = {};
	int m_width;
	int m_height;
};



