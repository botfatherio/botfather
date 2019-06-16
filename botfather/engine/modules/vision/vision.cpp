#include "vision.hpp"
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QPainter>
#include "types/match.hpp"
#include "types/blob_tpl.hpp"

cv::Mat Vision::isolateColor(const cv::Mat &image, const cv::Scalar &min_hsv, const cv::Scalar &max_hsv, bool keep_color)
{
	cv::UMat hsv_umat, bgr_result_umat;
	cv::UMat threshold_umat, inverted_threshold;

	// Convert from BGR to HSV, because cv::inRange only works with HSV mats.
	cv::cvtColor(image, hsv_umat, CV_BGR2HSV);

	// inRange produces a single channel binary matrix, i.e. a CV_8UC1 matrix
	// with values either 0 or 255. Note: Gimp2 shows you hsv in %.
	cv::inRange(hsv_umat, min_hsv, max_hsv, threshold_umat);

	// Inverts the threshold
	cv::bitwise_not(threshold_umat, inverted_threshold);

	// Set everything below the threshold color to black.
	hsv_umat.setTo(cv::Scalar(0, 0, 0), inverted_threshold);

	if (!keep_color) {
		// Set everything which fits the threshold color to white.
		hsv_umat.setTo(cv::Scalar(0, 0, 255), threshold_umat);
	}

	// Convert HSV back to BGR, because we use BGR in our application.
	cv::cvtColor(hsv_umat, bgr_result_umat, CV_HSV2BGR);

	hsv_umat.release();
	threshold_umat.release();
	inverted_threshold.release();

	return bgr_result_umat.getMat(cv::ACCESS_READ).clone();
}

int Vision::countDifferentPixels(const cv::Mat &image_1, const cv::Mat &image_2)
{
	cv::Mat image_1_gray, image_2_gray, diff;
	
	// Make both images grayscale (for better performance)
	cv::cvtColor(image_1, image_1_gray, CV_BGR2GRAY);
	cv::cvtColor(image_2, image_2_gray, CV_BGR2GRAY);
	
	// CMP_NE: image_1 is unequal to image_2.
	cv::compare(image_1_gray, image_2_gray, diff, cv::CMP_NE);
	
	image_1_gray.release();
	image_2_gray.release();

	// signed int: -2,147,483,647 to 2,147,483,647
	return cv::countNonZero(diff);
}

double Vision::histogramSimilarity(const cv::Mat &image_1, const cv::Mat &image_2)
{
	cv::Mat hsv_1, hsv_2;
	cv::cvtColor(image_1, hsv_1, CV_BGR2HSV);
	cv::cvtColor(image_2, hsv_2, CV_BGR2HSV);
	
	// Using 50 bins for hue and 60 for saturation
	int h_bins = 50;
	int s_bins = 60;
	int v_bins = 60;
	int hist_size[] = { h_bins, s_bins, v_bins };
	
	// Use the o-th and 1-st channels
	int channels[] = { 0, 1, 2 };
	
	// hue varies from 0 to 179, saturation from 0 to 255
	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };
	float v_ranges[] = { 0, 256 };
	const float* ranges[] = { h_ranges, s_ranges, v_ranges };
	
	int dims = 3;
	
	cv::MatND hsv_1_hist, hsv_2_hist;
	
	cv::calcHist(&hsv_1, 1, channels, cv::Mat(), hsv_1_hist, dims, hist_size, ranges, true, false);
	cv::normalize(hsv_1_hist, hsv_1_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
	
	cv::calcHist(&hsv_2, 1, channels, cv::Mat(), hsv_2_hist, dims, hist_size, ranges, true, false);
	cv::normalize(hsv_2_hist, hsv_2_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());	
	
	hsv_1.release();
	hsv_2.release();

	return cv::compareHist(hsv_1_hist, hsv_2_hist, CV_COMP_CORREL);
}

