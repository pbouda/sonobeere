#include <QDebug>

#include "beeresonotopy.h"

BeereSonotopy::BeereSonotopy(QObject *parent) : QObject(parent),
    buffer(audioParameters.bufferSize, 0)
{
    initializeAudioDevice();
    initializeAudioProcessing();
}

BeereSonotopy::~BeereSonotopy()
{
    stopAudioProcessing();
}

int BeereSonotopy::gridMapAt(int i, int j)
{
    return gridMap->getActivation(i, j);
}

QVariantList BeereSonotopy::gridMapAsList() const
{
    QVariantList values;

    for (int i=0; i<gridMapParameters.gridHeight; i++)
        for (int j=0; j<gridMapParameters.gridWidth; j++)
            values.append(QVariant(gridMap->getActivation(i, j)));

    return values;
}

int BeereSonotopy::gridWidth() const
{
    return gridMapParameters.gridWidth;
}

int BeereSonotopy::gridHeight() const
{
    return gridMapParameters.gridHeight;
}

void BeereSonotopy::processAudioSonotopy(const float *inputBuffer)
{
    gridMap->feedAudio(inputBuffer, audioParameters.bufferSize);
    emit gridMapChanged();
    //qDebug() << gridMap->getActivation(0, 0);
}

void BeereSonotopy::processAudio()
{
    if (!audio)
        return;
    int len = audio->bytesReady();
    if (len > audioParameters.bufferSize)
        len = audioParameters.bufferSize;
    int l = audioDevice->read(buffer.data(), len);
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

