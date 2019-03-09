#include "NetworkClient.h"

#include "log/Log.h"

#include "qt/Strings.h"

#include <QDataStream>
#include <QTcpSocket>

namespace shakespear
{

NetworkClient::NetworkClient(
    const QHostAddress& address,
    int port,
    int retryTimeout,
    int attempts,
    QObject* parent)
    : QObject(parent)
    , m_socket(std::make_unique<QTcpSocket>())
    , m_address(address)
    , m_port(port)
    , m_maxAttempts(attempts)
    , m_retryTimeout(retryTimeout)
{
    connect(
        m_socket.get(),
        &QTcpSocket::connected,
        this,
        &NetworkClient::connected);

    connect(
        m_socket.get(),
        &QTcpSocket::disconnected,
        this,
        &NetworkClient::disconnected);

    connect(
        m_socket.get(),
        QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
        this,
        &NetworkClient::connectionError);

    connect(&m_timer, &QTimer::timeout, this, &NetworkClient::tryToConnect);
}

void NetworkClient::send(const QString& data)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    out << data;
    m_socket->write(block);
}

void NetworkClient::connectToHost()
{
    m_attempt = 0;
    m_timer.start();
}

void NetworkClient::disconnectFromHost()
{
    m_timer.stop();
    m_socket->disconnectFromHost();
}

NetworkClient::~NetworkClient() = default;

void NetworkClient::tryToConnect()
{
    if (m_attempt < m_maxAttempts)
    {
        LOG_INFO
            << tr("Connecting. %1 of %2").arg(m_attempt + 1).arg(m_maxAttempts);

        m_timer.stop();
        m_socket->connectToHost(m_address, m_port);
        ++m_attempt;
    }
    else
    {
        emit error(tr("Can't connect to host"));
    }
}

void NetworkClient::connectionError(QAbstractSocket::SocketError error)
{
    LOG_INFO << tr("Error connecting %1").arg(error);
    LOG_INFO << tr("Retry in %1 ms").arg(m_retryTimeout);

    m_timer.start(m_retryTimeout);
}

} // namespace shakespear
