#ifdef WIN32
#include "stdafx.h"
#endif
#include "OpenCVShow.h"

#include "iostream"
#include "fstream"
#define PI 3.14159265
COpenCVShow::COpenCVShow()
{
}


COpenCVShow::~COpenCVShow()
{
}

void COpenCVShow::showImage(unsigned char* pIm, double scale, string sWinname){
	m_mImg.create(m_nHeight, m_nWidth, CV_8UC1);
	m_mImg.data = pIm;
	imshow(sWinname, m_mImg*scale);
}

//void COpenCVShow::showImage(unsigned char* pIm, int width, int height, double scale, string sWinname, double sizeScale){
//	m_mImg.create(height, width, CV_8UC1);
//	m_mImg.data = pIm;
//	if (sizeScale != 1)
//		resize(m_mImg, m_mImg, Size((int)(sizeScale*m_mImg.rows), (int)(sizeScale*m_mImg.cols)));
//	imshow(sWinname, m_mImg*scale);
//};

void COpenCVShow::showImage(int nCode, string sWinname){
	string myFilename = m_sFilename + ".foo";
	//bool* val = new bool (m_nSize);
	m_mImg.create(m_nHeight, m_nWidth, CV_8UC1);
	fstream ifs(myFilename, ios::binary | ios::in);
	if (!ifs.is_open()){
		cout << "file failed to load\n" << endl;
		return;
	}

	ifs.read((char*)m_mImg.data, sizeof(unsigned char)*m_nSize);

	imshow(sWinname, m_mImg);
}

void COpenCVShow::showImage(unsigned char* pIm, int width, int height, double scale, string sWinname, double sizeScale, int _type){
	m_mImg.create(height, width, _type);
	m_mImg.data = pIm;
	if (sizeScale != 1)
		resize(m_mImg, m_mImg, Size(), sizeScale, sizeScale);
	imshow(sWinname, m_mImg*scale);
};

