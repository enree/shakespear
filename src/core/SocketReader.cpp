#include "SocketReader.h"

#include "log/Log.h"

#include "gsl/gsl_assert"

#include <QTcpSocket>

namespace shakespear
{

namespace
{

qint32 arrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

QByteArray intToArray(qint32 source)
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::WriteOnly);
    data << source;
    return temp;
}

} // namespace

SocketReader::SocketReader(QTcpSocket* socket)
    : QObject(socket), m_socket(socket)
{
    Expects(m_socket != nullptr);
    connect(m_socket, &QAbstractSocket::readyRead, this, &SocketReader::read);
}

void SocketReader::read()
{
    const int headerSize = sizeof(qint32);
    while (m_socket->bytesAvailable() > 0)
    {
        m_buffer.append(m_socket->readAll());
        while ((m_bufferSize == 0 && m_buffer.size() >= headerSize)
               || (m_bufferSize > 0 && m_buffer.size() >= m_bufferSize))
        {
            if (m_bufferSize == 0 && m_buffer.size() >= headerSize)
            {
                m_bufferSize = arrayToInt(m_buffer.mid(0, headerSize));
                m_buffer.remove(0, headerSize);
            }
            if (m_bufferSize > 0 && m_buffer.size() >= m_bufferSize)
            {
                QByteArray data = m_buffer.mid(0, m_bufferSize);
                m_buffer.remove(0, m_bufferSize);
                m_bufferSize = 0;
                emit received(data);
            }
        }
    }
}

void writeToSocket(QTcpSocket& socket, const QByteArray& array)
{
    if (socket.state() == QAbstractSocket::ConnectedState)
    {
        socket.write(intToArray(array.size()));
        socket.write(array);
    }
}

} // namespace shakespear
