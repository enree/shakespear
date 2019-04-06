#include "SocketReader.h"

#include "gsl/gsl_assert"

#include <QTcpSocket>

namespace shakespear
{

SocketReader::SocketReader(QTcpSocket* socket) : QObject(socket)
{
    Expects(socket != nullptr);
    connect(socket, &QAbstractSocket::readyRead, this, &SocketReader::read);

    m_inputStream.setDevice(socket);
    m_inputStream.setVersion(QDataStream::Qt_5_12);
}

void SocketReader::read()
{
    m_inputStream.startTransaction();

    QByteArray message;
    m_inputStream >> message;

    if (m_inputStream.commitTransaction())
    {
        emit received(message);
    }
}

} // namespace shakespear
