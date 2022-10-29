#include <iostream>
#include "comics.hpp"
#include "opencv2/imgproc.hpp"

comics::comics(const std::string& fname) {
	if (fname.empty()) {
		throw std::invalid_argument("Empty fname\n");
	}

	src = cv::imread(fname);

	if (!src.data) {
		throw std::invalid_argument("No such file: " + fname);
	}

	namedWindow(window_name, cv::WINDOW_NORMAL);
	cv::createTrackbar("Th 1:", window_name, &ThresholdFirst, ThresholdFirstMax, callback, this);
	cv::createTrackbar("Th 2:", window_name, &ThresholdSecond, ThresholdSecondMax, callback, this);
	cv::createTrackbar("Blur Dev.:", window_name, &Deviation, DeviationMax, callback, this);
	cv::createTrackbar("Blur Size:", window_name, &gbSize, gbSizeMax, callback, this);
}

void comics::exec() {
	filtrate();
	imshow(window_name, dst);
}

void comics::filtrate() {
	dst = cv::Scalar::all(0);

	if (defaultMode) {
		GaussianBlur(src_gray, monoEdges, cv::Size(gbSize, gbSize), static_cast<float>(Deviation));
		Canny(monoEdges, monoEdges, ThresholdFirst, ThresholdSecond);
		src.copyTo(dst, monoEdges);
		cv::subtract(src, dst, dst);
	}
	else {
		src_gray = cv::Scalar::all(0);

		dst.create(src.size(), src.type());
		cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
		cv::split(src, channels);

		for (size_t i = 0; i < 3; i++) {
			channels_edges[i] = cv::Scalar::all(0);;
			channels_dst[i] = cv::Scalar::all(0);;

			GaussianBlur(channels[i], channels_edges[i], cv::Size(gbSize, gbSize), static_cast<float>(Deviation));
			Canny(channels_edges[i], channels_edges[i], ThresholdFirst, ThresholdSecond);

			channels[i].copyTo(channels_dst[i], channels_edges[i]);
			cv::subtract(channels[i], channels_dst[i], channels_dst[i]);
		}

		cv::merge(channels_dst, 3, dst);
	}

}

void comics::process() {
	bool flag = true;
	while (flag) {
		std::cout << "Menu: c - switch to custom (rgb) mode" << std::endl;
		std::cout << "		d - switch to default (single channel) mode" << std::endl;
		std::cout << "		s - save" << std::endl;
		std::cout << "		Esc - exit" << std::endl;

		int key = cv::waitKey();
		switch (key) {
		case 'd': {
			defaultMode = true;
			exec();
			break;
		}
		case 'c': {
			defaultMode = false;
			exec();
			break;
		}
		case 's': {
			std::cout << "Fname: ";
			std::string save_to;
			std::getline(std::cin, save_to);

			try {
				imwrite(save_to, dst);
				std::cout << "Done!" << std::endl;
			}
			catch (cv::Exception exception) {
				std::cout << exception.what() << std::endl;
			}
			break;
		}
		case 27: {
			flag = false;
			break;
		}
		case -1: {
			std::cout << "Some error" << std::endl;
			flag = false;
			break;
		}
		default: {
			std::cout << "Unknown command" << std::endl;
			break;
		}
		}
	}
}
