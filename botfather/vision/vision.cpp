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
	
	double min_val, max_val;
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
		
		// Stop looking for more matching when the matches threshold becomes to low.
		if (max_val < threshold)
			break;

		// Make the currently lightest (best matching) location black, so it won't
		// be found again while looking for the next best match.
		cv::floodFill(result, max_loc, cv::Scalar(0), 0, cv::Scalar(.1), cv::Scalar(1.));
		
		// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
		if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
			match_loc = min_loc;
		else
			match_loc = max_loc;
		
		// Turn the results into a TplMatch and push it to the results vector.
		QRect match_rect;
		match_rect.setLeft(match_loc.x);
		match_rect.setTop(match_loc.y);
		match_rect.setRight(match_loc.x + tpl.cols);
		match_rect.setBottom(match_loc.y + tpl.rows);
		match_rect.setWidth(tpl.cols);
		match_rect.setHeight(tpl.rows);
		
		matches.push_back(new Match(max_val, match_rect));
	}
	
	return matches;
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

// static
cv::UMat Vision::qimageToUmat(const QImage &q_image, bool clone_image_data)
{
	switch (q_image.format()) {
	case QImage::Format_RGB32: {
		// 8-bit, 4 channel
		
		cv::Mat mat(q_image.height(), q_image.width(), CV_8UC4, const_cast<uchar*>(q_image.bits()), q_image.bytesPerLine());
		cv::UMat result_umat;
		(clone_image_data ? mat.clone() : mat).copyTo(result_umat);

		// Cut off the alpha channel
		cv::cvtColor(result_umat, result_umat, cv::COLOR_BGRA2BGR);
		
		return result_umat;
	}
	case QImage::Format_RGB888: {
		// 8-bit, 3 channel (this is a common case). Requires cloning since we use a temporary QImage.
		
		QImage swapped = q_image.rgbSwapped();
		cv::UMat result_umat;
		cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone().copyTo(result_umat);
		
		return result_umat;
	}
	case QImage::Format_Indexed8: {
		// 8-bit, 1 channel
		
		cv::UMat result_umat;
		cv::Mat mat(q_image.height(), q_image.width(), CV_8UC1, const_cast<uchar*>(q_image.bits()), q_image.bytesPerLine());
		(clone_image_data ? mat.clone() : mat).copyTo(result_umat);

		cv::cvtColor(result_umat, result_umat, cv::COLOR_GRAY2BGR);
		return result_umat;
	}
	default: {
		qDebug() << "QImage format not handled in switch:" << q_image.format();
		break;
	}
	}
	return cv::UMat();
}