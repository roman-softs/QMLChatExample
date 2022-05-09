#include "backend.h"
#include "socket.h"

#include <QDebug>

Backend::Backend(QObject *parent)
    : QObject(parent)
    , m_socket(new Socket(this))
{
    connect(m_socket, &Socket::messageReceived, this, &Backend::messageReceived);
}

void Backend::setUserName(const QString& userName)
{
    if(m_userName == userName)
        return;
    m_userName = userName;
    emit userNameChanged(m_userName);
}

void Backend::setHostAddress(const QString &hostAddress)
{
    if(m_hostAddress == hostAddress)
        return;
    m_hostAddress = hostAddress;
    emit hostAddressChanged(m_hostAddress);
}

QString Backend::userName() const
{
    return m_userName;
}

QString Backend::hostAddress() const
{
    return m_hostAddress;
}

void Backend::connectToHost()
{
    m_socket->connectToHostWithUserName(QUrl(m_hostAddress), m_userName);
}

void Backend::sendMessage(const QString &message)
{
    m_socket->sendMessage(message);
}
