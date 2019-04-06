#pragma once

#include <QDataStream>
#include <QObject>
#include <QPointer>

#include <memory>

class QTcpServer;
class QTcpSocket;
class QHostAddress;

namespace shakespear
{

class NetworkServer : public QObject
{
    Q_OBJECT
public:
    explicit NetworkServer(
        const QHostAddress& address, int port, QObject* parent = nullptr);
    ~NetworkServer();

signals:
    void received(QByteArray message);

public slots:
    /**
     * Send data to connected client
     */
    void send(const QByteArray& data);

private slots:
    void acceptConnection();

private:
    std::unique_ptr<QTcpServer> m_server;
    QPointer<QTcpSocket> m_socket;
    QDataStream m_inputStream;
};

} // namespace shakespear
