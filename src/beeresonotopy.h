#ifndef BEERESONOTOPY_H
#define BEERESONOTOPY_H

#include <QAudioInput>
#include <sonotopy/sonotopy.hpp>

class BeereSonotopy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int gridWidth READ gridWidth)
    Q_PROPERTY(int gridHeight READ gridHeight)
    Q_PROPERTY(QVariantList gridMap READ gridMapAsList NOTIFY gridMapChanged)

public:
    explicit BeereSonotopy(QObject *parent = 0);
    ~BeereSonotopy();

    int gridWidth() const;
    int gridHeight() const;
    QVariantList gridMapAsList() const;

signals:
    void gridMapChanged();

public slots:
    int gridMapAt(int, int);

private slots:
    void processAudio();

private:
    QAudioInput* audio;
    QIODevice* audioDevice;
    QByteArray buffer;
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
