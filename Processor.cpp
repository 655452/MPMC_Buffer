#include "Processor.h"

Processor::Processor(ILockFreeRing<Frame> *ring, QObject *parent)
    : QThread(parent), m_ring(ring), m_stop(false) {
    // Cross-platform OS-level thread priority
    this->setPriority(QThread::TimeCriticalPriority);
}

void Processor::stop() {
    m_stop.store(true);
}

void Processor::run() {
    qDebug() << "Processor started in thread:" << QThread::currentThread();
    while (!m_stop.load()) {
        Frame f;

        if (m_ring->try_pop(f)) {
            // Simulated DSP: just print first channel
            qDebug() << "Processed Frame ts=" << f.ts_ns
                     << "ch0=" << f.channels[0];
        } else {
            msleep(1); // avoid busy spin
        }
    }
}
