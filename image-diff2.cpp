#include <cv.h>
#include <highgui.h>

int main ( int argc, char **argv )
{
	IplImage *img1;
	IplImage *img2;
	IplImage *imggray1;
	IplImage *imggray2;
	IplImage *imggray3;
	if(argc==3)
	{
		img1 = cvLoadImage( argv[1] );
		img2 = cvLoadImage( argv[2] );
	}
	else {exit(0);}

  // get the image size
  // IplImage *imgsize;
  // imgsize = cvQueryFrame( img1 );
  // if( !imgsize ) return -1;

	imggray1 = cvCreateImage( cvGetSize( img1 ), IPL_DEPTH_8U, 1);
	imggray2 = cvCreateImage( cvGetSize( img1 ), IPL_DEPTH_8U, 1);
	imggray3 = cvCreateImage( cvGetSize( img1 ), IPL_DEPTH_8U, 1);

	// convert rgb to grayscale
	cvCvtColor( img1, imggray1, CV_RGB2GRAY );

	// convert rgb to grayscale
	cvCvtColor( img2, imggray2, CV_RGB2GRAY );

	// compute difference
	cvAbsDiff( imggray1, imggray2, imggray3 );

	// display difference
	cvNamedWindow( "image", 1 );
	cvShowImage( "image", imggray3 );
	int key = 0;
	key = cvWaitKey( 10000 );

	// release camera and clean up resources when “q” is pressed
	cvReleaseImage( &img1 );
	cvReleaseImage( &img2 );
	cvDestroyWindow( "image" );
	return 0;
}