#include<iostream>
#include<cmath>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


// Computes the x component of the gradient vector
// at a given point in a image.
// returns gradient in the x direction
int xGradient(Mat image, int x, int y)
{
    return  image.at<uchar>(y-1, x-1) +
            2*image.at<uchar>(y, x-1) +
            image.at<uchar>(y+1, x-1) -
            image.at<uchar>(y-1, x+1) -
            2*image.at<uchar>(y, x+1) -
            image.at<uchar>(y+1, x+1);
}

// Computes the y component of the gradient vector
// at a given point in a image
// returns gradient in the y direction

int yGradient(Mat image, int x, int y)
{
    return  image.at<uchar>(y-1, x-1) +
            2*image.at<uchar>(y-1, x) +
            image.at<uchar>(y-1, x+1) -
            image.at<uchar>(y+1, x-1) -
            2*image.at<uchar>(y+1, x) -
            image.at<uchar>(y+1, x+1);
}

Mat SovelEdgeX(Mat src)
{
    Mat dst = src.clone();
        for(int y = 0; y < src.rows; y++)
            for(int x = 0; x < src.cols; x++)
                dst.at<uchar>(y,x) = 0.0;

        for(int y = 1; y < src.rows - 1; y++){
            for(int x = 1; x < src.cols - 1; x++){
                int gx = xGradient(src, x, y);
                int sum = abs(gx);
                sum = sum > 100 ? 255:sum;
                sum = sum < 255 ? 0 : sum;
                dst.at<uchar>(y,x) = sum;
            }
        }
    return dst;
}


Mat FlipCenter(Mat src,uint8_t sideLR)
{
    namedWindow("src");
    imshow("src",src);
    Mat dst(src,Rect(0, 0, src.cols * 0.5, src.rows));
    namedWindow("show");
    imshow("show",dst);

    flip(dst,dst, 1); // 垂直軸で反転（水平反転）

    namedWindow("show2");
    imshow("show2",dst);
    return dst;
}

Mat ipm(Mat src)
{
    Mat conv(src,Rect(250,490,800,200));
    Mat dst = Mat::zeros(Size(400,400),CV_8UC1);


    Point2f img_vertices[4];
    img_vertices[0] = Point(250, 0);
    img_vertices[1] = Point(0, 170);
    img_vertices[2] = Point(800, 170);
    img_vertices[3] = Point(520, 0);

    Point2f world_vertices[4];
    world_vertices[0] = Point(0, 0);
    world_vertices[1] = Point(0, 400);
    world_vertices[2] = Point(400, 400);
    world_vertices[3] = Point(400, 0);

    Mat warpMatrix = getPerspectiveTransform(img_vertices, world_vertices);
    
    warpPerspective(conv, dst, warpMatrix, dst.size(), INTER_LINEAR);
    namedWindow("show");
    imshow("show",dst);

    return dst;
}


int main()
{
    Mat src, dst;

    // Load an image
    VideoCapture cap("Sample.mp4");
    //src = imread("road2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    //cap.set(CAP_PROP_POS_MSEC,16000);
    while(1){
    cap >> src;
    cvtColor(src, src, CV_BGR2GRAY);
    dst = src.clone();

    if( !src.data )
        { return -1; }

    dst = ipm(dst);
    namedWindow("initial");
    imshow("initial", dst);

    dst = SovelEdgeX(dst);
    //FlipCenter(src,0);
/*
    namedWindow("roi1");
    imshow("roi1", roi[0]);

    namedWindow("roi1e");
    imshow("roi1e", SovelEdgeX(roi[0]));

    namedWindow("roi2");
    imshow("roi2", roi[1]);

    namedWindow("roi2e");
    imshow("roi2e", SovelEdgeX(roi[1]));
*/
    namedWindow("final");
    imshow("final", dst);
	if(src.empty() || waitKey(30) >= 0 || cap.get(CV_CAP_PROP_POS_AVI_RATIO) == 1){
			break;
	    }
    }

    return 0;
}