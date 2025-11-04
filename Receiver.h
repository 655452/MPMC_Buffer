#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>
#include "Frame.h"
#include "SPSCRing.h"
#include "ILockFreeRing.h"

/**
 * @brief Receiver handles TCP stream from simulator/hardware.
 *
 * Responsibilities:
 *   - Connect to TCP server at 127.0.0.1:5000
 *   - Read incoming byte stream
 *   - Parse packet headers and payload
 *   - Push frames into SPSC buffer
 */
class Receiver : public QObject {
    Q_OBJECT
public:
    explicit Receiver(ILockFreeRing<Frame> *ring, QObject *parent=nullptr);

private slots:
    void onReadyRead();

private:
    QTcpSocket *m_socket;
    QByteArray m_buffer;
    ILockFreeRing<Frame> *m_ring;
};

#endif // RECEIVER_H
