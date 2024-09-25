#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

// OpenCV Library
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\ippicvmt.lib")
#ifdef _DEBUG
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\ittnotifyd.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\ippiwd.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\IlmImfd.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libtiffd.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libpngd.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libwebpd.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\zlibd.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libprotobufd.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libopenjp2d.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libjpeg-turbod.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\opencv_world4100d.lib")
#else
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\ittnotify.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\ippiw.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\IlmImf.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libtiff.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libpng.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libwebp.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\zlib.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libprotobuf.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libopenjp2.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\libjpeg-turbo.lib")
#pragma comment(lib, "..\\opencv\\x64\\vc17\\staticlib\\opencv_world4100.lib")
#endif



