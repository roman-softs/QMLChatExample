#pragma once

#include <QObject>
#include <QList>

class QWebSocketServer;
class QWebSocket;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(quint16 port, QObject *parent = nullptr);

private slots:
    void onNewConnection();
    void processTextMessage(const QString& message);
    void socketDisconnected();

signals:

private:
    QWebSocketServer* m_server;
    QList<QWebSocket*> m_clients;
};
