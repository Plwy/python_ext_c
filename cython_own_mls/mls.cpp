#include "mls.h"
#include <cstdio>
#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"

using std::vector;
using cv::Mat;
using cv::Mat_;
using cv::Point_;
using cv::Point;
using cv::Vec3b;

using namespace std;
using namespace cv;

///////////////////////////////////////////////////////////////////////////////////////////////////

class ImgWarp_MLS {
   public:
    ImgWarp_MLS();
    virtual ~ImgWarp_MLS() {}

    Mat setAllAndGenerate(const Mat &oriImg, const vector<Point_<int> > &qsrc,
                          const vector<Point_<int> > &qdst, const int outW,
                          const int outH, const double transRatio = 1);

    Mat genNewImg(const Mat &oriImg, double transRatio);

    virtual void calcDelta() = 0;

    double alpha;

    int gridSize;

    void setDstPoints(const vector<Point_<int> > &qdst);

    void setSrcPoints(const vector<Point_<int> > &qsrc);

    void setSize(int w, int h) { srcW = w, srcH = h; }

    void setTargetSize(const int outW, const int outH) {
        tarW = outW;
        tarH = outH;
    }

   protected:
    vector<Point_<double> > oldDotL, newDotL;

    int nPoint;

    Mat_<double> /*! \brief delta_x */ rDx, /*! \brief delta_y */ rDy;