void COpenCVShow::showImage(std::vector<unsigned char>& vInput, int width, int height, double scale, string sWinname) {
	m_mImg.create(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			m_mImg.at<uchar>(i, j) = vInput[i*width + j];
	imshow(sWinname, m_mImg*scale);
};

void COpenCVShow::showImage(std::vector<unsigned char>& vInput, int width, int height, double scale, Point p1, Point p2, string sWinname) {
	m_mImg.create(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			m_mImg.at<uchar>(i, j) = vInput[i*width + j];
	//line(m_mImg, p1, p2, Scalar(0, 0, 0));
	m_mImg *= scale;
	imshow(sWinname, m_mImg);

}

void COpenCVShow::showImage(std::vector<int>& vInput, int width, int height, double scale, string sWinname) {
	m_mImg.create(height, width, CV_8UC1);
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			m_mImg.at<uchar>(i, j) = vInput[i*width + j];
	imshow(sWinname, m_mImg*scale);
};

Mat COpenCVShow::showFinal(std::vector< std::vector<int> >& vMinutiae, string sfilename){

	double scale = 1.5;
	int w = int(m_mInput.cols*scale);
	int h = int(m_mInput.rows*scale);

	resize(m_mInput, m_mInput, Size(w, h));
	resize(m_mBG, m_mBG, Size(w, h));
	//m_mBG2 = m_mBG.clone();
	resize(m_mThinned, m_mThinned, Size(w, h));
	resize(m_mEnhanced, m_mEnhanced, Size(w, h));

	m_mThinned = (255 - m_mThinned * 255);


	m_mImg.create(h, w * 3, CV_8UC1);
	m_mInput.copyTo(m_mImg(Range(0, h), Range(0, w)));
	m_mEnhanced.copyTo(m_mImg(Range(0, h), Range(w, 2 * w)));
	m_mThinned.copyTo(m_mImg(Range(0, h), Range(2 * w, 3 * w)));


	cvtColor(m_mImg, m_mImg, COLOR_GRAY2RGB);
	cvtColor(m_mInput, m_mInput, COLOR_GRAY2RGB);

	vector<vector<Point> > contours, contours2;
	vector<Vec4i> hierarchy, hierarchy2;
	/// Find contours
	findContours(m_mBG, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	for (unsigned int i = 0; i < (contours.size()); i++)
	{
		Scalar color = Scalar(0, 255, 0);
		drawContours(m_mImg, contours, i, color, 1, 8, hierarchy, 0, Point());
	}

	int x, y, type, quality;
	float theta;
	int len = 7;
	for (unsigned int i = 0; i < unsigned(vMinutiae.size()); i++){
		x = (int)(vMinutiae[i][0] * scale);
		y = (int)(vMinutiae[i][1] * scale);
		theta = (float)vMinutiae[i][2];
		type = vMinutiae[i][3];
		quality = vMinutiae[i][4];
		if (type == 0)
			continue;
		//if ((type == 1) || (type == 3))			putText(m_mImg, to_string(quality), Point(x-5, y-5), FONT_HERSHEY_SCRIPT_SIMPLEX, 0.3, Scalar(0, 255, 0), 1, 8);
		int newx, newy;
		switch (type)
		{
		case 10:		//CORE
			circle(m_mImg, Point(x, y), 5, Scalar(0, 0, 255),1);
			//circle(m_mImg, Point(x + w, y), 3, Scalar(230, 80, 237));
			//circle(m_mImg, Point(x + 2 * w, y), 3, Scalar(230, 80, 237));
			break;
		case 11:		//CORE
			circle(m_mImg, Point(x, y), 5, Scalar(255, 0, 0), 1);
			//circle(m_mImg, Point(x + w, y), 3, Scalar(230, 80, 237));
			//circle(m_mImg, Point(x + 2 * w, y), 3, Scalar(230, 80, 237));
			break;
		case 1:		//ENDING
			circle(m_mImg, Point(x, y), 3, Scalar(230, 80, 237));
			newx = (int)(x + 10 * cos(-(float)((255 - theta) / 255 * 360 / 180 * PI) - PI / 2));
			newy = (int)(y + 10 * sin(-(float)((255 - theta) / 255 * 360 / 180 * PI) - PI / 2));
			arrowedLine(m_mImg, Point(x, y), Point(newx, newy), Scalar(230, 80, 237));
			circle(m_mImg, Point(x + w, y), 3, Scalar(230, 80, 237));
			circle(m_mImg, Point(x + 2 * w, y), 3, Scalar(230, 80, 237));
			break;
		case 3:		//BIFURCATION
			circle(m_mImg, Point(x, y), 3, Scalar(0, 255, 255));
			newx = (int)(x + 10 * cos(-(float)((255 - theta) / 255 * 360 / 180 * PI) - PI / 2));
			newy = (int)(y + 10 * sin(-(float)((255 - theta) / 255 * 360 / 180 * PI) - PI / 2));
			arrowedLine(m_mImg, Point(x, y), Point(newx, newy), Scalar(0, 255, 255));
			circle(m_mImg, Point(x + w, y), 3, Scalar(0, 255, 255));
			circle(m_mImg, Point(x + 2 * w, y), 3, Scalar(0, 255, 255));
			break;

		case -1:		// bad quality region
			circle(m_mImg, Point(x, y), 3, Scalar(255, 255, 255));
			circle(m_mImg, Point(x + w, y), 3, Scalar(255, 255, 255));
			circle(m_mImg, Point(x + 2 * w, y), 3, Scalar(255, 255, 255));
			break;
		case -2:		// false removal
			circle(m_mImg, Point(x, y), 3, Scalar(255, 255, 0));
			newx = (int)(x - 10 * cos(-(float)((255 - theta) / 255 * 360 / 180 * PI) - PI / 2));
			newy = (int)(y - 10 * sin(-(float)((255 - theta) / 255 * 360 / 180 * PI) - PI / 2));
			arrowedLine(m_mImg, Point(x, y), Point(newx, newy), Scalar(255, 255, 0));
			circle(m_mImg, Point(x + w, y), 3, Scalar(255, 255, 0));
			circle(m_mImg, Point(x + 2 * w, y), 3, Scalar(255, 255, 0));
			break;

		case -3:		// angle removal
			circle(m_mImg, Point(x, y), 3, Scalar(255, 255, 255));
			newx = (int)(x - 10 * cos(-(float)((255 - theta) / 255 * 360 / 180 * PI) - PI / 2));
			newy = (int)(y - 10 * sin(-(float)((255 - theta) / 255 * 360 / 180 * PI) - PI / 2));
			arrowedLine(m_mImg, Point(x, y), Point(newx, newy), Scalar(255, 255, 255));
			circle(m_mImg, Point(x + w, y), 3, Scalar(255, 255, 255));
			circle(m_mImg, Point(x + 2 * w, y), 3, Scalar(255, 255, 255));
			break;
		}
	}

	m_mImg(Range(0, h), Range(0, w)).copyTo(m_mInput);

	//save the image
	int nIdx = sfilename.find_last_of("\\");
	string sPathname = sfilename.substr(0, nIdx+1);
	sPathname += "Result";

	//string saved_fname = sPathname + sfilename.substr(nIdx, sfilename.length()) + ".bmp";
	string saved_fname = sPathname + sfilename.substr(nIdx, sfilename.length());

	imwrite(saved_fname, m_mImg);

	//string stemp;
	//stemp = saved_fname;
	//stemp.insert(stemp.length() - 4, "_ENH");
	//imwrite(stemp, m_mEnhanced);
	//stemp = saved_fname;
	//stemp.insert(stemp.length() - 4, "_THI");
	//imwrite(stemp, m_mThinned);
	//stemp = saved_fname;
	//stemp.insert(stemp.length() - 4, "_BGR");
	//imwrite(stemp, 255*m_mBG2);

	saved_fname = "_result.bmp";

	imwrite(saved_fname, m_mInput);

	return m_mImg;
}

void COpenCVShow::saveOrientation(std::vector< std::vector<int> >& vMinutiae, std::vector<float>& vData, int h, int w, string filename) {
	int nIdx = filename.find_last_of("\\");
	string sPathname = filename.substr(0, nIdx + 1);
	sPathname += "Result";

	//string saved_fname = sPathname + sfilename.substr(nIdx, sfilename.length()) + ".bmp";
	string saved_fname = sPathname + filename.substr(nIdx, filename.length());

	string stemp;
	stemp = saved_fname.substr(0, saved_fname.length() - 4);
	stemp.insert(stemp.length(), "_ORI_SIN.txt");
	ofstream of;
	of.open(stemp);
	of << h << endl;
	of << w << endl;
	for (int i = 0; i < (int)vData.size(); i++)
		of << vData[i] << endl;

	for (unsigned int j = 0; j < vMinutiae.size(); j++) {
		int x = vMinutiae[j][0];
		int y = vMinutiae[j][1];
		int ntype = vMinutiae[j][3];

		if (ntype > 9) {
			of << x << "	" << y << "	" << ntype << endl;
		}
			
	}
	of.close();
}

void COpenCVShow::saveRAW2BMP(unsigned char* pIm, int width, int height, string szfilename){
	m_mImg.create(height, width, CV_8UC1);
	m_mImg.data = pIm;
	imwrite(szfilename, m_mImg);
}

void drawFGContour(std::vector<int>& fgmask, int width_, int height_){

}

void COpenCVShow::resizeandsave(string sfilename){
	Mat im = imread(sfilename);

	resize(im, im, Size(704, 240));
	string saved_fname = sfilename;
	int len = saved_fname.length();
	//saved_fname.insert(len - 4, "_");
	imwrite(sfilename, im);
}

void COpenCVShow::addInputImage(unsigned char* pIm, int width, int height){
	m_mImg.create(height, width, CV_8UC1);
	m_mImg.data = pIm;
	m_mInput = m_mImg.clone();
};

void COpenCVShow::addBGImage(unsigned char* pIm, int width, int height){
	m_mImg.create(height, width, CV_8UC1);
	m_mImg.data = pIm;
	m_mBG = m_mImg.clone();

};

void COpenCVShow::addBGImage2(unsigned char* pIm, int width, int height){
	m_mImg.create(height, width, CV_8UC1);
	m_mImg.data = pIm;
	m_mBG2 = m_mImg.clone();
};


void COpenCVShow::addEnhancedImage(unsigned char* pIm, int width, int height){
	m_mImg.create(height, width, CV_8UC1);
	m_mImg.data = pIm;
	m_mEnhanced = m_mImg.clone();
};

void COpenCVShow::addThinImage(unsigned char* pIm, int width, int height){
	m_mImg.create(height, width, CV_8UC1);
	m_mImg.data = pIm;
	m_mThinned = m_mImg.clone();
};

void COpenCVShow::estimateQualityMap(){
	Mat mTemp;
	GaussianBlur(m_mInput, mTemp, Size(5, 5), 1.);
	mTemp.convertTo(mTemp, CV_32F);
	double vmin, vmax;
	minMaxLoc(mTemp, &vmin, &vmax);
	mTemp = (mTemp - vmin)/(vmax - vmin);

	minMaxLoc(mTemp, &vmin, &vmax);


	Mat kernelX = (Mat_<double>(1, 3) << -1, 0, 1);
	Mat kernelY = (Mat_<double>(3, 1) << -1, 0, 1);
	Mat Gx, Gy, Gxx, Gyy, Gxy;
	filter2D(mTemp, Gx, CV_32F, kernelX);
	filter2D(mTemp, Gy, CV_32F, kernelY);
	Gxx = Gx.mul(Gx);
	Gyy = Gy.mul(Gy);
	Gxy = Gx.mul(Gy);
	Rect winrec(0, 0, 6, 6);

	int nRows = (int)floor(m_mInput.rows / winrec.height);
	int nCols = (int)floor(m_mInput.cols / winrec.width);

	winrec.x = winrec.y=100;
	Mat mMat = mTemp(winrec).clone();


	Mat mCohenrence(mTemp.size(),CV_32FC1);
	mCohenrence.setTo(0);
	for (int i = 0; i < mTemp.rows-winrec.height; i++)
		for (int j = 0; j < mTemp.cols-winrec.width; j++){
			winrec.y = i;
			winrec.x = j;
			double a = sum(Gxx(winrec)).val[0];
			double b = sum(Gyy(winrec)).val[0];
			double c = sum(Gxy(winrec)).val[0];
			double lamdaMax = ((a + b) + sqrt((a - b)*(a - b) + 4 * c*c)) / 2;
			double lamdaMin = ((a + b) - sqrt((a - b)*(a - b) + 4 * c*c)) / 2;
			Scalar scMean, scSTDev;
			meanStdDev(mTemp(winrec), scMean, scSTDev);
			minMaxLoc(mTemp(winrec), &vmin, &vmax);
			//mCohenrence.at<float>(i+winrec.height/2, j+winrec.width/2) = (lamdaMax - lamdaMin) / (lamdaMax + lamdaMin);
			//mCohenrence.at<float>(i + winrec.height / 2, j + winrec.width / 2) = scSTDev.val[0];
			mCohenrence.at<float>(i + winrec.height / 2, j + winrec.width / 2) = (float)(vmax - vmin);
		}


	//mCohenrence = mCohenrence > 0.5;
	mCohenrence = mCohenrence > 0.15;
	resize(mCohenrence, mCohenrence, m_mBG.size());
	mCohenrence = mCohenrence > 0;
	bitwise_and(mCohenrence, m_mBG, mCohenrence);
	resize(mCohenrence, mCohenrence, m_mInput.size());
	mCohenrence = mCohenrence > 0;
	imshow("Coherence", 255*mCohenrence);
	//cvWaitKey(0);
}

