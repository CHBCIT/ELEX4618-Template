#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>

// OpenCV Library
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\ippicvmt.lib")
#ifdef _DEBUG
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\ittnotifyd.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\ippiwd.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\IlmImfd.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libtiffd.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libpngd.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libwebpd.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\zlibd.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libprotobufd.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libopenjp2d.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libjpeg-turbod.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\opencv_world450d.lib")
#else
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\ittnotify.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\ippiw.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\IlmImf.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libtiff.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libpng.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libwebp.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\zlib.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libprotobuf.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libopenjp2.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\libjpeg-turbo.lib")
#pragma comment(lib, ".\\opencv\\x64\\vc16\\staticlib\\opencv_world450.lib")
#endif



