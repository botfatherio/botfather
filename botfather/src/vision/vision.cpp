#include "vision.h"
#include "blob_tpl.h"
#include "match.h"
#include <QDebug>

// static
void Vision::saveImage(cv::UMat image, QString path)
{
	cv::Mat mat = image.getMat(cv::ACCESS_READ);
	cv::imwrite(path.toStdString(), mat);
}

// static
cv::UMat Vision::loadImage(QString path)
{
	return cv::imread(path.toStdString(), cv::IMREAD_COLOR).getUMat(cv::ACCESS_FAST);
}

// static
cv::UMat Vision::cropImage(cv::UMat image, QRect region)
{
	cv::Rect target_region(region.x(), region.y(), region.width(), region.height());
	return image(target_region);
}

// static
cv::UMat Vision::grayImage(cv::UMat image)
{
	cv::UMat gray_image;
	cv::cvtColor(image, gray_image, CV_BGR2GRAY);
	return gray_image;
}

// static
cv::UMat Vision::resizeImage(cv::UMat image, int new_width, int new_height)
{
	cv::UMat resized_image;
	cv::resize(image, resized_image, cv::Size(new_width, new_height));
	return resized_image;
}

// static
cv::UMat Vision::isolateColor(cv::UMat image, cv::Scalar min_hsv, cv::Scalar max_hsv, bool keep_color)
{
	cv::UMat hsv_image, bgr_result_image;
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
bool Vision::sameImages(cv::UMat image_1, cv::UMat image_2)
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
	cv::UMat image_1_gray, image_2_gray;
	cv::cvtColor(image_1, image_1_gray, CV_BGR2GRAY);
	cv::cvtColor(image_2, image_2_gray, CV_BGR2GRAY);
	
	// Then compare them
	cv::UMat diff;
	cv::compare(image_1_gray, image_2_gray, diff, cv::CMP_NE);
	int non_zero = cv::countNonZero(diff);
	
	// They are identical if there is no difference
	return non_zero == 0;
}

// static
QVector<Match*> Vision::findMatches(cv::UMat image, cv::UMat tpl, double threshold, int max_matches)
{
	static const int match_method = CV_TM_CCORR_NORMED;
	QVector<Match*> matches;
	
	if (image.empty()) {
		// trow exception instead.
		return matches;
	}
	
	if (tpl.empty()) {
		// trow exception instead.
		return matches;
	}
	
	double min_val, max_val, match_val;
	cv::Point min_loc, max_loc, match_loc;
	
	// Create a result matrix of the images size.
	cv::UMat result(image.rows - tpl.rows + 1, image.cols - tpl.cols + 1, CV_32FC1);

	// Mark on the result mat spots where the template matches the image the most lighter and other darker.
	cv::matchTemplate(image, tpl, result, match_method);
	
	// DO NOT NORMALIZE THE RESULT. IT WILL BE A WHITE FUCKING MATRIX!
	//cv::normalize(never, ever, use, this, shitty, fuck, shit);
	
	// Make areas completely black which are not as intensive as the threshold
	// requires. Doing so floodfill can later easily remove recognised matches
	// without making the whole result matrix black.
	cv::threshold(result, result, threshold, 1., CV_THRESH_TOZERO);

	while (matches.size() < max_matches){
		
		// Find the lightest spot aka the best matching location
		cv::minMaxLoc(result, &min_val, &max_val, &min_loc, &max_loc, cv::Mat());
		
		// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
			
			// Normalize the match value so that it can be interpreted as 0-100% accordance
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
		// be found again while looking for the next best match.
		cv::floodFill(result, match_loc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
		
		// Format the match and push it to the other matches. x and y of match_loc are
		// the left and top coordinates of the match. Note: in our Match class x and y
		// mark the center of the match.
		matches.push_back(new Match(match_val, match_loc.x, match_loc.y, tpl.cols, tpl.rows));
	}
	
	return matches;
}

// static
Match* Vision::findMatch(cv::UMat image, cv::UMat tpl, double threshold)
{
	QVector<Match*> matches = Vision::findMatches(image, tpl, threshold, 1);
	if (!matches.isEmpty()) {
		return matches[0];
	}
	return new Match();
}

// static
QVector<cv::KeyPoint> Vision::findBlobs(BlobTpl *blob_tpl, cv::UMat image)
{
	// Make pixels in the color of our intereset white and everything else black.
	cv::UMat threshold_umat = Vision::isolateColor(
		image,
		blob_tpl->getMinHSV()->getScalar(),
		blob_tpl->getMaxHSV()->getScalar(),
		false
	);
	
	// Turning the threshold umat into a mat in one step causes ocv to fuck up dealocation stuff.
	cv::Mat threshold_mat = threshold_umat.getMat(cv::ACCESS_READ);
	
	// Detect wanted (now white) color blobs as keypoints
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(blob_tpl->getBlobParams());
	std::vector<cv::KeyPoint> keypoints;
	detector->detect(threshold_mat, keypoints);
	
	return QVector<cv::KeyPoint>::fromStdVector(keypoints);
}

cv::UMat Vision::qimageToUmat(const QImage &q_image)
{
	// We always clone the q_image data as we probably always use temporary qimages.
	switch (q_image.format()) {

	// 8-bit, 4 channel
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied: {
		cv::Mat mat(
			q_image.height(),
			q_image.width(),
			CV_8UC4,
			const_cast<uchar*>(q_image.bits()),
			static_cast<size_t>(q_image.bytesPerLine())
		);
		
		cv::UMat umat;
		mat.clone().copyTo(umat);
		
		return umat;
	}
	
	// 8-bit, 3 channel
	case QImage::Format_RGB32: {
		cv::Mat mat(
			q_image.height(), q_image.width(),
			CV_8UC4,
			const_cast<uchar*>(q_image.bits()),
			static_cast<size_t>(q_image.bytesPerLine())
		);
	
		cv::UMat umat_without_alpha;
		cv::cvtColor(mat, umat_without_alpha, cv::COLOR_BGRA2BGR); // Does it copyTo?
		
		return umat_without_alpha;
	}
	
	// 8-bit, 3 channel
	case QImage::Format_RGB888: {
		QImage swapped = q_image.rgbSwapped();
		cv::Mat mat(
			swapped.height(),
			swapped.width(),
			CV_8UC3,
			const_cast<uchar*>(swapped.bits()),
			static_cast<size_t>(swapped.bytesPerLine())
		);
		
		cv::UMat umat;
		mat.clone().copyTo(umat);
		
		return umat;
	}
	
	// 8-bit, 1 channel
	case QImage::Format_Indexed8: {
		cv::Mat mat( q_image.height(), q_image.width(),
			CV_8UC1,
			const_cast<uchar*>(q_image.bits()),
			static_cast<size_t>(q_image.bytesPerLine())
		);
		
		cv::UMat umat;
		mat.clone().copyTo(umat);
		
		return umat;
	}
	
	default: {
		qWarning() << Q_FUNC_INFO << "QImage format not handled in switch:" << q_image.format();
		break;
	}
	}
	return cv::UMat();
}