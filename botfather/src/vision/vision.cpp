#include "vision.h"
#include <opencv2/highgui.hpp>
#include <QDebug>
#include "blob_tpl.h"
#include "match.h"

// static
cv::Mat Vision::cropImage(cv::Mat image, QRect region)
{
	cv::Rect target_region(region.x(), region.y(), region.width(), region.height());
	return image(target_region);
}

// static
cv::Mat Vision::grayImage(cv::Mat image)
{
	cv::Mat gray_image;
	cv::cvtColor(image, gray_image, CV_BGR2GRAY);
	return gray_image;
}

// static
cv::Mat Vision::resizeImage(cv::Mat image, int new_width, int new_height)
{
	cv::Mat resized_image;
	cv::resize(image, resized_image, cv::Size(new_width, new_height));
	return resized_image;
}

// static
cv::Mat Vision::isolateColor(cv::Mat image, cv::Scalar min_hsv, cv::Scalar max_hsv, bool keep_color)
{
	cv::Mat hsv_image, bgr_result_image;
	cv::Mat threshold_image;
	
	// Convert from BGR to HSV, because cv::inRange only works with HSV mats.
	cv::cvtColor(image, hsv_image, CV_BGR2HSV);
	
	// inRange produces a single channel binary matrix, i.e. a CV_8UC1 matrix
	// with values either 0 or 255. Note: Gimp2 shows you hsv in %.
	cv::inRange(hsv_image, min_hsv, max_hsv, threshold_image);
	
	// Set everything below the threshold color to black.
	hsv_image.setTo(cv::Scalar(0, 0, 0), ~threshold_image);
	
	if (!keep_color) {
		// Set everything which fits the threshold color to white.
		hsv_image.setTo(cv::Scalar(0, 0, 255), threshold_image);
	}
	
	// Convert HSV back to BGR, because we use BGR in our application.
	cv::cvtColor(hsv_image, bgr_result_image, CV_HSV2BGR);
	
	return bgr_result_image;
}

// static
bool Vision::sameImages(cv::Mat image_1, cv::Mat image_2)
{
	// Tread two empty images as identical
	if (image_1.empty() && image_2.empty()) {
		return true;
	}
	
	// Images or not identical if they have different sizes or dimensions
	if (image_1.cols != image_2.cols || image_1.rows != image_2.rows || image_1.dims != image_2.dims) {
		return false;
	}
	
	// Make both images grayscale (for better performance)
	cv::Mat image_1_gray, image_2_gray;
	cv::cvtColor(image_1, image_1_gray, CV_BGR2GRAY);
	cv::cvtColor(image_2, image_2_gray, CV_BGR2GRAY);
	
	// Then compare them
	cv::Mat diff;
	cv::compare(image_1_gray, image_2_gray, diff, cv::CMP_NE);
	int non_zero = cv::countNonZero(diff);
	
	// They are identical if there is no difference
	return non_zero == 0;
}

