#include "filter.h"
#include <vector>
#include <algorithm>

int Filter::calculateHeightStart(int up, image_data& imgData) {
	int hStart;
	if (up == 0)
		hStart = 0;
	else
		hStart = imgData.h / up;
	return hStart;
}
int Filter::calculateHeightFinish(int down, image_data& imgData) {
	int hFinish;
	if (down == 0)
		hFinish = 0;
	else
		hFinish = imgData.h / down;
	return hFinish;
}
int Filter::calculateWidthStart(int left, image_data& imgData) {
	int wStart;
	if (left == 0)
		wStart = 0;
	else
		wStart = imgData.w / left;
	int startWidth = imgData.compPerPixel * wStart;
	return startWidth;
}
int Filter::calculateWidthFinish(int right, image_data& imgData) {
	int wFinish;
	if (right == 0)
		wFinish = 0;
	else
		wFinish = imgData.w / right;
	int finishWidth = imgData.compPerPixel * wFinish;
	return finishWidth;
}
int Filter::calculateLenghtString(image_data& imgData) {
	int sizeString = imgData.compPerPixel * imgData.w;
	return sizeString;
}
void Filter::filterBW(int hStart, int hFinish, int startWidth, int finishWidth, int sizeString, image_data& imgData) {
	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			int x = 0.3 * imgData.pixels[i * sizeString + j] + 0.6 * imgData.pixels[i * sizeString + j + 1] + 0.1 * imgData.pixels[i * sizeString + j + 2];
			for (int k = 0; k < imgData.compPerPixel; k++) {
				imgData.pixels[k + i * sizeString + j] = (unsigned char)x;
			}
		}
	}
}

Red::Red(vector<int> coordinates, image_data& imgData) {
	hStart = calculateHeightStart(coordinates[0], imgData);
	startWidth = calculateWidthStart(coordinates[1], imgData);
	hFinish = calculateHeightFinish(coordinates[2], imgData);
	finishWidth = calculateWidthFinish(coordinates[3], imgData);
	sizeString = calculateLenghtString(imgData);
}

void Red::red_filter(image_data& imgData) {
	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			for (int k = 0; k < imgData.compPerPixel; k++) {
				if (k == 0)
					imgData.pixels[k + i * sizeString + j] = (unsigned char)255;
				if (k == 1 || k == 2)
					imgData.pixels[k + i * sizeString + j] = (unsigned char)0;
			}
		}
	}
}

Threshold::Threshold(vector<int> coordinates, image_data& imgData) {
	hStart = calculateHeightStart(coordinates[0], imgData);
	startWidth = calculateWidthStart(coordinates[1], imgData);
	hFinish = calculateHeightFinish(coordinates[2], imgData);
	finishWidth = calculateWidthFinish(coordinates[3], imgData);
	sizeString = calculateLenghtString(imgData);
}

void Threshold::threshold_filter(image_data& imgData) {
	std::vector<int> matrix;
	std::vector<int> middle;
	int middleElem = 0;

	filterBW(hStart, hFinish, startWidth, finishWidth, sizeString, imgData);

	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			int newI = i - 2, newJ = j - 2 * imgData.compPerPixel;
			int endI = newI + 5, endJ = newJ + 5 * imgData.compPerPixel;
			for (int k = newI; k < endI; k++) {
				if (k >= hStart && k < hFinish) {
					for (int l = newJ; l < endJ; l += imgData.compPerPixel) {
						if (l >= startWidth && l < finishWidth) {
							matrix.push_back(imgData.pixels[k * sizeString + l]);
						}
					}
				}
			}
			//изменять
			sort(matrix.begin(), matrix.end());

			middleElem = matrix[matrix.size() / 2];
			middle.push_back(middleElem);
			matrix.clear();
		}
	}

	int numColor = 0;
	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			if (imgData.pixels[i * sizeString + j] < middle[numColor]) {
				for (int k = 0; k < imgData.compPerPixel; k++) {
					imgData.pixels[k + i * sizeString + j] = (unsigned char)0;
				}
			}
			numColor++;
		}
	}

	middle.clear();
}

Blur::Blur(vector<int> coordinates, image_data& imgData) {
	hStart = calculateHeightStart(coordinates[0], imgData);
	startWidth = calculateWidthStart(coordinates[1], imgData);
	hFinish = calculateHeightFinish(coordinates[2], imgData);
	finishWidth = calculateWidthFinish(coordinates[3], imgData);
	sizeString = calculateLenghtString(imgData);
}

