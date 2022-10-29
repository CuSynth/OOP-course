#pragma once
#include "opencv2/highgui.hpp"

class comics
{
public:
	comics() = delete;
	comics(const std::string& fname);

	void process();

private:
	void exec();
	void filtrate();

	const char* window_name = "comics";
	bool defaultMode = false;

	cv::Mat src;
	cv::Mat src_gray;
	cv::Mat dst;

	cv::Mat monoEdges;

	cv::Mat channels[3];
	cv::Mat channels_edges[3];
	cv::Mat channels_dst[3];

	int ThresholdFirst = 80;
	int ThresholdFirstMax = 500;
	int ThresholdSecond = 85;
	int ThresholdSecondMax = 500;
	int Deviation = 2;
	int DeviationMax = 100;
	int gbSize = 7;
	int gbSizeMax = 15;

	static void callback(int pos, void* usr) {
		auto _usr = reinterpret_cast<comics*>(usr);
		_usr->gbSize = _usr->gbSize % 2 == 0 ? _usr->gbSize + 1 : _usr->gbSize;
		_usr->exec();
	}

};