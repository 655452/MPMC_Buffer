#ifndef FRAME_H
#define FRAME_H

// #include <QtGloSPbal>
#include <QtGlobal>
#include <QtTypes>
/**
 * @brief Frame represents one timestamped sample set across all 64 channels.
 */
struct Frame {
    quint64 ts_ns;       ///< Timestamp in nanoseconds
    float channels[64];  ///< Data values for each channel
};

#endif // FRAME_H
