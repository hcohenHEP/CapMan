#include "imagehelper.h"
#include <math.h>
#include <qqueue.h>

Float_t ImageHelper::FindNeighborsDensity(int x, int y, int radious, bool isBig)
{
    float nIPixels = 0,nDPixels=0;
    int xLow=x-radious*SQ2, xHigh=x+radious*SQ2,yLow=y-radious*SQ2,yHigh=y+radious*SQ2;
    while (xLow<0)xLow++;
    while (xHigh>ImageManager::getInstance().xPixels) xHigh--;
    while (yLow<0)yLow++;
    while (yHigh>ImageManager::getInstance().yPixels) yHigh--;

    for ( int xi = xLow ; xi < xHigh ; xi ++)
    {
        for (int yi = yLow ; yi < yHigh ; yi ++)
        {
            if ( Distance(x,y,xi,yi) > radious) continue;
            nIPixels++;

            if ( false == ImageManager::getInstance().IsBlack(xi,yi) )
                nDPixels++;
        }
    }
    return  nDPixels/nIPixels;
}

double ImageHelper::Distance (int x1, int y1, int x2, int y2)
{
    double retVal = sqrt( pow(static_cast<double>(x1-x2),2) + pow(static_cast<double>(y1-y2),2));
    return retVal;
}

double ImageHelper::gaus2D( int x0, int y0, int x, int y, double sigma)
{
    return exp(-1*( pow(static_cast<double>(x0-x),2)/(2*pow(static_cast<double>(sigma),2)) + pow(static_cast<double>(y0-y),2)/(2*pow(static_cast<double>(sigma),2)) ));
}

double ImageHelper::invGaus( double val, double sigma)
{
    return 1/(  (1/(sigma*sqrt(2*M_PI)))*exp(-1*(pow(val,2)/(2*sigma*sigma)))    );
}

void ImageHelper::AdvanceInDirection(int *x, int *y, Direction direction)
{
    switch (direction)
    {
    case UP:(*y)+=1;break;
    case DOWN:(*y)+=-1;break;
    case RIGHT:(*x)+=1;break;
    case LEFT:(*x)+=-1;break;
    case UP_LEFT:(*x)+=-1;(*y)+=1;break;
    case UP_LEFT_LEFT:(*x)+=-2;(*y)+=1;break;
    case UP_UP_LEFT:(*x)+=-1;(*y)+=2;break;
    case UP_RIGHT:(*x)+=1;(*y)+=1;break;
    case UP_RIGHT_RIGHT:(*x)+=2;(*y)+=1;break;
    case UP_UP_RIGHT:(*x)+=1;(*y)+=2;break;
    case DOWN_LEFT:(*x)+=-1;(*y)+=-1;break;
    case DOWN_LEFT_LEFT:(*x)+=-2;(*y)+=-1;break;
    case DOWN_DOWN_LEFT:(*x)+=-1;(*y)+=-2;break;
    case DOWN_RIGHT:(*x)+=1;(*y)+=-1;break;
    case DOWN_RIGHT_RIGHT:(*x)+=2;(*y)+=-1;break;
    case DOWN_DOWN_RIGHT:(*x)+=1;(*y)+=-2;break;

    }
}