QVector<Match*> Vision::findMaskedMatches(cv::Mat image, cv::Mat tpl, cv::Mat mask, double threshold, int max_matches)
{
	// Note: Only CV_TM_SQDIFF and CV_TM_CCORR_NORMED accept maskes.
	static const int match_method = CV_TM_CCORR_NORMED;
	bool match_method_accepts_mask = (CV_TM_SQDIFF == match_method || match_method == CV_TM_CCORR_NORMED);
	bool use_mask = !mask.empty();
	
	QVector<Match*> matches;
	
	if (image.empty()) {
		// This must be prevented by the API.
		qDebug() << Q_FUNC_INFO << "Image empty";
		return matches;
	}
	
	if (tpl.empty()) {
		// This must be prevented by the API.
		qDebug() << Q_FUNC_INFO << "Template empty";
		return matches;
	}
	
	if (image.rows <= tpl.rows || image.cols <= tpl.cols) {
		// This must be prevented by the API.
		qDebug() << Q_FUNC_INFO << "Image smaller than the tpl";
		return matches;
	}

	double min_val, max_val, match_val;
	cv::Point min_loc, max_loc, match_loc;
	
	// Create a result matrix of the images size. NOTE: Saving the result image doesn't work
	// for some reason. But 
	// We tested saving it using: cv::imwrite("result.png", result.getMat(cv::ACCESS_READ).clone());
	// But result.png is always black, no matter where in this method we tried to save it.
	cv::UMat result(image.rows - tpl.rows + 1, image.cols - tpl.cols + 1, CV_32FC1);

	// Marks spots on the result mat than lighten than the template matches on the image.
	// When a mask is used ignore those regions which are black on the mask.
	if (use_mask && match_method_accepts_mask) {
		cv::matchTemplate(image, tpl, result, match_method, mask);
	} else {
		cv::matchTemplate(image, tpl, result, match_method);
	}
	
	// Makes not so good results better which results in "better" matches in some
	// cases but in wrong matches if there is actually nothing to match.
	// Note: Using this method fucked up everything already 3 times!
	//cv::normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
	
	// Make areas completely black which are not as intensive as the threshold
	// requires. Looking for minmax values and using floodfill are faster after
	// thresholding.
	cv::threshold(result, result, threshold, 1.0, CV_THRESH_TOZERO);
	
	// Uncomment the following 2 lines of code to see where matches were found.
	// One may disabled thresholding to understand the result better.
	//cv::namedWindow("Result", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Result", result);
	
	while (matches.size() < max_matches || max_matches == -1){
		
		// Find the lightest spot aka the best matching location
		cv::minMaxLoc(result, &min_val, &max_val, &min_loc, &max_loc, cv::Mat());
		
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
			cv::circle(result, match_loc, tpl.cols / 2, cv::Scalar(255), -1);
		} else {
			// ... black to hide processed matches.
			cv::circle(result, match_loc, tpl.cols / 2, cv::Scalar(0), -1);
		}
		
		// Format the match and push it to the other matches. x and y of match_loc are
		// the left and top coordinates of the match. Note: in our Match class x and y
		// mark the center of the match.
		matches.push_back(new Match(match_val, match_loc.x, match_loc.y, tpl.cols, tpl.rows));
	}
	
	return matches;
}


Match* Vision::findMaskedMatch(cv::Mat image, cv::Mat tpl, cv::Mat mask, double threshold)
{
	QVector<Match*> matches = Vision::findMaskedMatches(image, tpl, mask, threshold, 1);
	if (!matches.isEmpty()) {
		return matches[0];
	}
	return new Match();
}

QVector<Match*> Vision::findMatches(cv::Mat image, cv::Mat tpl, double threshold, int max_matches)
{
	return Vision::findMaskedMatches(image, tpl, cv::Mat(), threshold, max_matches);
}

Match* Vision::findMatch(cv::Mat image, cv::Mat tpl, double threshold)
{
	return Vision::findMaskedMatch(image, tpl, cv::Mat(), threshold);
}

QVector<cv::KeyPoint> Vision::findBlobs(BlobTpl *blob_tpl, cv::Mat image)
{
	// Make pixels in the color of our intereset white and everything else black.
	cv::Mat threshold_umat = Vision::isolateColor(
		image,
		blob_tpl->getMinHSV()->getScalar(),
		blob_tpl->getMaxHSV()->getScalar(),
		false
	);
	
	// Turning the threshold umat into a mat in one step causes ocv to fuck up dealocation stuff.
	//cv::Mat threshold_mat = threshold_umat.getMat(cv::ACCESS_READ);
	cv::Mat threshold_mat = threshold_umat;
	
	// Detect wanted (now white) color blobs as keypoints
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(blob_tpl->getBlobParams());
	std::vector<cv::KeyPoint> keypoints;
	detector->detect(threshold_mat, keypoints);
	
	return QVector<cv::KeyPoint>::fromStdVector(keypoints);
}

cv::Mat Vision::markMatches(cv::Mat image, QVector<Match *> matches, cv::Scalar color, int thickness)
{
	for (Match* match : matches) {
		image = Vision::markMatch(image, match, color, thickness);
	}
	return image;
}

cv::Mat Vision::markMatch(cv::Mat image, Match *match, cv::Scalar color, int thickness)
{
	cv::rectangle(
		image,
		cv::Point(match->getLeft(), match->getTop()),
		cv::Point(match->getRight(), match->getBottom()),
		color,
		thickness,
		8,
		0
	);
	return image;
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