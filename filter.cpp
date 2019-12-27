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
	return wStart;
}
int Filter::calculateWidthFinish(int right, image_data& imgData) {
	int wFinish;
	if (right == 0)
		wFinish = 0;
	else
		wFinish = imgData.w / right;
	return wFinish;
}
/*int Filter::calculateLenghtString(image_data& imgData) {
	int sizeString = imgData.compPerPixel * imgData.w;
	return sizeString;
}*/
void Filter::filterBW(vector<int> coordinates, image_data& imgData) {
	for (int i = coordinates[0]; i < coordinates[2]; i++) {
		for (int j = coordinates[1]; j < coordinates[3]; j++) {
			int pos = (i * imgData.w + j) * imgData.compPerPixel;
			int r = imgData.pixels[pos];
			int g = imgData.pixels[pos + 1];
			int b = imgData.pixels[pos + 2];
			int x = 0.3 * r + 0.6 * g + 0.1 * b;
			for (int k = 0; k < 3; k++) {
				imgData.pixels[pos + k] = (unsigned char)x;
			}
		}
	}
}

Red::Red(vector<int> coordinates, image_data& imgData) {
	posPicture.push_back(calculateHeightStart(coordinates[0], imgData));
	posPicture.push_back(calculateWidthStart(coordinates[1], imgData));
	posPicture.push_back(calculateHeightFinish(coordinates[2], imgData));
	posPicture.push_back(calculateWidthFinish(coordinates[3], imgData));
}

void Red::red_filter(image_data& imgData) {
	for (int i = posPicture[0]; i < posPicture[2]; i++) {
		for (int j = posPicture[1]; j < posPicture[3]; j++) {
			int pos = (i * imgData.w + j) * imgData.compPerPixel;
			for (int k = 0; k < 3; k++) {
				if (k == 0)
					imgData.pixels[k + pos] = (unsigned char)255;
				if (k == 1 || k == 2)
					imgData.pixels[k + pos] = (unsigned char)0;
			}
		}
	}
}

Threshold::Threshold(vector<int> coordinates, image_data& imgData) {
	posPicture.push_back(calculateHeightStart(coordinates[0], imgData));
	posPicture.push_back(calculateWidthStart(coordinates[1], imgData));
	posPicture.push_back(calculateHeightFinish(coordinates[2], imgData));
	posPicture.push_back(calculateWidthFinish(coordinates[3], imgData));
}

/*void Threshold::threshold_filter(image_data& imgData) {
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
}*/

int Threshold::middleFilter(image_data& imgData, vector<int>matrixCoordinates) {
	std::vector<int> middle;
	int middleElem = 0;
	for (int k = matrixCoordinates[0]; k <= matrixCoordinates[2]; k++) {
		if (k >= posPicture[0] && k < posPicture[2]) {
			for (int l = matrixCoordinates[1]; l <= matrixCoordinates[3]; l++) {
				if (l >= posPicture[1] && l < posPicture[3]) {
					middle.push_back(imgData.pixels[(k * imgData.w + l) * imgData.compPerPixel]);
				}
			}
		}
	}
	//изменять
	std::sort(middle.begin(), middle.end());

	middleElem = middle[middle.size() / 2];
	middle.clear();
	return middleElem;
}

