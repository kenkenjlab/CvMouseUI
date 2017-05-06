#include "cv_mouse_ui.hpp"
#include <random>
#include <ctime>
#include <opencv2/highgui.hpp>
#include <iostream>

CvMouseUI _cv_mouse_ui;
void onMouse(int e, int x, int y, int flag, void* param) { _cv_mouse_ui.onMouse(e, x, y, flag, param); }

int main(int argc, char** argv) {
	std::srand(std::time(NULL));
	cv::Size size(1280, 720);
	cv::Mat img(size, CV_8UC3, cv::Scalar::all(64));

	_cv_mouse_ui.configTextDrawing(CV_FONT_HERSHEY_SIMPLEX, 0.5, 2, CV_AA);
	_cv_mouse_ui.clear();
	for (int i = 0; i < 100; ++i) {
		cv::Scalar color(std::rand() % 256, std::rand() % 256, std::rand() % 256);
		_cv_mouse_ui.setItem(std::rand() % 1000, std::rand() % size.width, std::rand() % size.height, color);
	}

	cv::namedWindow("img");
	bool iterate = true;
	while (iterate) {
		cv::setMouseCallback("img", onMouse);
		cv::Mat img2 = img.clone();
		_cv_mouse_ui.draw(img2);
		cv::imshow("img", img2);
		int key = cv::waitKey(10);
		switch (key) {
		case 'q':
			iterate = false;
			break;
		case ' ':
		{
			std::vector<int> selected_items = _cv_mouse_ui.getSelectedItems();
			for (int i = 0; i < selected_items.size(); ++i) {
				int id = selected_items[i];
				std::cout << id << ",";
			}
			std::cout << std::endl;
		}
			break;
		}
	}

	return 0;
}