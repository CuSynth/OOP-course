#ifndef __MEDIANFILTER_H__
#define __MEDIANFILTER_H__

#include "BitmapPlusPlus.hpp"

#include <vector>

using pxVector = std::vector<bmp::Pixel>;

class MedianFilter 
{
public:
	MedianFilter() = default;
	~MedianFilter() = default;
	
	void filter(const bmp::Bitmap& _img, size_t size, size_t level) {
		filter(_img.data(), _img.Width(), _img.Height(), size, level);
	}
	void filter(const pxVector& pixels, size_t width, size_t height, size_t size, size_t level);
	const bmp::Bitmap& get_result() { return img; };

private:
	bmp::Bitmap img;

	std::vector<uint8_t> r;
	std::vector<uint8_t> g;
	std::vector<uint8_t> b; 
	
	void EditPixel(
		size_t X, size_t Y, const pxVector& pixels,
		size_t dist, size_t level);

	uint8_t GetColor(const std::vector<uint8_t>& col, size_t level, uint8_t current, size_t center);
};

#endif // !__MEDIANFILTER_H__