void Threshold::threshold_filter(image_data& imgData) {
	std::vector<int> matrixCoordinates;
	int middleElem = 0;

	filterBW(posPicture, imgData);

	image_data copyData;
	copyData.compPerPixel = imgData.compPerPixel;
	copyData.h = imgData.h;
	copyData.w = imgData.w;
	int size = copyData.w * copyData.h * copyData.compPerPixel;
	copyData.pixels = new stbi_uc[size];
	memcpy(copyData.pixels, imgData.pixels, size);

	for (int i = posPicture[0]; i < posPicture[2]; i++) {
		for (int j = posPicture[1]; j < posPicture[3]; j++) {
			int pos = (i * imgData.w + j) * imgData.compPerPixel;
			matrixCoordinates.push_back(i - 2);
			matrixCoordinates.push_back(j - 2);
			matrixCoordinates.push_back(i + 2);
			matrixCoordinates.push_back(j + 2);
			middleElem = middleFilter(copyData, matrixCoordinates);
			if (imgData.pixels[pos] < middleElem) {
				for (int k = 0; k < 3; k++) {
					imgData.pixels[k + pos] = (unsigned char)0;
				}
			}
			matrixCoordinates.clear();
		}
	}
	delete[] copyData.pixels;
}
/*void Threshold::threshold_filter(image_data& imgData) {
	std::vector<int> matrixCoordinates;
	int middleElem = 0;

	filterBW(hStart, hFinish, startWidth, finishWidth, sizeString, imgData);
	image_data copyData;
	copyData.compPerPixel = imgData.compPerPixel;
	copyData.h = imgData.h;
	copyData.w = imgData.w;
	int sizeData = copyData.h * copyData.w * copyData.compPerPixel;
	copyData.pixels = new stbi_uc[sizeData];
	memcpy(copyData.pixels, imgData.pixels, sizeData);

	for (int i = hStart; i < hFinish; i++) {
		for (int j = startWidth; j < finishWidth; j += imgData.compPerPixel) {
			matrixCoordinates.push_back(i - 2);
			matrixCoordinates.push_back(j - 2 * copyData.compPerPixel);
			matrixCoordinates.push_back(i + 2);
			matrixCoordinates.push_back(j + 2 * copyData.compPerPixel);
			middleElem = middleFilter(copyData, matrixCoordinates);
			if (imgData.pixels[i * sizeString + j] < middleElem) {
				for (int k = 0; k < imgData.compPerPixel; k++) {
					imgData.pixels[k + i * sizeString + j] = (unsigned char)0;
				}
			}
			matrixCoordinates.clear();
		}
	}

}*/
/*void Threshold::threshold_filter(image_data& imgData) {
	std::vector<int> matrixCoordinates;
	std::vector<int> middle;
	int middleElem = 0;

	filterBW(posPicture, imgData);

	image_data saveImg;
	saveImg.compPerPixel = imgData.compPerPixel;
	saveImg.h = imgData.h;
	saveImg.w = imgData.w;
	int size = saveImg.h * saveImg.w * saveImg.compPerPixel;
	saveImg.pixels = new stbi_uc[size];
	memcpy(saveImg.pixels, imgData.pixels, size);

	for (int i = posPicture[0]; i < posPicture[2]; i++) {
		for (int j = posPicture[1]; j < posPicture[3]; j++) {
			matrixCoordinates.push_back(i - 2);
			matrixCoordinates.push_back(j - 2);
			matrixCoordinates.push_back(i + 2);
			matrixCoordinates.push_back(j + 2);
			middleElem = middleFilter(imgData, matrixCoordinates);
			middle.push_back(middleElem);
			matrixCoordinates.clear();
		}
	}

	int numMiddle = 0;
	for (int i = posPicture[0]; i < posPicture[2]; i++) {
		for (int j = posPicture[1]; j < posPicture[3]; j++) {
			int pos = (i * imgData.w + j) * imgData.compPerPixel;
			if (imgData.pixels[pos] < middle[numMiddle]) {
				for (int k = 0; k < 3; k++) {
					imgData.pixels[k + pos] = (unsigned char)0;
				}
			}
			numMiddle++;
		}
	}
	middle.clear();

}*/

Blur::Blur(vector<int> coordinates, image_data& imgData) {
	posPicture.push_back(calculateHeightStart(coordinates[0], imgData));
	posPicture.push_back(calculateWidthStart(coordinates[1], imgData));
	posPicture.push_back(calculateHeightFinish(coordinates[2], imgData));
	posPicture.push_back(calculateWidthFinish(coordinates[3], imgData));
}

