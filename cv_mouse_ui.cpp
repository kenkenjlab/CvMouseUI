#include "cv_mouse_ui.hpp"
#include <opencv2/highgui.hpp>
#include <sstream>

void CvMouseUI::setItem(int id, int x, int y, const cv::Scalar &color) {
	std::stringstream ss;
	ss << id;
	int baseline = 0;
	cv::Size text_size = cv::getTextSize(ss.str(), font_face_, font_scale_, font_thickness_, &baseline);
	ItemInfo item_info;
	item_info.roi = cv::Rect(x, y - text_size.height, text_size.width, text_size.height + baseline);
	item_info.text_pos = cv::Point(x, y);
	item_info.color = color;
	item_info_map_[id] = item_info;
	if (selected_table_.find(id) == selected_table_.cend()) {
		selected_table_[id] = false;
	}
}

void CvMouseUI::configTextDrawing(int font_face, double font_scale, int font_thickness, int line_type) {
	font_face_ = font_face;
	font_scale_ = font_scale;
	font_thickness_ = font_thickness;
	line_type_ = line_type;
}

bool CvMouseUI::checkMouseover_(int id, int x, int y) {
	if (item_info_map_.find(id) == item_info_map_.cend()) { return false; }
	const ItemInfo &item_info = item_info_map_.at(id);
	const cv::Rect &rect = item_info.roi;
	if (x < rect.x || x > rect.x + rect.width || y < rect.y || y > rect.y + rect.height) { return false; }
	return true;
}

int CvMouseUI::checkMouseoverFromAll_(int x, int y) {
	for (ItemInfoMap::const_reverse_iterator crit_map = item_info_map_.crbegin(); crit_map != item_info_map_.crend(); ++crit_map) {
		int id = crit_map->first;
		if (checkMouseover_(id, x, y)) { return id; }
	}
	return -1;
}

void CvMouseUI::draw(cv::Mat &img) {
	// Find item under the mouse cursor
	int active_id = checkMouseoverFromAll_(mouse_pos_.x, mouse_pos_.y);
	
	// Draw
	for (ItemInfoMap::const_iterator cit_map = item_info_map_.cbegin(); cit_map != item_info_map_.cend(); ++cit_map) {
		const int id = cit_map->first;
		const ItemInfo &item_info = cit_map->second;

		const cv::Scalar *color = &color_std_;
		if (selected_table_.at(id)) { color = &color_selected_; }
		else if (id == active_id) { color = &color_mouseover_; }
		std::stringstream ss;
		ss << id;
		//cv::rectangle(img, item_info.roi, color, CV_FILLED);
		cv::putText(img, ss.str(), item_info.text_pos, font_face_, font_scale_, *color, font_thickness_, line_type_);
		cv::putText(img, ss.str(), item_info.text_pos, font_face_, font_scale_, item_info.color, font_thickness_ - 1, line_type_);
	}
}

std::vector<int> CvMouseUI::getSelectedItems() const {
	std::vector<int> vec;
	for (std::map<int, bool>::const_iterator cit_map = selected_table_.cbegin(); cit_map != selected_table_.cend(); ++cit_map) {
		if (!cit_map->second) { continue; }
		vec.push_back(cit_map->first);
	}
	return vec;
}

void CvMouseUI::onMouse(int e, int x, int y, int flag, void* param) {
	if (e == CV_EVENT_LBUTTONUP) {
		int active_id = checkMouseoverFromAll_(mouse_pos_.x, mouse_pos_.y);
		if (selected_table_.find(active_id) != selected_table_.cend()) {
			selected_table_[active_id] ^= true;
		}
	} else if (e == CV_EVENT_MOUSEMOVE) {
		mouse_pos_.x = x;
		mouse_pos_.y = y;
	}
}