#include "socket.h"

#include "../protocol.h"

#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>


Socket::Socket(QObject *parent)
    : QObject(parent)
    , m_socket(new QWebSocket)
{
    m_socket->setParent(this);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            this,  [this] (QAbstractSocket::SocketError error) {
        Q_UNUSED(error)
        qDebug() << QString("socket error: " + m_socket->errorString());
    });

    connect(m_socket, &QWebSocket::textFrameReceived, this, &Socket::onTextFrameReceived);
}

void Socket::connectToHostWithUserName(const QUrl &url, const QString &userName)
{
    if(userName == m_userName && url == m_socket->requestUrl() && m_socket->isValid()) {
        return;
    }
    m_userName = userName;

    m_socket->open(url);
}

bool Socket::isConnected()
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void Socket::sendMessage(const QString &message)
{
    QJsonObject obj;

    using namespace protocol;
    obj[JsonKeys::dateTime] = QDateTime::currentDateTime().toString(Qt::ISODate);
    obj[JsonKeys::userName] = m_userName;
    obj[JsonKeys::message] = message;
    QJsonDocument doc(obj);
    m_socket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
}

void Socket::onTextFrameReceived(const QString &frame, bool isLastFrame)
{
    Q_UNUSED(isLastFrame)

    QJsonDocument doc = QJsonDocument::fromJson(frame.toUtf8());

    const QString cantParseDebugMessage = "can't parse message";

    if(doc.isNull() || !doc.isObject()) {
        qDebug() << cantParseDebugMessage;
        return;
    }

    QJsonObject obj = doc.object();

    using namespace protocol;

    auto dateTimeStr = obj[JsonKeys::dateTime];
    if(!dateTimeStr.isString()) {
        qDebug() << cantParseDebugMessage;
        return;
    }
    auto userNameStr = obj[JsonKeys::userName];
    if(!userNameStr.isString()) {
        qDebug() << cantParseDebugMessage;
        return;
    }
    auto messageStr = obj[JsonKeys::message];
    if(!messageStr.isString()) {
        qDebug() << cantParseDebugMessage;
        return;
    }
    QDateTime dateTime = QDateTime::fromString(dateTimeStr.toString(), Qt::ISODate);
    if(!dateTime.isValid()) {
        qDebug() << cantParseDebugMessage;
        return;
    }

    emit messageReceived(dateTime, userNameStr.toString(), messageStr.toString());
}

