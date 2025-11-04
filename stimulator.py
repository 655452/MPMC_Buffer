# simulator_tcp.py
import socket, struct, time, random

HOST = "127.0.0.1"
PORT = 5000

CHANNELS = 64
SAMPLES_PER_PACKET = 8
SAMPLE_RATE = 32000

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(1)
print("Simulator: waiting for client connection...")
conn, addr = server.accept()
print("Client connected:", addr)

seq = 0
while True:
    ts_ns = int(time.time() * 1e9)  # timestamp in ns

    # header (16 bytes)
    header = struct.pack("<IQHH", seq, ts_ns, SAMPLES_PER_PACKET, 0)

    payload = []
    for s in range(SAMPLES_PER_PACKET):
        for ch in range(CHANNELS):
            payload.append(random.uniform(-1.0, 1.0))  # fake sample

    payload_bytes = struct.pack("<" + "f" * len(payload), *payload)
    packet = header + payload_bytes

    conn.sendall(packet)
    seq += 1
    time.sleep(SAMPLES_PER_PACKET / SAMPLE_RATE)  # pace ~real-time
