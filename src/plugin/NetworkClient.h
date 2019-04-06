#pragma once

#include <QHostAddress>
#include <QObject>
#include <QTimer>

#include <memory>

class QTcpSocket;

namespace shakespear
{

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    NetworkClient(
        const QHostAddress& address,
        int port,
        int retryTimeout,
        int attempts,
        QObject* parent = nullptr);

    ~NetworkClient();

signals:
    /**
     * Client is connected to host
     */
    void connected();
    /**
     * Error is occured
     */
    void error(QString errorString);
    /**
     * Client is disconnected from host
     */
    void disconnected();
    /**
     * New message has arrived
     */
    void received(QByteArray message);

public slots:
    /**
     * Send data to server
     */
    void write(const QByteArray& data);
    /**
     * Connect to remote server
     */
    void connectToHost();
    /**
     * Disconnect from remote server
     */
    void disconnectFromHost();

private slots:
    void tryToConnect();
    void connectionError(QAbstractSocket::SocketError error);

private:
    std::unique_ptr<QTcpSocket> m_socket;

    QHostAddress m_address;
    int m_port;
    int m_maxAttempts;
    int m_attempt = 0;
    int m_retryTimeout;

    QTimer m_timer;
};

} // namespace shakespear
