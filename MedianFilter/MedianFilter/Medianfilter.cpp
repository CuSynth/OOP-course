#include "MedianFilter.hpp"

#include <assert.h>

void MedianFilter::filter(const pxVector& pixels, size_t width, size_t height, size_t size, size_t level) {
	if (level > size * size / 2)
		throw std::out_of_range("\"Level\" must be within 0..N^2/2!\n");
	
	img = bmp::Bitmap(width, height);
	r.resize((size + 1) * (size + 1));	//  + 1 для корректной работы с четным размером 
	g.resize((size + 1) * (size + 1));
	b.resize((size + 1) * (size + 1));

	for (size_t i = 0; i < height; ++i) {
		for (size_t j = 0; j < width; ++j) {
			EditPixel(j, i, pixels, size / 2, level);
		}
	}
}

uint8_t MedianFilter::GetColor(const std::vector<uint8_t>& col, size_t level, uint8_t current, size_t center) {
	uint8_t pivot = col[center];
	if (current <= col[center + level] && current >= col[center - level])
		return current;
	return pivot;
}

void MedianFilter::EditPixel(
	size_t X, size_t Y, const pxVector& pixels,
	size_t dist, size_t level) 
 {
	size_t width = img.Width();
	size_t height = img.Height();
	size_t pos = Y * width + X;

	size_t leftSide = X > dist ? (X - dist) : 0;
	size_t rightSide = X < (width - dist) ? (X + dist) : width - 1;
	size_t upperSide = Y > dist ? (Y - dist) : 0;
	size_t downSide = Y < (height - dist) ? (Y + dist) : height - 1;
	size_t numPx = (downSide - upperSide + 1) * (rightSide - leftSide + 1);
	if (numPx / 2 <= level) {
		img[pos].r = pixels[pos].r;	// Переносим угловые пиксели напрямую
		img[pos].g = pixels[pos].g;
		img[pos].b = pixels[pos].b;
		return;
	}

	size_t counter = 0;
	for (size_t i = leftSide; i <= rightSide; ++i) {
		for (size_t j = upperSide; j <= downSide; ++j, ++counter) {
			r[counter] = pixels[j * width + i].r;
			g[counter] = pixels[j * width + i].g;
			b[counter] = pixels[j * width + i].b;
		}
	}

	std::sort(r.begin(), r.begin() + numPx);
	std::sort(g.begin(), g.begin() + numPx);
	std::sort(b.begin(), b.begin() + numPx);

	img[pos].r = GetColor(r, level, pixels[pos].r, numPx / 2);
	img[pos].g = GetColor(g, level, pixels[pos].g, numPx / 2);
	img[pos].b = GetColor(b, level, pixels[pos].b, numPx / 2);
}

