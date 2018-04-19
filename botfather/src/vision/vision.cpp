#include "vision.h"
#include <opencv2/highgui.hpp>
#include <QDebug>
#include "blob_tpl.h"
#include "match.h"

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
	static const int match_method = CV_TM_CCOEFF_NORMED;
	QVector<Match*> matches;
	
	if (image.empty()) {
		// TODO: Make the user aware of the image being empty
		qDebug() << Q_FUNC_INFO << "Image empty";
		return matches;
	}
	
	if (tpl.empty()) {
		// TODO: Make the user aware of the tpl being empty
		qDebug() << Q_FUNC_INFO << "Template empty";
		return matches;
	}
	
	if (image.rows <= tpl.rows || image.cols <= tpl.cols) {
		// TODO: Make the user aware of the image being smaller than the tpl.
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
	cv::matchTemplate(image, tpl, result, match_method);
		
	// Normalizing the result mats values results in better threshold values.
	// Better meaning: scores of good matching matches are not too spreaded.
	// eg. a template matches 4 spots really good. Scores where ~ 1., .99, .96, .94
	// without normalizing they were: ~ 1., .95, .82, 0.79
	// (Matching works without normalizing).
	cv::normalize( result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
		
	// Make areas completely black which are not as intensive as the threshold
	// requires. Looking for minmax values and using floodfill are faster after
	// thresholding.
	cv::threshold(result, result, threshold, 1.0, CV_THRESH_TOZERO);
	
	// Uncomment the following 2 lines of code to see where matches were found.
	// One may disabled thresholding to understand the result better.
	//cv::namedWindow("Result", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Result", result);
		
	while (matches.size() < max_matches){
		
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