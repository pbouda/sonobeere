#ifndef BEERESONOTOPY_H
#define BEERESONOTOPY_H

#include <QAudioInput>
#include <sonotopy/sonotopy.hpp>

class BeereSonotopy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int gridWidth READ gridWidth NOTIFY gridWidthChanged)
    Q_PROPERTY(int gridHeight READ gridHeight NOTIFY gridHeightChanged)
    Q_PROPERTY(QList<qreal> gridMap READ gridMapAsList NOTIFY gridMapChanged)

public:
    explicit BeereSonotopy(QObject *parent = 0);
    ~BeereSonotopy();

    int gridWidth() const;
    int gridHeight() const;
    QList<qreal> gridMapAsList() const;
    Q_INVOKABLE float gridMapAt(int, int) const;

signals:
    void gridMapChanged();
    void gridWidthChanged();
    void gridHeightChanged();

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