void Blur::blur_filter(image_data& imgData) {
	int sumR = 0, sumG = 0, sumB = 0;
	std::vector<int> sum;

	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			int newI = i - 1, newJ = j - imgData.compPerPixel;
			int endI = newI + 3, endJ = newJ + 3 * imgData.compPerPixel;
			for (int k = newI; k < endI; k++) {
				if (k >= hStart && k < hFinish) {
					for (int l = newJ; l < endJ; l += imgData.compPerPixel) {
						if (l >= startWidth && l < finishWidth) {
							sumR += imgData.pixels[k * sizeString + l];
							sumG += imgData.pixels[k * sizeString + l + 1];
							sumB += imgData.pixels[k * sizeString + l + 2];
						}
					}
				}
			}
			//изменять
			sumR = sumR / 9;
			sumG = sumG / 9;
			sumB = sumB / 9;
			sum.push_back(sumR);
			sum.push_back(sumG);
			sum.push_back(sumB);
			sumR = 0;
			sumG = 0;
			sumB = 0;
		}
	}

	int numColor = 0;
	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			for (int k = 0; k < imgData.compPerPixel; k++) {
					imgData.pixels[k + i * sizeString + j] = (unsigned char)sum[numColor];
					numColor++;
			}
		}
	}

	sum.clear();
}

Edge::Edge(vector<int> coordinates, image_data& imgData) {
	hStart = calculateHeightStart(coordinates[0], imgData);
	startWidth = calculateWidthStart(coordinates[1], imgData);
	hFinish = calculateHeightFinish(coordinates[2], imgData);
	finishWidth = calculateWidthFinish(coordinates[3], imgData);
	sizeString = calculateLenghtString(imgData);
}

void Edge::edge_filter(image_data& imgData) {
	int sumR = 0, sumG = 0, sumB = 0;
	std::vector<int> sum;

	filterBW(hStart, hFinish, startWidth, finishWidth, sizeString, imgData);

	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			int newI = i - 1, newJ = j - imgData.compPerPixel;
			int endI = newI + 3, endJ = newJ + 3 * imgData.compPerPixel;
			for (int k = newI; k < endI; k++) {
				if (k >= hStart && k < hFinish) {
					for (int l = newJ; l < endJ; l += imgData.compPerPixel) {
						if (l >= startWidth && l < finishWidth) {
							if (k == i && l == j) {
								sumR += 9 * imgData.pixels[k * sizeString + l];
								sumG += 9 * imgData.pixels[k * sizeString + l + 1];
								sumB += 9 * imgData.pixels[k * sizeString + l + 2];
							}
							else {
								sumR += (-1) * imgData.pixels[k * sizeString + l];
								sumG += (-1) * imgData.pixels[k * sizeString + l + 1];
								sumB += (-1) * imgData.pixels[k * sizeString + l + 2];
							}
						}
					}
				}
			}
			//изменять
			//sumR = sumR / 9;
			if (sumR < 0)
				sumR = 0;
			if (sumR > 255)
				sumR = 255;
			//sumG = sumG / 9;
			if (sumG < 0)
				sumG = 0;
			if (sumG > 255)
				sumG = 255;
			//sumB = sumB / 9;
			if (sumB < 0)
				sumB = 0;
			if (sumB > 255)
				sumB = 255;
			sum.push_back(sumR);
			sum.push_back(sumG);
			sum.push_back(sumB);
			sumR = 0;
			sumG = 0;
			sumB = 0;
		}
	}

	int numColor = 0;
	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			for (int k = 0; k < imgData.compPerPixel; k++) {
				imgData.pixels[k + i * sizeString + j] = (unsigned char)sum[numColor];
				numColor++;
			}
		}
	}

	sum.clear();
}

void Filters::doFilter(configData data, image_data& imgData) {
	if (data.nameFilter == "Red") {
		Red red(data.coordinates, imgData);
		red.red_filter(imgData);
	}
	if (data.nameFilter == "Threshold") {
		Threshold tr(data.coordinates, imgData);
		tr.threshold_filter(imgData);
	}
	if (data.nameFilter == "Blur") {
		Blur blur(data.coordinates, imgData);
		blur.blur_filter(imgData);
	}
	if (data.nameFilter == "Edge") {
		Edge edge(data.coordinates, imgData);
		edge.edge_filter(imgData);
	}
}