vector<int> Blur::filterExtraBlur(image_data& imgData, vector<int>matrixCoordinates) {
	int sumR = 0, sumG = 0, sumB = 0;
	vector<int> sum;
	for (int k = matrixCoordinates[0]; k <= matrixCoordinates[2]; k++) {
		if (k >= posPicture[0] && k < posPicture[2]) {
			for (int l = matrixCoordinates[1]; l <= matrixCoordinates[3]; l++) {
				if (l >= posPicture[1] && l < posPicture[3]) {
					int pos = (k * imgData.w + l) * imgData.compPerPixel;
					sumR += imgData.pixels[pos];
					sumG += imgData.pixels[pos + 1];
					sumB += imgData.pixels[pos + 2];
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
	return sum;
}

void Blur::blur_filter(image_data& imgData) {
	std::vector<int> sum;
	std::vector<int> matrixCoordinates;

	image_data copyData;
	copyData.compPerPixel = imgData.compPerPixel;
	copyData.h = imgData.h;
	copyData.w = imgData.w;
	int size = copyData.w * copyData.h * copyData.compPerPixel;
	copyData.pixels = new stbi_uc[size];
	memcpy(copyData.pixels, imgData.pixels, size);

	for (int i = posPicture[0]; i < posPicture[2]; i++) {
		for (int j = posPicture[1]; j < posPicture[3]; j++) {
			int pos = (i * imgData.w + j) * imgData.compPerPixel;
			matrixCoordinates.push_back(i - 1);
			matrixCoordinates.push_back(j - 1);
			matrixCoordinates.push_back(i + 1);
			matrixCoordinates.push_back(j + 1);
			sum = filterExtraBlur(copyData, matrixCoordinates);
			for (int k = 0; k < 3; k++) {
				imgData.pixels[k + pos] = sum[k];
			}
			sum.clear();
			matrixCoordinates.clear();
		}
	}

	delete[] copyData.pixels;
}

/*void Blur::blur_filter(image_data& imgData) {
	int sumR = 0, sumG = 0, sumB = 0;
	std::vector<int> sum;

	for (int i = posPicture[0]; i < posPicture[2]; i++) {
		for (int j = posPicture[1]; j < posPicture[3]; j++) {
			int newI = i - 1, newJ = j - 1;
			int endI = newI + 3, endJ = newJ + 3;
			for (int k = newI; k < endI; k++) {
				if (k >= posPicture[0] && k < posPicture[2]) {
					for (int l = newJ; l < endJ; l++) {
						if (l >= posPicture[1] && l < posPicture[3]) {
							int pos = (k * imgData.w + l) * imgData.compPerPixel;
							sumR += imgData.pixels[pos];
							sumG += imgData.pixels[pos + 1];
							sumB += imgData.pixels[pos + 2];
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
	for (int i = posPicture[0]; i < posPicture[2]; i++) {
		for (int j = posPicture[1]; j < posPicture[3]; j++) {
			int pos = (i * imgData.w + j) * imgData.compPerPixel;
			for (int k = 0; k < 3; k++) {
					imgData.pixels[k + pos] = (unsigned char)sum[numColor];
					numColor++;
			}
		}
	}

	sum.clear();
}*/

Edge::Edge(vector<int> coordinates, image_data& imgData) {
	posPicture.push_back(calculateHeightStart(coordinates[0], imgData));
	posPicture.push_back(calculateWidthStart(coordinates[1], imgData));
	posPicture.push_back(calculateHeightFinish(coordinates[2], imgData));
	posPicture.push_back(calculateWidthFinish(coordinates[3], imgData));
}

int Edge::filterExtraEdge(image_data& imgData, vector<int>matrixCoordinates) {
	int sumPixel = 0;
	int middleMatrix = 0;
	for (int k = matrixCoordinates[0]; k <= matrixCoordinates[2]; k++) {
		if (k >= posPicture[0] && k < posPicture[2]) {
			for (int l = matrixCoordinates[1]; l <= matrixCoordinates[3]; l++) {
				if (l >= posPicture[1]  && l < posPicture[3]) {
					int pos = (k * imgData.w + l) * imgData.compPerPixel;
					if (middleMatrix == 9 / 2) {
						sumPixel += 9 * imgData.pixels[pos];
					}
					else {
						sumPixel -= imgData.pixels[pos];
					}
				}
				middleMatrix++;
			}
		}
		else
			middleMatrix += 3;

	}
	if (sumPixel < 0)
		sumPixel = 0;
	if (sumPixel > 255)
		sumPixel = 255;
	return sumPixel;
}

void Edge::edge_filter(image_data& imgData) {
	std::vector<int> matrixCoordinates;
	int sumPixel = 0;

	filterBW(posPicture, imgData);

	image_data copyData;
	copyData.compPerPixel = imgData.compPerPixel;
	copyData.h = imgData.h;
	copyData.w = imgData.w;
	int size = copyData.w * copyData.h * copyData.compPerPixel;
	copyData.pixels = new stbi_uc[size];
	memcpy(copyData.pixels, imgData.pixels, size);

	for (int i = posPicture[0]; i < posPicture[2]; i++) {
		for (int j = posPicture[1]; j < posPicture[3]; j++) {
			int pos = (i * imgData.w + j) * imgData.compPerPixel;
			matrixCoordinates.push_back(i - 1);
			matrixCoordinates.push_back(j - 1);
			matrixCoordinates.push_back(i + 1);
			matrixCoordinates.push_back(j + 1);
			sumPixel = filterExtraEdge(copyData, matrixCoordinates);
			for (int k = 0; k < 3; k++) {
				imgData.pixels[k + pos] = sumPixel;
			}
			matrixCoordinates.clear();
		}
	}

	delete[] copyData.pixels;
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