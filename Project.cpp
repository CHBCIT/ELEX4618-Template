////////////////////////////////////////////////
// Added a header comment
////////////////////////////////////////////////
#include "stdafx.h"

#include "cv.h"
#include "highgui.h"

#include "Serial.h"

using namespace std;
using namespace cv;

////////////////////////////////////////////////////////////////
// Serial Communication
////////////////////////////////////////////////////////////////
void test_com()
{
	string comport = "\\\\.\\COM7";
  Serial com(comport);
	
  DWORD sent;
  char sBuf[] = "G 1 0\n";

  char buff[2];
  do
  {
		com.write(sBuf, sizeof(sBuf));
    com.flush();
    printf("\n");
    Sleep(100);
  
		// If 1 byte was read then print to screen
    while (com.read(buff, 1, FALSE) > 0)
    {
      printf("%c", buff[0]);
    }
  }
  while (1);
}

////////////////////////////////////////////////////////////////
// Display Image on screen
////////////////////////////////////////////////////////////////
void do_image()
{
  namedWindow("Image");

  Mat im=Mat::zeros(1000,1000,CV_8UC1);
  //im = imread("image.jpg");

  for (int i = 0; i < 500; i++)
  {
    im.at<char>(i,i) = 255;
    imshow("Image", im);
    cvWaitKey(10);
  }
}

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
  VideoCapture vid;
  Mat imvid;

  namedWindow("Video");

  vid.open(0);

  if (vid.isOpened() == TRUE)
  {
    do
    {
      vid.read(imvid);
      if (!imvid.empty())
      {
        imshow ("Video",imvid);
      }
    }
    while (cvWaitKey(10) != 'q');
  }      
}		
    
int _tmain(int argc, _TCHAR* argv[])
{
	test_com();
	//do_image();
	//do_video ();
}

