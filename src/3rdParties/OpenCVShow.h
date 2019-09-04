#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <string>
#include <stdio.h>
#include <stdlib.h>
//#include "WTypes.h"

using namespace cv;
using namespace std;

enum{
	IMG_BG,
	IMG_ENHANCED,
	IMG_MINUTIA
};
class COpenCVShow
{
public:
	COpenCVShow();
	~COpenCVShow();

	int m_nWidth;
	int m_nHeight;
	string m_sFilename;
	int m_nSize;
	Mat   m_mImg;
	Mat m_mInput;
	Mat m_mBG;
	Mat m_mBG2;
	Mat m_mEnhanced;
	Mat m_mThinned;

	inline void init(int nWidth, int nHeight, string sFilename = ""){ m_nWidth = nWidth; m_nHeight = nHeight; m_sFilename = sFilename; 
	m_nSize = m_nWidth*m_nHeight;
	};
	void showImage(unsigned char* pIm, double scale = 1, string sWinname = "Window");
	void showImage(unsigned char* pIm, int width, int height, double scale = 1, string sWinname = "Window", double sizeScale = 2, int _type = CV_8UC1);
	void showImage(std::vector<int>& vInput, int width, int height, double scale = 1, string sWinname = "Window");
	void showImage(std::vector<unsigned char>& vInput, int width, int height, double scale, Point p1, Point p2, string sWinname = "Window");
	void showImage(std::vector<unsigned char>& vInput, int width, int height, double scale = 1, string sWinname = "Window");
	//void showImage(unsigned char* pIm, int width, int height, double scale, string sWinname, double sizeScale);
	void showImage(int nCode, string sWinname = "Window");
	Mat showFinal(std::vector< std::vector<int> >& vMinutiae, string sfilename);
	void addInputImage(unsigned char* pImint, int width, int height);
	void addBGImage(unsigned char* pIm, int width, int height);
	void addBGImage2(unsigned char* pIm, int width, int height);
	void addEnhancedImage(unsigned char* pIm, int width, int height);
	void addThinImage(unsigned char* pIm, int width, int height);
	Mat getResult(){ return m_mImg; };
	void saveOrientation(std::vector< std::vector<int> >& vMinutiae, std::vector<float>& vData, int h, int w, string filename);
	void estimateQualityMap();
	void resizeandsave( string sfilename);
	void saveRAW2BMP(unsigned char* pIm, int width, int height, string szfilename);
};

