#include "opencv2/imgproc/imgproc.hpp"
#include "sobel_alg.h"
using namespace cv;

/*******************************************
 * Model: grayScale
 * Input: Mat img
 * Output: None directly. Modifies a ref parameter img_gray_out
 * Desc: This module converts the image to grayscale
********************************************/
void grayScale(Mat& img, Mat& img_gray_out)
{

  // Convert to grayscale

  // give loop size locally
  const int rows = img.rows;
  const int cols = img.cols;

  // cast Mat.data to uchar*
  uchar* img_ptr = img.data;
  uchar* img_gray_out_ptr = img_gray_out.data;

  //double color;
  for (int i=0; i<rows; i++) {
    for (int j=0; j<cols; j++) {
      img_gray_out_ptr[IMG_WIDTH*i + j] = 
              (114*img_ptr[STEP0*i + STEP1*j] +
              587*img_ptr[STEP0*i + STEP1*j + 1] +
              299*img_ptr[STEP0*i + STEP1*j + 2]) >> 10;
      //img_gray_out_ptr[IMG_WIDTH*i + j] = color;
    }
  }

  /*
  double color;
  for (int i=0; i<rows; i++) {
    for (int j=0; j<cols; j++) {
      color = .114*img.data[STEP0*i + STEP1*j] +
              .587*img.data[STEP0*i + STEP1*j + 1] +
              .299*img.data[STEP0*i + STEP1*j + 2];
      img_gray_out.data[IMG_WIDTH*i + j] = color;
    }
  }
  */
}

/*******************************************
 * Model: sobelCalc
 * Input: Mat img_in
 * Output: None directly. Modifies a ref parameter img_sobel_out
 * Desc: This module performs a sobel calculation on an image. It first
 *  converts the image to grayscale, calculates the gradient in the x
 *  direction, calculates the gradient in the y direction and sum it with Gx
 *  to finish the Sobel calculation
 ********************************************/

void sobelCalc(Mat& img_gray, Mat& img_sobel_out)
{
  Mat img_outx = img_gray.clone();
  Mat img_outy = img_gray.clone();

  // Apply Sobel filter to black & white image
  unsigned short sobel;


  const int rows = img_gray.rows;
  const int cols = img_gray.cols;

  uchar* img_ptr = img_gray.data;
  uchar* outx_ptr = img_outx.data;
  uchar* outy_ptr = img_outy.data;
  uchar* out_ptr = img_sobel_out.data;

  // Calculate the x convolution
  for (int i=1; i<rows; i++) {
    for (int j=1; j<cols; j++) {
      sobel = abs(img_ptr[IMG_WIDTH*(i-1) + (j-1)] -
		  img_ptr[IMG_WIDTH*(i+1) + (j-1)] +
		  2*img_ptr[IMG_WIDTH*(i-1) + (j)] -
		  2*img_ptr[IMG_WIDTH*(i+1) + (j)] +
		  img_ptr[IMG_WIDTH*(i-1) + (j+1)] -
		  img_ptr[IMG_WIDTH*(i+1) + (j+1)]);

      sobel = (sobel > 255) ? 255 : sobel;
      outx_ptr[IMG_WIDTH*(i) + (j)] = sobel;
    }
  }

  // Calc the y convolution
  for (int i=1; i<rows; i++) {
    for (int j=1; j<cols; j++) {
     sobel = abs(img_ptr[IMG_WIDTH*(i-1) + (j-1)] -
		   img_ptr[IMG_WIDTH*(i-1) + (j+1)] +
		   2*img_ptr[IMG_WIDTH*(i) + (j-1)] -
		   2*img_ptr[IMG_WIDTH*(i) + (j+1)] +
		   img_ptr[IMG_WIDTH*(i+1) + (j-1)] -
		   img_ptr[IMG_WIDTH*(i+1) + (j+1)]);

     sobel = (sobel > 255) ? 255 : sobel;

     outy_ptr[IMG_WIDTH*(i) + j] = sobel;
    }
  }

  // Combine the two convolutions into the output image
  for (int i=1; i<rows; i++) {
    for (int j=1; j<cols; j++) {
      sobel = outx_ptr[IMG_WIDTH*(i) + j] +
	outy_ptr[IMG_WIDTH*(i) + j];
      sobel = (sobel > 255) ? 255 : sobel;
      out_ptr[IMG_WIDTH*(i) + j] = sobel;
    }
  }
}

