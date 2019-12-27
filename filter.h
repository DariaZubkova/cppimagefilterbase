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
	//int calculateLenghtString(image_data& imgData);
	void filterBW(vector<int> coordinates, image_data& imgData);
};

class Red : protected Filter {
public:
	Red(vector<int> coordinates, image_data& imgData);
	void red_filter(image_data& imgData);
private:
	vector<int> posPicture;
};

class Threshold : protected Filter {
public:
	Threshold(vector<int> coordinates, image_data& imgData);
	void threshold_filter(image_data& imgData);
	int middleFilter(image_data& imgData, vector<int>matrixCoordinates);
private:
	vector<int> posPicture;
};

class Blur : protected Filter {
public:
	Blur(vector<int> coordinates, image_data& imgData);
	void blur_filter(image_data& imgData);
	vector<int> filterExtraBlur(image_data& imgData, vector<int>matrixCoordinates);
private:
	vector<int> posPicture;
};

class Edge : protected Filter {
public:
	Edge(vector<int> coordinates, image_data& imgData);
	void edge_filter(image_data& imgData);
	int filterExtraEdge(image_data& imgData, vector<int>matrixCoordinates);
private:
	vector<int> posPicture;
};

class Filters {
public:
	void doFilter(configData data, image_data& imgData);
};