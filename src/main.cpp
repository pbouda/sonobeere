#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "beeresonotopy.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    BeereSonotopy sonotopy;
    QQmlApplicationEngine* engine = new QQmlApplicationEngine();

    engine->rootContext()->setContextProperty("sonotopy", &sonotopy);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
