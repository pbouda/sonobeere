#include "beeresonotopy.h"

#include <QDebug>

BeereSonotopy::BeereSonotopy(QObject *parent) : QObject(parent)
{
    currentAudioDevice = QAudioDeviceInfo::defaultInputDevice();
    currentAudioDeviceName = currentAudioDevice.deviceName();
    foreach (QAudioDeviceInfo i, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        availableAudioDevices.append(i.deviceName());
    }

    initializeAudioDevice();
    initializeAudioProcessing();
    buffer = QByteArray(audioParameters.bufferSize*4, 0);
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(1000 / 33);

    connect(this, SIGNAL(currentAudioDeviceChanged()), this, SLOT(deviceChanged()));
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
    static float v;

    const sonotopy::SOM::ActivationPattern *activationPattern =
            gridMap->getActivationPattern();
    sonotopy::SOM::ActivationPattern::const_iterator activationPatternIterator =
            activationPattern->begin();

    for(int y = 0; y < gridMapParameters.gridHeight; y++) {
        for(int x = 0; x < gridMapParameters.gridWidth; x++) {
            v = *activationPatternIterator;
            QString color = QString::number((int)(v*255), 16);
            if (color.length() < 2) color = "0" + color;
            colors.append("#" + color + color + color);
            activationPatternIterator++;
        }
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
    return gridMapParameters.gridHeight*gridMapParameters.gridWidth;
}

void BeereSonotopy::processAudioSonotopy(const float *inputBuffer)
{
    gridMap->feedAudio(inputBuffer, audioParameters.bufferSize);
    //emit gridMapChanged();
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

void BeereSonotopy::update()
{
    emit gridMapChanged();
}

void BeereSonotopy::deviceChanged()
{
    audio->stop();
    delete audio;

    foreach (QAudioDeviceInfo i, QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        if (i.deviceName() == currentAudioDeviceName) {
            currentAudioDevice = i;
            qDebug() << "Changed device to " << currentAudioDeviceName;
        }
    }
    initializeAudioDevice();
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

    if (!currentAudioDevice.isFormatSupported(format)) {
        qWarning() << "Default format not supported, trying to use the nearest.";
        format = currentAudioDevice.nearestFormat(format);
        qDebug() << format;
    }

    audio = new QAudioInput(currentAudioDevice, format, this);
    audioDevice = audio->start();
    connect(audioDevice, SIGNAL(readyRead()), this, SLOT(processAudio()));
}

void BeereSonotopy::stopAudioProcessing()
{
    audio->stop();
    delete audio;
}