    int srcW, srcH;
    int tarW, tarH;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

ImgWarp_MLS::ImgWarp_MLS() { gridSize = 5; }

inline double bilinear_interp(double x, double y, double v11, double v12,
                              double v21, double v22) {
    return (v11 * (1 - y) + v12 * y) * (1 - x) + (v21 * (1 - y) + v22 * y) * x;
}

Mat ImgWarp_MLS::setAllAndGenerate(const Mat &oriImg,
                                   const vector<Point_<int> > &qsrc,
                                   const vector<Point_<int> > &qdst,
                                   const int outW, const int outH,
                                   const double transRatio) {
    setSize(oriImg.cols, oriImg.rows);
    setTargetSize(outW, outH);
    setSrcPoints(qsrc);
    setDstPoints(qdst);
    calcDelta();
    return genNewImg(oriImg, transRatio);
}

Mat ImgWarp_MLS::genNewImg(const Mat &oriImg, double transRatio) {
    int i, j;
    double di, dj;
    double nx, ny;
    int nxi, nyi, nxi1, nyi1;
    double deltaX, deltaY;
    double w, h;
    int ni, nj;

    Mat newImg(tarH, tarW, oriImg.type());
    for (i = 0; i < tarH; i += gridSize)
        for (j = 0; j < tarW; j += gridSize) {
            ni = i + gridSize, nj = j + gridSize;
            w = h = gridSize;
            if (ni >= tarH) ni = tarH - 1, h = ni - i + 1;
            if (nj >= tarW) nj = tarW - 1, w = nj - j + 1;
            for (di = 0; di < h; di++)
                for (dj = 0; dj < w; dj++) {
                    deltaX =
                        bilinear_interp(di / h, dj / w, rDx(i, j), rDx(i, nj),
                                        rDx(ni, j), rDx(ni, nj));
                    deltaY =
                        bilinear_interp(di / h, dj / w, rDy(i, j), rDy(i, nj),
                                        rDy(ni, j), rDy(ni, nj));
                    nx = j + dj + deltaX * transRatio;
                    ny = i + di + deltaY * transRatio;
                    if (nx > srcW - 1) nx = srcW - 1;
                    if (ny > srcH - 1) ny = srcH - 1;
                    if (nx < 0) nx = 0;
                    if (ny < 0) ny = 0;
                    nxi = int(nx);
                    nyi = int(ny);
                    nxi1 = ceil(nx);
                    nyi1 = ceil(ny);

                    if (oriImg.channels() == 1)
                        newImg.at<uchar>(i + di, j + dj) = bilinear_interp(
                            ny - nyi, nx - nxi, oriImg.at<uchar>(nyi, nxi),
                            oriImg.at<uchar>(nyi, nxi1),
                            oriImg.at<uchar>(nyi1, nxi),
                            oriImg.at<uchar>(nyi1, nxi1));
                    else {
                        for (int ll = 0; ll < 3; ll++)
                            newImg.at<Vec3b>(i + di, j + dj)[ll] =
                                bilinear_interp(
                                    ny - nyi, nx - nxi,
                                    oriImg.at<Vec3b>(nyi, nxi)[ll],
                                    oriImg.at<Vec3b>(nyi, nxi1)[ll],
                                    oriImg.at<Vec3b>(nyi1, nxi)[ll],
                                    oriImg.at<Vec3b>(nyi1, nxi1)[ll]);
                    }
                }
        }
    return newImg;
}

void ImgWarp_MLS::setSrcPoints(const vector<Point_<int> > &qsrc) {
    nPoint = qsrc.size();

    newDotL.clear();
    newDotL.reserve(nPoint);

    for (size_t i = 0; i < qsrc.size(); i++) newDotL.push_back(qsrc[i]);
}

void ImgWarp_MLS::setDstPoints(const vector<Point_<int> > &qdst) {
    nPoint = qdst.size();
    oldDotL.clear();
    oldDotL.reserve(nPoint);

    for (size_t i = 0; i < qdst.size(); i++) oldDotL.push_back(qdst[i]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

class ImgWarp_MLS_Rigid : public ImgWarp_MLS
{
public:

    bool preScale;

    ImgWarp_MLS_Rigid();
    void calcDelta();
};

///////////////////////////////////////////////////////////////////////////////////////////////////

ImgWarp_MLS_Rigid::ImgWarp_MLS_Rigid() { preScale = false; }

double calcArea(const vector<Point_<double> > &V) {
    Point_<double> lt, rb;
    lt.x = lt.y = 1e10;
    rb.x = rb.y = -1e10;
    for (vector<Point_<double> >::const_iterator i = V.begin(); i != V.end();
         i++) {
        if (i->x < lt.x) lt.x = i->x;
        if (i->x > rb.x) rb.x = i->x;
        if (i->y < lt.y) lt.y = i->y;
        if (i->y > rb.y) rb.y = i->y;
    }
    return (rb.x - lt.x) * (rb.y - lt.y);
}

void ImgWarp_MLS_Rigid::calcDelta() {
    int i, j, k;

    Point_<double> swq, qstar, newP, tmpP;
    double sw;

    double ratio = 0;

    if (preScale) {
        ratio = sqrt(calcArea(newDotL) / calcArea(oldDotL));
        for (i = 0; i < nPoint; i++) newDotL[i] *= 1 / ratio;
    }

    rDx.create(tarH, tarW);
    rDy.create(tarH, tarW);

    if (nPoint < 2) {
        rDx.setTo(0);
        rDy.setTo(0);
        return;
    }
    Point_<double> swp, pstar, curV, curVJ, Pi, PiJ, Qi;
    double miu_r;
    std::vector<double> w(nPoint);

    for (i = 0;; i += gridSize) {
        if (i >= tarW && i < tarW + gridSize - 1)
            i = tarW - 1;
        else if (i >= tarW)
            break;
        for (j = 0;; j += gridSize) {
            if (j >= tarH && j < tarH + gridSize - 1)
                j = tarH - 1;
            else if (j >= tarH)
                break;
            sw = 0;
            swp.x = swp.y = 0;
            swq.x = swq.y = 0;
            newP.x = newP.y = 0;
            curV.x = i;
            curV.y = j;
            for (k = 0; k < nPoint; k++) {
                if ((i == oldDotL[k].x) && j == oldDotL[k].y) break;
                if (alpha == 1)
                    w[k] = 1 / ((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                (j - oldDotL[k].y) * (j - oldDotL[k].y));
                else
                    w[k] = pow((i - oldDotL[k].x) * (i - oldDotL[k].x) +
                                   (j - oldDotL[k].y) * (j - oldDotL[k].y),
                               -alpha);
                sw = sw + w[k];
                swp = swp + w[k] * oldDotL[k];
                swq = swq + w[k] * newDotL[k];
            }
            if (k == nPoint) {
                pstar = (1 / sw) * swp;
                qstar = 1 / sw * swq;

                // Calc miu_r
                double s1 = 0, s2 = 0;
                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;

                    Pi = oldDotL[k] - pstar;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;
                    Qi = newDotL[k] - qstar;
                    s1 += w[k] * Qi.dot(Pi);
                    s2 += w[k] * Qi.dot(PiJ);
                }
                miu_r = sqrt(s1 * s1 + s2 * s2);

                curV -= pstar;
                curVJ.x = -curV.y, curVJ.y = curV.x;

                for (k = 0; k < nPoint; k++) {
                    if (i == oldDotL[k].x && j == oldDotL[k].y) continue;

                    Pi = oldDotL[k] - pstar;
                    PiJ.x = -Pi.y, PiJ.y = Pi.x;

                    tmpP.x = Pi.dot(curV) * newDotL[k].x -
                             PiJ.dot(curV) * newDotL[k].y;
                    tmpP.y = -Pi.dot(curVJ) * newDotL[k].x +
                             PiJ.dot(curVJ) * newDotL[k].y;
                    tmpP *= w[k] / miu_r;
                    newP += tmpP;
                }
                newP += qstar;
            } else {
                newP = newDotL[k];
            }

            if (preScale) {
                rDx(j, i) = newP.x * ratio - i;
                rDy(j, i) = newP.y * ratio - j;
            } else {
                rDx(j, i) = newP.x - i;
                rDy(j, i) = newP.y - j;
            }
        }
    }

    if (preScale) {
        for (i = 0; i < nPoint; i++) newDotL[i] *= ratio;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void mls_deformation(int height, int width, uchar *oriImg,  uchar *dstImg, int *x1, int *y1, int n1, int *x2, int *y2)
{

    // std::cout << "height: " << height << "width: " << width << std::endl;
    // std::cout << "n: " << n1 << std::endl;
    std::vector<cv::Point2i> vL;
    std::vector<cv::Point2i> vQ;
    for(int i = 0; i < n1*2; i+=2){
	    vL.push_back(cv::Point2i(x1[i], y1[i]));
	    vQ.push_back(cv::Point2i(x2[i], y2[i]));
	}
    
    int count = 0;
    cv::Mat image(height, width, CV_8UC3);
    uchar* pxvec =image.ptr<uchar>(0);
    for(int row = 0; row < height; row++)
    {
        pxvec = image.ptr<uchar>(row);
        for(int col = 0; col < width; col++)
        {
            for(int c = 0; c < 3; c++)
            {
                pxvec[col*3+c] = oriImg[count];
                count++;
            }
        }
    }

    cv::Mat curImg = image;
    ImgWarp_MLS * imgTrans;
    imgTrans = new ImgWarp_MLS_Rigid();   
    imgTrans->alpha = 1.0;
    imgTrans->gridSize = 10;
    curImg = imgTrans->setAllAndGenerate(image, vL, vQ, image.cols, image.rows, 1.0);

    count = 0;
    pxvec = curImg.ptr<uchar>(0);
    for(int row = 0; row < height; row++)
    {
        pxvec = curImg.ptr<uchar>(row);
        for(int col = 0; col < width; col++)
        {
            for(int c = 0; c < 3; c++)
            {
                dstImg[count] = pxvec[col*3+c];
                count++;
            }
        }
    }


    delete imgTrans;
    return;
}
