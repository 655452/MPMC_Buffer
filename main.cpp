#include <QCoreApplication>
#include <QTcpSocket>
#include <QThread>
#include <QDebug>
#include <atomic>
#include <vector>
#include <cstring>
#include "Frame.h"
#include "Processor.h"
#include "Receiver.h"
#include "ILockFreeRing.h"
#include "QCommandLineParser"
#include "BoostRing.h"
#include "MpmcRing.h"

// -------- main.cpp --------
int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Real-Time Buffer Framework");
    parser.addHelpOption();

    QCommandLineOption ringOption(
        QStringList() << "r" << "ring",
        "Select ring type: 'spsc' or 'boost'.",
        "type", "spsc"
        );
    parser.addOption(ringOption);
    parser.process(a);

    QString ringType = parser.value(ringOption).toLower();
    std::unique_ptr<ILockFreeRing<Frame>> ring;

    if (ringType == "boost") {
        qDebug() << "Using Boost Ring Buffer.";
        ring = std::make_unique<BoostRing<Frame>>(1 << 16);
    }
    else {
        qDebug() << "Using Moodycamel MPMC Queue.";
        ring = std::make_unique<MpmcRing<Frame>>(1 << 16);
    }

    // // 64k frames capacityFr
    // SpscRing<Frame> ring(1 << 16);

    // Start processor thread
    Processor processor(ring.get());

    // Start receiver
    Receiver recv(ring.get());
    processor.start();

    QObject::connect(&a, &QCoreApplication::aboutToQuit, [&](){
        processor.stop();
        processor.wait();
    });

    return a.exec();
}
