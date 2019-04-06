#pragma once

#include <QDataStream>

class QTcpSocket;

namespace shakespear
{

class SocketReader : public QObject
{
    Q_OBJECT
public:
    explicit SocketReader(QTcpSocket* socket);

signals:
    void received(QByteArray message);

private slots:
    void read();

private:
    QTcpSocket* m_socket;
    QByteArray m_buffer;
    qint32 m_bufferSize = 0;
};

void writeToSocket(QTcpSocket& socket, const QByteArray& array);

} // namespace shakespear
