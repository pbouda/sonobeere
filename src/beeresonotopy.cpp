#include "beeresonotopy.h"

#include <QDebug>

BeereSonotopy::BeereSonotopy(QObject *parent) : QObject(parent)
{
    initializeAudioDevice();
    initializeAudioProcessing();
    buffer = QByteArray(audioParameters.bufferSize*4, 0);
}

BeereSonotopy::~BeereSonotopy()
{
    stopAudioProcessing();
}

float BeereSonotopy::gridMapAt(int i, int j) const
{
    return gridMap->getActivation(i, j);
}

QList<qreal> BeereSonotopy::gridMapAsList() const
{
    const std::vector<float> *p = gridMap->getActivationPattern();
    const std::list<qreal> pD(p->begin(), p->end());
    //QVector<qreal> vec = QVector<qreal>::fromStdVector(pD);
    QList<qreal> values = QList<qreal>::fromStdList(pD);
    //qDebug() << values.length();
    return values;
}

QStringList BeereSonotopy::gridMapColors() const
{
    QStringList colors;
    foreach (qreal v, gridMapAsList()) {
        QString color = QString::number((int)(v*255), 16);
        if (color.length() < 2) color = "0" + color;
        colors.append("#" + color + color + color);
    }
    return colors;
}

int BeereSonotopy::gridWidth() const
{
    return gridMapParameters.gridWidth;
}

int BeereSonotopy::gridHeight() const
{
    return gridMapParameters.gridHeight;
}

int BeereSonotopy::gridMapLength() const
{
    return 900;
}

void BeereSonotopy::processAudioSonotopy(const float *inputBuffer)
{
    gridMap->feedAudio(inputBuffer, audioParameters.bufferSize);
    emit gridMapChanged();
    //qDebug() << gridMap->getActivation(15, 15);
}

void BeereSonotopy::processAudio()
{
    if (!audio)
        return;
    int len = audio->bytesReady();
    //qDebug() << len;
    if (len > audioParameters.bufferSize*4)
        len = audioParameters.bufferSize*4;
    int l = audioDevice->read(buffer.data(), len);
    //qDebug() << audio->error();
    //qDebug() << l;
    if (l > 0) {
        processAudioSonotopy(reinterpret_cast<const float*>(buffer.data()));
    }
}

void BeereSonotopy::initializeAudioProcessing()
{
    gridMap = new sonotopy::GridMap(audioParameters, spectrumAnalyzerParameters, gridMapParameters);
}

void BeereSonotopy::initializeAudioDevice()
{
    QAudioFormat format;
    // Set up the desired format, for example:
    format.setSampleRate(audioParameters.sampleRate);
    format.setChannelCount(1);
    format.setSampleSize(32);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::Float);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    qDebug() << info.deviceName();
    if (!info.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = info.nearestFormat(format);
        qDebug() << format;
    }

    audio = new QAudioInput(format, this);
    audioDevice = audio->start();
    connect(audioDevice, SIGNAL(readyRead()), this, SLOT(processAudio()));
}

void BeereSonotopy::stopAudioProcessing()
{
    audio->stop();
    delete audio;
}