Float_t ImageHelper::FindNearestBlack(TH2D* hist,int x,int y, Direction direction, int threshold, int* retX, int* retY)
{
    int x_b=x,y_b=y;

    //QQueue<int> qX,qY;
    //qX.enqueue(x);qX.enqueue(x);qX.enqueue(x);qX.enqueue(x);qX.enqueue(x);qX.enqueue(x);qX.enqueue(x);qX.enqueue(x);qX.enqueue(x);
    //qY.enqueue(y);qY.enqueue(y);qY.enqueue(y);qY.enqueue(y);qY.enqueue(y);qY.enqueue(y);qY.enqueue(y);qY.enqueue(y);qY.enqueue(y);

    int count = 0, runs=0;
    float previousPixelIntensity = ImageManager::getInstance().GetInitialIntensity(x_b,y_b);

    while ( runs < XB*YB && count < threshold )
    {        

        if ( R <= 0.65 )
            AdvanceInDirection(&x_b,&y_b,direction);
        else
            AdvanceInDirection(&x_b,&y_b,(Direction)RDIRECTIONS);

        //qX.enqueue(x_b);
        //qY.enqueue(y_b);

        if ( !ImageManager::getInstance().PixelInBoundry(x_b,y_b) ) break;

        //Check for black pixel proximity
        float currentPixelIntensity = ImageManager::getInstance().GetInitialIntensity(x_b,y_b);
        float intensityRatio = std::max(previousPixelIntensity/currentPixelIntensity, currentPixelIntensity/previousPixelIntensity);

        if (hist->GetBinContent(x_b+1,y_b+1) == 0 || intensityRatio > 1.45 )
        {//The its black/ignored/(small or big)/intensity jumped
            count++;
        }
        runs++;
    }
    (*retX) = x_b;
    (*retY) = y_b;
    return Distance(x,y,x_b,y_b);//TBD - needs to add x_b-x_adv*(count-1)
}

Float_t ImageHelper::FindNeighborsIntensity(int x, int y,int radious)
{
    //TMP HADAR 0601
    //return ImageManager::getInstance().GetPixelValue(x,y);
    radious = 2;

    Float_t retVal = 0;
    float nIPixels = 0;
    int xLow=x-radious*SQ2, xHigh=x+radious*SQ2,yLow=y-radious*SQ2,yHigh=y+radious*SQ2;
    while (xLow<0)xLow++;
    while (xHigh>ImageManager::getInstance().xPixels) xHigh--;
    while (yLow<0)yLow++;
    while (yHigh>ImageManager::getInstance().yPixels) yHigh--;

    for ( int xi = xLow ; xi < xHigh ; xi ++)
    {
        for (int yi = yLow ; yi < yHigh ; yi ++)
        {
            if ( Distance(x,y,xi,yi) > radious) continue;
            if ( ImageManager::getInstance().IsBlackOrIgnored(xi,yi) == true) continue;

            double weight = 1;//gaus2D(x,y,xi,yi,radious*radious);//Weight function for the nearby pixels, could be also 1/dist, all we need is a
            retVal +=ImageManager::getInstance().GetPixelValue(xi,yi)*weight;
            nIPixels+=weight;
        }
    }

    return (retVal/nIPixels);
}

Float_t ImageHelper::FindAvgMinDistance(int x, int y, int radious, bool isBig)
{
    Float_t retVal = 0;
    float nIPixels = 0;
    int xLow=x-radious*SQ2, xHigh=x+radious*SQ2,yLow=y-radious*SQ2,yHigh=y+radious*SQ2;
    while (xLow<0)xLow++;
    while (xHigh>ImageManager::getInstance().xPixels) xHigh--;
    while (yLow<0)yLow++;
    while (yHigh>ImageManager::getInstance().yPixels) yHigh--;

    for ( int xi = xLow ; xi < xHigh ; xi ++)
    {
        for (int yi = yLow ; yi < yHigh ; yi ++)
        {
            double dis = Distance(x,y,xi,yi);
            if (  ( dis > radious) ||  true == ImageManager::getInstance().IsBlackOrIgnored(xi,yi) ) continue;

            double weight = gaus2D(x,y,xi,yi,radious);//Weight function for the nearby pixels, could be also 1/dist, all we need is a
            retVal +=ImageManager::getInstance().GetInitialMinDist(xi,yi,isBig)*weight;
            nIPixels+=weight;
        }
    }

    return (retVal/nIPixels);
}
Float_t ImageHelper::FindAvgMaxDistance(int x, int y, int radious, bool isBig)
{
    Float_t retVal = 0;
    float nIPixels = 0;
    int xLow=x-radious*SQ2, xHigh=x+radious*SQ2,yLow=y-radious*SQ2,yHigh=y+radious*SQ2;
    while (xLow<0)xLow++;
    while (xHigh>ImageManager::getInstance().xPixels) xHigh--;
    while (yLow<0)yLow++;
    while (yHigh>ImageManager::getInstance().yPixels) yHigh--;

    for ( int xi = xLow ; xi < xHigh ; xi ++)
    {
        for (int yi = yLow ; yi < yHigh ; yi ++)
        {
            double dis = Distance(x,y,xi,yi);
            if (  ( dis > radious) ||  true == ImageManager::getInstance().IsBlackOrIgnored(xi,yi) ) continue;

            double weight = gaus2D(x,y,xi,yi,radious);//Weight function for the nearby pixels, could be also 1/dist, all we need is a
            retVal +=ImageManager::getInstance().GetInitialMaxDist(xi,yi,isBig)*weight;
            nIPixels+=weight;
        }
    }

    return (retVal/nIPixels);
}



