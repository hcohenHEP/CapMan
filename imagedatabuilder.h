#ifndef IMAGEDATABUILDER_H
#define IMAGEDATABUILDER_H
#include <QString>
#include <QThread>
class ImageDataBuilder : public QThread
{
    Q_OBJECT
public:
    ImageDataBuilder(QString fileName,int blackPixelThreshold,int distanceRadius);
    float Progress;

    void run();


private:
    QString m_FileName;
    int m_BlackPixelThreshold;
    int m_DistanceRadius;


};

#endif // IMAGEDATABUILDER_H
