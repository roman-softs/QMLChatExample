#include "server.h"

#include "../protocol.h"

#include <QWebSocketServer>
#include <QWebSocket>

#include <QJsonDocument>
#include <QJsonObject>

Server::Server(quint16 port, QObject *parent)
    : QObject(parent)
    , m_server(new QWebSocketServer(QStringLiteral("TestingServer"),
                                    QWebSocketServer::SslMode::NonSecureMode,
                                    this))
{

    connect(m_server, QOverload<QWebSocketProtocol::CloseCode>::of(&QWebSocketServer::serverError),
            this, [this] (QWebSocketProtocol::CloseCode error) {
        Q_UNUSED(error)
        qDebug() << m_server->errorString();
    });
    connect(m_server, QOverload<const QList<QSslError>&>::of(&QWebSocketServer::sslErrors),
            this, [this] (const QList<QSslError> &errors) {
        Q_UNUSED(errors)
        qDebug() << m_server->errorString();
    });

    if(m_server->listen(QHostAddress::LocalHost, port)) {
        connect(m_server, &QWebSocketServer::newConnection, this, &Server::onNewConnection);
    }

}

void Server::onNewConnection()
{
    qDebug() << "new connection" << "\n";
    auto* newClientSocket = m_server->nextPendingConnection();
    connect(newClientSocket, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
    connect(newClientSocket, &QWebSocket::disconnected, this, &Server::socketDisconnected);
    m_clients.append(newClientSocket);
}

void Server::processTextMessage(const QString &message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << message << "\n";
    if (pClient) {
        pClient->sendTextMessage(message);
    }

}

void Server::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
