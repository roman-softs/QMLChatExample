#include "sql_conversation_model.h"
#include "backend.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtQml>


void connectToDatabase(const QDir& writeDir)
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid()) {
        database = QSqlDatabase::addDatabase("QSQLITE");
        if (!database.isValid())
            qFatal("Cannot add database: %s", qPrintable(database.lastError().text()));
    }

    if (!writeDir.mkpath("."))
        qFatal("Failed to create writable directory at %s", qPrintable(writeDir.absolutePath()));

    const QString fileName = writeDir.absolutePath() + "/chat-database.sqlite3";

    database.setDatabaseName(fileName);
    if (!database.open()) {
        qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
        QFile::remove(fileName);
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    connectToDatabase(app.applicationDirPath());

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);
    SqlConversationModel sqlConversationModel;
    engine.rootContext()->setContextProperty("sqlConversationModel", &sqlConversationModel);
    QObject::connect(&backend, &Backend::messageReceived, &sqlConversationModel, &SqlConversationModel::sendMessage);

    return app.exec();
}
