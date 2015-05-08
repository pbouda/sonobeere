#ifndef BEERESONOTOPY_H
#define BEERESONOTOPY_H

#include <QAudioInput>
#include <QQmlListProperty>
#include <QTimer>

#include <sonotopy/sonotopy.hpp>

class BeereSonotopy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int gridWidth READ gridWidth NOTIFY gridMapChanged)
    Q_PROPERTY(int gridHeight READ gridHeight NOTIFY gridMapChanged)
    Q_PROPERTY(QStringList gridMapColors READ gridMapColors NOTIFY gridMapChanged)
    Q_PROPERTY(QList<qreal> gridMap READ gridMapAsList NOTIFY gridMapChanged)
    Q_PROPERTY(int gridMapLength READ gridMapLength NOTIFY gridMapChanged)
    Q_PROPERTY(QStringList availableAudioDevices MEMBER availableAudioDevices NOTIFY availableAudioDevicesChanged)
    Q_PROPERTY(QString currentAudioDeviceName MEMBER currentAudioDeviceName NOTIFY currentAudioDeviceChanged)

public:
    explicit BeereSonotopy(QObject *parent = 0);
    ~BeereSonotopy();

    int gridWidth() const;
    int gridHeight() const;
    int gridMapLength() const;
    QList<qreal> gridMapAsList() const;
    QStringList gridMapColors() const;
    Q_INVOKABLE float gridMapAt(int, int) const;

signals:
    void gridMapChanged();
    void gridWidthChanged();
    void gridHeightChanged();
    void availableAudioDevicesChanged();
    void currentAudioDeviceChanged();

private slots:
    void processAudio();
    void update();
    void deviceChanged();

private:
    QAudioInput* audio;
    QIODevice* audioDevice;
    QAudioDeviceInfo currentAudioDevice;
    QString currentAudioDeviceName;
    QStringList availableAudioDevices;
    QByteArray buffer;
    QTimer timer;
    sonotopy::AudioParameters audioParameters;
    sonotopy::SpectrumAnalyzerParameters spectrumAnalyzerParameters;
    sonotopy::GridMapParameters gridMapParameters;
    sonotopy::GridMap *gridMap;

    void initializeAudioProcessing();
    void initializeAudioDevice();
    void stopAudioProcessing();
    void processAudioSonotopy(const float *);
};

#endif // BEERESONOTOPY_H
