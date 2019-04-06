#include "NetworkServer.h"

#include "InvalidConnection.h"

#include "core/SocketReader.h"

#include "log/Log.h"
#include "qt/Strings.h"

#include <QTcpServer>
#include <QTcpSocket>

namespace shakespear
{

NetworkServer::NetworkServer(
    const QHostAddress& address, int port, QObject* parent)
    : QObject(parent), m_server(std::make_unique<QTcpServer>())
{
    connect(
        m_server.get(),
        &QTcpServer::newConnection,
        this,
        &NetworkServer::acceptConnection);

    m_server->listen(address, port);
}

void NetworkServer::send(const QByteArray& data)
{
    if (!m_socket.isNull())
    {
        m_socket->write(data);
    }

    BOOST_THROW_EXCEPTION(exception::InvalidConnection());
}

NetworkServer::~NetworkServer() = default;

void NetworkServer::acceptConnection()
{
    LOG_INFO << tr("Incoming connection");
    auto m_socket = m_server->nextPendingConnection();

    connect(
        m_socket,
        &QAbstractSocket::disconnected,
        m_socket,
        &QObject::deleteLater);

    auto socketReader = new SocketReader(m_socket);

    connect(
        socketReader, &SocketReader::received, this, &NetworkServer::received);
}

} // namespace shakespear
