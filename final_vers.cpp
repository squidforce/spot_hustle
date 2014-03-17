#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main ( int argc, char **argv )
{
	cv::Mat img1, img2;
	// load images -- hopefully i've chosen the right data structure
	img1 = cv::imread("left2.png", 1);
	img2 = cv::imread("right2.png", 1);

	//-- create detector and descriptor --
	// if you want it faster, take e.g. FAST or ORB
	cv::Ptr<cv::FeatureDetector> detector = cv::FeatureDetector::create("HARRIS"); 
	// if you want it faster take e.g. ORB or FREAK
	cv::Ptr<cv::DescriptorExtractor> descriptor = cv::DescriptorExtractor::create("SIFT");
	// FLANN based matcher
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("FlannBased"); 

	// detect keypoints
	std::vector<cv::KeyPoint> keypoints1, keypoints2;
	detector->detect(img1, keypoints1);
	detector->detect(img2, keypoints2);

	// extract features
	cv::Mat desc1, desc2;
	descriptor->compute(img1, keypoints1, desc1);
	descriptor->compute(img2, keypoints2, desc2);

	// match features
	cv::vector<cv::DMatch> matches;
	matcher->match(desc1, desc2, matches);

  double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
  for( int i = 0; i < desc1.rows; i++ )
  { double dist = matches[i].distance;
    if( dist < min_dist ) min_dist = dist;
    if( dist > max_dist ) max_dist = dist;
  }

  printf("-- Max dist : %f \n", max_dist );
  printf("-- Min dist : %f \n", min_dist );

  //-- Draw only "good" matches (i.e. whose distance is less than 2*min_dist )
  //-- PS.- radiusMatch can also be used here.
  std::vector<cv::DMatch> good_matches;

  for( int i = 0; i < desc1.rows; i++ )
  { if( matches[i].distance < 2*min_dist )
    { good_matches.push_back( matches[i]); }
  }

  // find homography of good matches
  //-- Localize the object
  cv::vector<cv::Point2f> left;
  cv::vector<cv::Point2f> right;

  for( int i = 0; i < good_matches.size(); i++ )
  {
    //-- Get the keypoints from the good matches
    left.push_back( keypoints1[ good_matches[i].queryIdx ].pt );
    right.push_back( keypoints2[ good_matches[i].trainIdx ].pt );
  }

  cv::Mat H = cv::findHomography( right, left, CV_RANSAC );

  // warp img2 into trans_img to match img1
	cv::Size s = img2.size();
	cv::Mat trans_img;
  warpPerspective(img2, trans_img, H, s);

  cv::namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
  // cv::namedWindow( "Display window 2", CV_WINDOW_AUTOSIZE );
  // cv::imshow( "Display window", img1 );                   // Show our image inside it.
  // cv::imshow( "Display window 2", trans_img );

  cv::Mat result;
  cv::absdiff( img1, trans_img, result );

  // thresholding
  // cv::Mat result2;
  // cv::threshold( result, result2, 80, 255, CV_THRESH_BINARY );

  cv::imshow( "Display window", result );

  cv::waitKey(0);                                          // Wait for a keystroke in the window
  return 0;

}