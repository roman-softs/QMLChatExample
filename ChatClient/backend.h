#pragma once

#include <QObject>

class Socket;

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString hostAddress READ hostAddress WRITE setHostAddress NOTIFY hostAddressChanged)
public:
    explicit Backend(QObject *parent = nullptr);

    void setUserName(const QString& userName);
    void setHostAddress(const QString& hostAddress);

    QString userName() const;
    QString hostAddress() const;

    Q_INVOKABLE void connectToHost();
    Q_INVOKABLE void sendMessage(const QString &message);

signals:
    void userNameChanged(const QString & userName);
    void hostAddressChanged(const QString& hostAddress);

    void messageReceived(const QDateTime& dateTime, const QString& userName, const QString text);

private:
    QString m_userName;
    QString m_hostAddress;
    Socket* m_socket;
};

