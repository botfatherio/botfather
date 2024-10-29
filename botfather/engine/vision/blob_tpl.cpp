#include "blob_tpl.hpp"

BlobTpl::BlobTpl() {
    blob_params.filterByColor =
        true;  // Always filter by color. Color means black and white...
    blob_params.blobColor = 255;  // Look for white blobs on black background.

    // Default parameters taken from opencv sourcecode.
    // https://github.com/opencv/opencv/blob/master/modules/features2d/src/blobdetector.cpp

    // 0 <= minArea <= maxArea
    blob_params.filterByArea =
        true;  // Always filter by area. Not doing so makes no sense.
    blob_params.minArea = 25;
    blob_params.maxArea = 5000;

    // 0 <= minCircularity <= maxCircularity <= 1
    // > double ratio = 4 * CV_PI * area / (perimeter * perimeter);
    // > if (ratio < params.minCircularity || ratio >= params.maxCircularity)
    blob_params.filterByCircularity = false;
    blob_params.minCircularity = -1;
    blob_params.maxCircularity = -1;

    // 0 <= minConvexity <= maxConvexity <= 1
    blob_params.filterByConvexity = false;
    blob_params.minConvexity = -1;
    blob_params.maxConvexity = -1;

    // 0 <= minInertiaRatio <= maxInertiaRatio <= 1.
    blob_params.filterByInertia = false;
    blob_params.minInertiaRatio = -1;
    blob_params.maxInertiaRatio = -1;

    // 0 <= minThreshold <= maxThreshold <= 255.
    // Max threshold is 255 according to opencv blobdetector.cpp sourcecode.
    blob_params.minThreshold = 50;
    blob_params.maxThreshold = 220;
    blob_params.thresholdStep = 10;

    blob_params.minDistBetweenBlobs = 10;
    blob_params.minRepeatability = 2;
}

cv::SimpleBlobDetector::Params BlobTpl::getBlobParams() const {
    return blob_params;
}

void BlobTpl::setMinArea(int new_min_area) {
    blob_params.minArea = new_min_area;
}

int BlobTpl::getMinArea() const {
    return static_cast<int>(blob_params.minArea);
}

void BlobTpl::setMaxArea(int new_max_area) {
    blob_params.maxArea = new_max_area;
}

int BlobTpl::getMaxArea() const {
    return static_cast<int>(blob_params.maxArea);
}

void BlobTpl::setMinCircularity(double new_min_circularity) {
    blob_params.minCircularity = static_cast<float>(new_min_circularity);
    blob_params.filterByCircularity =
        blob_params.minCircularity >= 0 && blob_params.maxCircularity >= 0;
}

double BlobTpl::getMinCircularity() const {
    return static_cast<double>(blob_params.minCircularity);
}

void BlobTpl::setMaxCircularity(double new_max_circularity) {
    blob_params.maxCircularity = static_cast<float>(new_max_circularity);
    blob_params.filterByCircularity =
        blob_params.minCircularity >= 0 && blob_params.maxCircularity >= 0;
}

double BlobTpl::getMaxCircularity() const {
    return static_cast<double>(blob_params.maxCircularity);
}

void BlobTpl::setMinConvexity(double new_min_convexity) {
    blob_params.minConvexity = static_cast<float>(new_min_convexity);
    blob_params.filterByConvexity =
        blob_params.minConvexity >= 0 && blob_params.maxConvexity >= 0;
}

double BlobTpl::getMinConvexity() const {
    return static_cast<double>(blob_params.minConvexity);
}

void BlobTpl::setMaxConvexity(double new_max_convexity) {
    blob_params.maxConvexity = static_cast<float>(new_max_convexity);
    blob_params.filterByConvexity =
        blob_params.minConvexity >= 0 && blob_params.maxConvexity >= 0;
}

double BlobTpl::getMaxConvexity() const {
    return static_cast<double>(blob_params.maxConvexity);
}

void BlobTpl::setMinInertia(double new_min_inertia_ratio) {
    blob_params.minInertiaRatio = static_cast<float>(new_min_inertia_ratio);
    blob_params.filterByInertia =
        blob_params.minInertiaRatio >= 0 && blob_params.maxInertiaRatio >= 0;
}

double BlobTpl::getMinInertia() const {
    return static_cast<double>(blob_params.minInertiaRatio);
}

void BlobTpl::setMaxInertia(double new_max_inertia_ratio) {
    blob_params.maxInertiaRatio = static_cast<float>(new_max_inertia_ratio);
    blob_params.filterByInertia =
        blob_params.minInertiaRatio >= 0 && blob_params.maxInertiaRatio >= 0;
}

double BlobTpl::getMaxInertia() const {
    return static_cast<double>(blob_params.maxInertiaRatio);
}

void BlobTpl::setMinThreshold(int new_min_threshold) {
    blob_params.minThreshold = new_min_threshold;
}

int BlobTpl::getMinThreshold() const {
    return static_cast<int>(blob_params.minThreshold);
}

void BlobTpl::setMaxThreshold(int new_max_threshold) {
    blob_params.maxThreshold = new_max_threshold;
}

int BlobTpl::getMaxThreshold() const {
    return static_cast<int>(blob_params.maxThreshold);
}

void BlobTpl::setThresholdStep(int new_threshold_step) {
    blob_params.thresholdStep = static_cast<int>(new_threshold_step);
}

int BlobTpl::getThresholdStep() const {
    return static_cast<int>(blob_params.thresholdStep);
}

void BlobTpl::setMinBlobDistance(int new_min_blob_distance) {
    blob_params.minDistBetweenBlobs = new_min_blob_distance;
}

int BlobTpl::getMinBlobDistance() const {
    return static_cast<int>(blob_params.minDistBetweenBlobs);
}

void BlobTpl::setMinRepeatability(int new_min_repeatability) {
    blob_params.minRepeatability =
        static_cast<unsigned long>(new_min_repeatability);
}

int BlobTpl::getMinRepeatability() const {
    return static_cast<int>(blob_params.minRepeatability);
}