#pragma once

#include <QObject>
#include <QUrl>
#include <QQueue>

class QWebSocket;

class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(QObject *parent = nullptr);
    void connectToHostWithUserName(const QUrl &url, const QString& userName);
    bool isConnected();
    void sendMessage(const QString& message);

private slots:
    void onTextFrameReceived(const QString &frame, bool isLastFrame);

signals:
    void textFrameReceived(const QString& frame);
    void messageReceived(const QDateTime& dateTime, const QString& userName, const QString& text);
    void connected();
    void disconnected();

private:
    QWebSocket* m_socket;
    QString m_userName;
};