QVector<Match> Vision::findMaskedMatches(const cv::Mat &image, const cv::Mat &tpl, const cv::Mat &mask, double threshold, int max_matches)
{
	// Note: Only CV_TM_SQDIFF and CV_TM_CCORR_NORMED accept maskes.
	static const int match_method = CV_TM_CCORR_NORMED;
	bool match_method_accepts_mask = (CV_TM_SQDIFF == match_method || match_method == CV_TM_CCORR_NORMED);
	bool use_mask = !mask.empty();
	
	QVector<Match> matches;

	cv::UMat uimage = image.getUMat(cv::ACCESS_RW);
	cv::UMat utpl = tpl.getUMat(cv::ACCESS_RW);

	double min_val, max_val, match_val;
	cv::Point min_loc, max_loc, match_loc;
	
	// Create a result matrix of the images size. NOTE: Saving the result image doesn't work
	// for some reason. But 
	// We tested saving it using: cv::imwrite("result.png", result.getMat(cv::ACCESS_READ).clone());
	// But result.png is always black, no matter where in this method we tried to save it.
	cv::UMat uresult(uimage.rows - utpl.rows + 1, uimage.cols - utpl.cols + 1, CV_32FC1);
	
	// cv::matchTemplate requires the image to have a depth of 0 or 5, and both the image and the template
	// to gave the same types. The images dims must be equal or less 2.
	Q_ASSERT(uimage.depth() == 0 || uimage.depth() == 5);
	Q_ASSERT(uimage.type() == utpl.type());
	Q_ASSERT(uimage.dims <= 2);

	// Marks spots on the result mat than lighten than the template matches on the image.
	// When a mask is used ignore those regions which are black on the mask.
	if (use_mask && match_method_accepts_mask) {
		cv::matchTemplate(uimage, utpl, uresult, match_method, mask);
	} else {
		cv::matchTemplate(uimage, utpl, uresult, match_method);
	}
	
	// Makes not so good results better which results in "better" matches in some
	// cases but in wrong matches if there is actually nothing to match.
	// Note: Using this method fucked up everything already 3 times!
	//cv::normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
	
	// Make areas completely black which are not as intensive as the threshold
	// requires. Looking for minmax values and using floodfill are faster after
	// thresholding.
	cv::threshold(uresult, uresult, threshold, 1.0, CV_THRESH_TOZERO);
	
	// Uncomment the following 2 lines of code to see where matches were found.
	// One may disabled thresholding to understand the result better.
	//cv::namedWindow("Result", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Result", result);
	
	while (matches.size() < max_matches || max_matches == -1){
		
		// Find the lightest spot aka the best matching location
		cv::minMaxLoc(uresult, &min_val, &max_val, &min_loc, &max_loc, cv::Mat());
		
		// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
			match_val = 1.0 - min_val;
			match_loc = min_loc;
		} else{
			match_val = max_val;
			match_loc = max_loc;
		}
		
		// Stop looking for more matching when the matches threshold becomes to low.
		if (match_val < threshold) {
			break;
		}

		// Make the currently lightest (best matching) location black, so it won't
		// be found again while looking for the next best match. One can use floodfill
		// to do so, which should work as expected, especially after thresholding.
		// BUT we had some issues doing so! (Matches where found twice or more. With a
		// different configuration the whole result image turned black after the first
		// match). Drawing a circle where the lightes match is turned out to be more
		// reliable.
		// NOTE: Whatever method is used, one has to respect the user match method as
		// different methods mark the best matching location either black OR white.
		
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
			// Here we use the color white to hide processed matches.
			// A thickness of -1 means the circle shall be filled.
			cv::circle(uresult, match_loc, utpl.cols / 2, cv::Scalar(255), -1);
		} else {
			// ... black to hide processed matches.
			cv::circle(uresult, match_loc, utpl.cols / 2, cv::Scalar(0), -1);
		}
		
		// Format the match and push it to the other matches. x and y of match_loc are
		// the left and top coordinates of the match. Note: in our Match class x and y
		// mark the center of the match.
		matches.push_back(Match(QRect(match_loc.x, match_loc.y, utpl.cols, utpl.rows), match_val));
	}
	
	uimage.release();
	utpl.release();
	uresult.release();

	return matches;
}


Match Vision::findMaskedMatch(const cv::Mat &image, const cv::Mat &tpl, const cv::Mat &mask, double threshold)
{
	QVector<Match> matches = Vision::findMaskedMatches(image, tpl, mask, threshold, 1);
	if (!matches.isEmpty()) {
		return matches[0];
	}
	return Match();
}

QVector<Match> Vision::findMatches(const cv::Mat &image, const cv::Mat &tpl, double threshold, int max_matches)
{
	return Vision::findMaskedMatches(image, tpl, cv::Mat(), threshold, max_matches);
}

