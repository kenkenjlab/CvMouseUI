#include <map>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

struct ItemInfo {
	cv::Rect roi;
	cv::Point text_pos;
	cv::Scalar color;
	ItemInfo() {}
};
typedef std::map<int, ItemInfo> ItemInfoMap;

class CvMouseUI {
private:
	ItemInfoMap item_info_map_;
	std::map<int, bool> selected_table_;
	cv::Point mouse_pos_;
	int font_face_;
	double font_scale_;
	int font_thickness_;
	int line_type_;
	cv::Scalar color_std_, color_mouseover_, color_selected_;

	bool checkMouseover_(int id, int x, int y);
	int checkMouseoverFromAll_(int x, int y);

public:
	CvMouseUI()
		: font_face_(CV_FONT_HERSHEY_SIMPLEX)
		, font_scale_(0.4)
		, font_thickness_(2)
		, line_type_(CV_AA)
		, mouse_pos_(-1, -1)
		, color_std_(0, 0, 0)
		, color_mouseover_(128, 128, 128)
		, color_selected_(255, 255, 255)
	{}
	void setItem(int id, int x, int y, const cv::Scalar &color);
	void configTextDrawing(int font_face, double font_scale, int font_thickness, int line_type);
	inline void clear() { item_info_map_.clear(); selected_table_.clear(); }
	void draw(cv::Mat &img);
	std::vector<int> getSelectedItems() const;

	// Callback
	void onMouse(int e, int x, int y, int flag, void* param);
};