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
    QDataStream m_inputStream;
};

} // namespace shakespear
