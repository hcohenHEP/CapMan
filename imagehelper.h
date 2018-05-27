#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H
#include <TASImage.h>
#include <QPair>
#include <TTree.h>
#include <TH2D.h>
#include <QList>
#include <TFile.h>
#include <QString>
#include <math.h>
#include <iostream>
#include <numeric>
#include <QMap>
#include <TH1D.h>
#include <QList>
#include "RootStaticDisplayInterface.h"
#define XB 1000
#define YB 1000
#define R ((double) rand() / (RAND_MAX))
#define RDIRECTIONS ((int)(rand() % 16))
#define R4DIRECTIONS ((int)(rand() % 4))
#define R8DIRECTIONS ((int)(rand() % 8))

#define SQ2 sqrt(static_cast<double>(2))

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,


    DOWN_RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,


    UP_LEFT_LEFT,
    UP_UP_LEFT,
    UP_RIGHT_RIGHT,
    UP_UP_RIGHT,
    DOWN_LEFT_LEFT,
    DOWN_DOWN_LEFT,
    DOWN_RIGHT_RIGHT,
    DOWN_DOWN_RIGHT
};


class ImageHelper
{
public:
    static ImageHelper& Instance()
    {
        static ImageHelper    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    void AdvanceInDirection(int *x, int *y, Direction direction);
    Float_t FindNearestBlack(TH2D* hist,int x,int y, Direction direction, int threshold, int* retX, int* retY);

    Float_t FindNeighborsDensity(int x, int y,int radious, bool isBig);
    Float_t FindNeighborsIntensity(int x, int y, int radious);
    Float_t FindAvgMinDistance(int x, int y,int radious, bool isBig);
    Float_t FindAvgMaxDistance(int x, int y,int radious, bool isBig);
    Double_t MaxValue(const char* fileName);
    float RandomWalkSteps(TH2D *hist, int x, int y);
    float FindAvgRandomWalkSteps(int x, int y, int radious, bool isBig);

    double Distance (int x1, int y1, int x2, int y2);
    double gaus2D( int x0, int y0, int x, int y, double sigma);
    double invGaus( double val, double sigma);
};

class ImageManager
{
public:
    static ImageManager& getInstance()
    {
        static ImageManager    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    void Init(int XPixels,int YPixels,TArrayD* Arr)
    {
        xPixels = XPixels;
        yPixels = YPixels;
        arr = Arr;
        avgValue = 0;
        int tmpCount = 0;
        for (int yi=0; yi<yPixels; yi++)
        {
            for (int xi=0; xi<xPixels; xi++)
            {
                if( IsBlackOrIgnored(xi,yi)) continue;
                avgValue+=GetPixelValue(xi,yi);
                tmpCount++;
            }
        }
        avgValue/=tmpCount;
        initialBigMinDistance.clear();
        initialSmallMinDistance.clear();
        initialBigMaxDistance.clear();
        initialSmallMaxDistance.clear();
        initialBigRandomWalkSteps.clear();
        initialSmallRandomWalkSteps.clear();
    }

    bool PixelInBoundry(int x, int y)
    {
        if ( x <0 || x >= xPixels || y<0 || y >= yPixels) return false;
        return true;
    }
    Double_t GetPixelValue(int x, int y)
    {
        return arr->At(y*yPixels + x);
    }

    bool IsBlack(int x, int y)
    {
        return GetPixelValue(x,y) == 0;

    }
    bool IsIgnored(int x, int y)
    {
        if ( GetPixelValue(x,y) > 0.98 )
            return  true;
        else
            return false;
    }
    bool IsBlackOrIgnored(int x,int y)
    {
        return ( IsBlack(x,y) || IsIgnored(x,y) );
    }

    void AddInitialIntensity(int x, int y, float val)
    {
        initialIntensity.insert(y*yPixels + x,val);
    }
    float GetInitialIntensity(int x, int y)
    {
        return initialIntensity.value(y*yPixels + x);
    }

    void AddInitialMinDist(int x, int y, float val,bool isBig)
    {
        if ( isBig == true )
            initialBigMinDistance.insert(y*yPixels + x,val);
        else
            initialSmallMinDistance.insert(y*yPixels + x,val);
    }
    float GetInitialMinDist(int x, int y,bool isBig)
    {
        if ( isBig == true )
            return initialBigMinDistance.value(y*yPixels + x);
        else
            return initialSmallMinDistance.value(y*yPixels + x);

    }
    void AddInitialMaxDist(int x, int y, float val,bool isBig)
    {
        if ( isBig == true )
            initialBigMaxDistance.insert(y*yPixels + x,val);
        else
            initialSmallMaxDistance.insert(y*yPixels + x,val);
    }
    float GetInitialMaxDist(int x, int y,bool isBig)
    {
        if ( isBig == true )
            return initialBigMaxDistance.value(y*yPixels + x);
        else
            return initialSmallMaxDistance.value(y*yPixels + x);
    }

    void AddInitialRandomWalkSteps(int x, int y, int val,bool isBig)
    {
        if ( isBig == true )
            initialBigRandomWalkSteps.insert(y*yPixels + x,val);
        else
            initialSmallRandomWalkSteps.insert(y*yPixels + x,val);
    }
    int GetInitialRandomWalkSteps(int x, int y,bool isBig)
    {
        if ( isBig == true )
            return initialBigRandomWalkSteps.value(y*yPixels + x);
        else
            return initialSmallRandomWalkSteps.value(y*yPixels + x);
    }

    int yPixels;
    int xPixels;
    TArrayD* arr;
    double avgValue;

    QMap<int,float> initialIntensity;

    QMap<int,float> initialBigMinDistance;
    QMap<int,float> initialBigMaxDistance;
    QMap<int,int> initialBigRandomWalkSteps;

    QMap<int,float> initialSmallMinDistance;
    QMap<int,float> initialSmallMaxDistance;
    QMap<int,int> initialSmallRandomWalkSteps;
};

#endif // IMAGEHELPER_H