Match Vision::findMatch(const cv::Mat &image, const cv::Mat &tpl, double threshold)
{
	return Vision::findMaskedMatch(image, tpl, cv::Mat(), threshold);
}

QVector<Match> Vision::findBlobs(const cv::Mat &image, const cv::SimpleBlobDetector::Params &blob_params)
{
	cv::UMat image_umat;
	image.copyTo(image_umat);

	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(blob_params);
	std::vector<cv::KeyPoint> key_points;
	detector->detect(image_umat, key_points);
	
	QVector<Match> matches;
	
	for (cv::KeyPoint kp : key_points)
	{	
		int size = static_cast<int>(kp.size);
		int left = static_cast<int>(kp.pt.x) - size / 2;
		int top = static_cast<int>(kp.pt.y) - size / 2;
		
		Match match(QRect(left, top, size, size), 1);
		matches.append(match);
	}
	
	image_umat.release();
	return matches;
}

QImage Vision::markMatches(const QImage &image, const QVector<Match> &matches, const QColor &color, int thickness)
{
	QImage result = image;
	
	QPen pen;
	pen.setColor(color);
	pen.setWidth(thickness);
	
	QPainter painter(&result);
	painter.setPen(pen);
	
	for (Match match : matches) {
		painter.drawRect(match);
	}
	
	painter.end();
	return result;
}

cv::Mat Vision::qimageToBGRMat(const QImage &qimage)
{
	switch(qimage.format()) {
	
	// 8-bit, 4 channel
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied: {
		cv::Mat mat(
			qimage.height(),
			qimage.width(),
			CV_8UC4,
			const_cast<uchar*>(qimage.bits()),
			static_cast<size_t>(qimage.bytesPerLine())
		);

		return mat.clone();
	}
	
	// 8-bit, 3 channel
	case QImage::Format_RGB32: {
		cv::Mat mat(
			qimage.height(),
			qimage.width(),
			CV_8UC4,
			const_cast<uchar*>(qimage.bits()),
			static_cast<size_t>(qimage.bytesPerLine())
		);
	 
		cv::Mat mat_without_alpha;
		cv::cvtColor(mat, mat_without_alpha, cv::COLOR_BGRA2BGR);
		return mat_without_alpha;
	}
	 
	// 8-bit, 3 channel
	case QImage::Format_RGB888: {
		QImage swapped = qimage.rgbSwapped();
		return cv::Mat(
			swapped.height(),
			swapped.width(),
			CV_8UC3,
			const_cast<uchar*>(swapped.bits()),
			static_cast<size_t>(swapped.bytesPerLine())
		).clone();
	}
	 
	// 8-bit, 1 channel
	case QImage::Format_Indexed8: {
		cv::Mat mat(
			qimage.height(),
			qimage.width(),
			CV_8UC1,
			const_cast<uchar*>(qimage.bits()),
			static_cast<size_t>(qimage.bytesPerLine())
		);
		return mat.clone();
	}
	
	default:
		qWarning() << "QImage format not handled in switch:" << qimage.format();
		break;
	}
	return cv::Mat();
}

QImage Vision::cvMatToQImage(const cv::Mat &mat)
{
	switch (mat.type()) {
	
	// 8-bit, 4 channel
    case CV_8UC4: {
		QImage image(
			mat.data,
			mat.cols,
			mat.rows,
            static_cast<int>(mat.step),
			QImage::Format_ARGB32
		);
		return image;
	}
 
    // 8-bit, 3 channel
	case CV_8UC3: {
		QImage image(
			mat.data,
			mat.cols,
			mat.rows,
			static_cast<int>(mat.step),
			QImage::Format_RGB888
		);
		return image.rgbSwapped();
	}
 
	// 8-bit, 1 channel
	case CV_8UC1: {
		QImage image(
			mat.data,
            mat.cols,
            mat.rows,
            static_cast<int>(mat.step),
            QImage::Format_Grayscale8
		);
		return image;
	}
 
    default:
		qWarning() << "cv::Mat image type not handled in switch:" << mat.type();
		break;
	}
 
	return QImage();
}

cv::Scalar Vision::qColorToHsvScalar(const QColor &qcolor)
{
	return cv::Scalar(qcolor.hslHue(), qcolor.hslSaturation(), qcolor.value());
}
