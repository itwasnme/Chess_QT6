#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include "ChessWrappers.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    BoardWrapper* gameBoard = new BoardWrapper();
    engine.rootContext()->setContextProperty("gameBoard", gameBoard);

    const QUrl url(QStringLiteral("qrc:/Chess_QT6/mainMenu.qml"));
    engine.load(url);


    return app.exec();

}