Double_t ImageHelper::MaxValue(const char* fileName)
{
    TASImage image(fileName);
    UInt_t yPixels = image.GetHeight();
    UInt_t xPixels = image.GetWidth();
    int index = 0;
    TArrayD* arr = image.GetArray();
    Double_t retVal = arr->GetAt(0)*100;

    for (UInt_t yi=0; yi<yPixels; yi++)
    {
        for (UInt_t xi=0; xi<xPixels; xi++)
        {
            if ( retVal < arr->GetAt(index)*100 )
                retVal = arr->GetAt(index)*100;

            index++;
        }
    }

    return retVal;
}

float ImageHelper::RandomWalkSteps(TH2D* hist, int x, int y)
{
    //Lets send N random walkers and avg on them
    float retVal = 0;
    int oldX, oldY;
    int threshold = 0;
    int steps = 0;
    float originalPixelIntensity = ImageManager::getInstance().GetInitialIntensity(x,y);


    while ( threshold < 5 )
    {
        oldX = x; oldY=y;
        AdvanceInDirection(&x,&y,(Direction)R8DIRECTIONS);
        if ( ImageManager::getInstance().PixelInBoundry(x,y) == false ) break;

        float currentPixelIntensity = ImageManager::getInstance().GetInitialIntensity(x,y);
        float intensityRatio = std::max(originalPixelIntensity/currentPixelIntensity, currentPixelIntensity/originalPixelIntensity);

        if (hist->GetBinContent(x+1,y+1) == 0  || intensityRatio > 1.5 )
        {//The its black/ignored/(small or big)
            threshold++;
            x = oldX; y = oldY; //restore last position
            steps-=1;
        }
        steps++;
    }
    retVal += steps;

    return steps;//Distance(x,y,originX,originY);
}

float ImageHelper::FindAvgRandomWalkSteps(int x,int y, int radious,bool isBig)
{
    Float_t retVal = 0;
    float nIPixels = 0;
    int xLow=x-radious*SQ2, xHigh=x+radious*SQ2,yLow=y-radious*SQ2,yHigh=y+radious*SQ2;
    while (xLow<0)xLow++;
    while (xHigh>ImageManager::getInstance().xPixels) xHigh--;
    while (yLow<0)yLow++;
    while (yHigh>ImageManager::getInstance().yPixels) yHigh--;

    for ( int xi = xLow ; xi < xHigh ; xi ++)
    {
        for (int yi = yLow ; yi < yHigh ; yi ++)
        {
            double dis = Distance(x,y,xi,yi);
            if (  ( dis > radious) ||  true == ImageManager::getInstance().IsBlack(xi,yi) ) continue;

            double weight = gaus2D(x,y,xi,yi,radious);//Weight function for the nearby pixels, could be also 1/dist, all we need is a
            retVal +=ImageManager::getInstance().GetInitialRandomWalkSteps(xi,yi,isBig)*weight;
            nIPixels+=weight;
        }
    }

    return (retVal/nIPixels);
}
