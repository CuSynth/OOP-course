#ifndef __BGCROP_H__
#define __BGCROP_H__

#define DEFAULT_EPS 1
#include "BitmapPlusPlus.hpp"

class Croper{
public:
	
	void crop(const bmp::Bitmap& _img);

	void setEpsilon(int _epsilon) { epsilon = _epsilon; }
	void setPivot(int32_t _px, int32_t _py) {
		px = _px;
		py = _py;
	}

	bmp::Bitmap& GetNewBitmap() { return new_bmp; };

private:
	void FindBorder(const bmp::Bitmap& _img);

	bmp::Bitmap new_bmp;

	int epsilon = DEFAULT_EPS;
	int32_t px = 0;
	int32_t py = 0;

	int32_t Xmin = 0;
	int32_t Xmax = 0;
	int32_t Ymin = 0;
	int32_t Ymax = 0;
};

#endif // !__BGCROP_H__