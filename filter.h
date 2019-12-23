#pragma once
#include "png_toolkit.h"
#include <vector>
#include <algorithm>
#include "parseconfig.h"

class Filter {
protected:
	int calculateHeightStart(int up, image_data& imgData);
	int calculateHeightFinish(int down, image_data& imgData);
	int calculateWidthStart(int left, image_data& imgData);
	int calculateWidthFinish(int right, image_data& imgData);
	int calculateLenghtString(image_data& imgData);
	void filterBW(int hStart, int hFinish, int startWidth, int finishWidth, int sizeString, image_data& imgData);
};

class Red : protected Filter {
public:
	Red(vector<int> coordinates, image_data& imgData);
	void red_filter(image_data& imgData);
private:
	int hStart;
	int hFinish;
	int startWidth;
	int finishWidth;
	int sizeString;
};

class Threshold : protected Filter {
public:
	Threshold(vector<int> coordinates, image_data& imgData);
	void threshold_filter(image_data& imgData);
private:
	int hStart;
	int hFinish;
	int startWidth;
	int finishWidth;
	int sizeString;
};

class Blur : protected Filter {
public:
	Blur(vector<int> coordinates, image_data& imgData);
	void blur_filter(image_data& imgData);
private:
	int hStart;
	int hFinish;
	int startWidth;
	int finishWidth;
	int sizeString;
};

class Edge : protected Filter {
public:
	Edge(vector<int> coordinates, image_data& imgData);
	void edge_filter(image_data& imgData);
private:
	int hStart;
	int hFinish;
	int startWidth;
	int finishWidth;
	int sizeString;
};

class Filters {
public:
	void doFilter(configData data, image_data& imgData);
};