#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QThread>
#include <QDebug>
#include <atomic>
#include "Frame.h"
#include "SPSCRing.h"
#include "ILockFreeRing.h"

/**
 * @brief Processor thread consumes Frames from SPSC buffer and performs DSP.
 *
 * For now: just prints first channel of each frame.
 * Later: integrate filtering, FFT, beamforming, etc.
 */
class Processor : public QThread {
    Q_OBJECT
public:
    Processor(ILockFreeRing<Frame> *ring, QObject *parent=nullptr);
    void stop();

protected:
    void run() override;

private:
    ILockFreeRing<Frame> *m_ring;
    std::atomic<bool> m_stop;
};

#endif // PROCESSOR_H
