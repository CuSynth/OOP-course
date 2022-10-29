#include "bg_crop.hpp"

void Croper::FindBorder(const bmp::Bitmap& _img) {
	int32_t width = _img.Width();
	int32_t height = _img.Height();
	Xmin = width - 1;
	Ymin = height - 1;
	
	int	pr = _img.data()[py*width + px].r;
	int	pg = _img.data()[py * width + px].g;
	int	pb = _img.data()[py * width + px].b;
	 
	for (int32_t i = 0; i < height; i++) {
		for(int32_t j = 0; j < width; j++){
			if ((abs(_img[i * width + j].r - pr) > epsilon)
				|| (abs(_img[i * width + j].g - pg) > epsilon)
				|| (abs(_img[i * width + j].b - pb) > epsilon))
			{
				if (i > Ymax)
					Ymax = i;
				if (j > Xmax)
					Xmax = j;
				if (i < Ymin)
					Ymin = i;
				if (j < Xmin)
					Xmin = j;
			}
		}
	}
}

void  Croper::crop(const bmp::Bitmap& _img) {

	FindBorder(_img);
	
	if (Xmin > Xmax || Ymin > Ymax)
		return;

	int32_t width = _img.Width();
	int32_t height = _img.Height();
	
	int32_t newWidth = Xmax - Xmin + 1;
	int32_t newHeight = Ymax - Ymin + 1;

	new_bmp = bmp::Bitmap(newWidth, newHeight);

	//auto bmpIt = _img.data().begin();
	//auto newBmpIt = _img.data().begin();

	//for (size_t i = 0; i < newHeight; i++) {
	//	std::copy(bmpIt + (i * newWidth) + Xmin, bmpIt + (i * newWidth) + newHeight, newBmpIt + i * newWidth);
	//}

	for (size_t i = 0; i < newHeight; i++)
	{
		for (size_t j = 0; j < newWidth; j++)
		{
			new_bmp[i * newWidth + j] = _img[(Ymin + i) * width + (Xmin + j)];
		}
	}
}
