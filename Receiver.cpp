#include "Receiver.h"
#include <cstring>

Receiver::Receiver(ILockFreeRing<Frame> *ring, QObject *parent)
    : QObject(parent), m_ring(ring)
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::readyRead, this, &Receiver::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, []() {
        qDebug() << "Connected to simulator!";
    });

    m_socket->connectToHost("127.0.0.1", 5000);
}

void Receiver::onReadyRead()
{
    m_buffer.append(m_socket->readAll());

    // Each packet = header (16) + samples * 64 * sizeof(float)
    while (m_buffer.size() >= 16) {
        const char* ptr = m_buffer.constData();

        quint32 seq;
        quint64 ts_ns;
        quint16 samples, reserved;

        memcpy(&seq, ptr, 4); ptr += 4;
        memcpy(&ts_ns, ptr, 8); ptr += 8;
        memcpy(&samples, ptr, 2); ptr += 2;
        memcpy(&reserved, ptr, 2); ptr += 2;

        int payloadSize = samples * 64 * sizeof(float);
        int packetSize  = 16 + payloadSize;

        if (m_buffer.size() < packetSize)
            break; // incomplete packet

        const float* fptr = reinterpret_cast<const float*>(m_buffer.constData() + 16);

        // Push each time sample as a Frame
        for (int s = 0; s < samples; ++s) {
            Frame f;
            f.ts_ns = ts_ns + s * (1000000000ULL / 32000ULL);
            for (int ch = 0; ch < 64; ++ch) {
                f.channels[ch] = fptr[s * 64 + ch];
            }
            if (!m_ring->try_push(f)) {
                qWarning() << "Ring buffer overflow!";
            }
        }

        m_buffer.remove(0, packetSize);
    }
}
