Cursor was used in the making of this :) basically a test/showcase project

# Java ENet JNI Wrapper

This project provides Java bindings for the ENet reliable UDP networking library through JNI.

## Prerequisites

- JDK 8 or higher (java 21 recommended)
- CMake 3.10 or higher (4.0 recommended)
- A C++ compiler (GCC, MSVC, Clang)
- Gradle

## Building

1. Clone the repository with submodules:
```bash
git clone --recursive https://github.com/theoplegends/java-enet-jni.git
```

2. Build the project using Gradle:
```bash
./gradlew build
```

This will:
- Generate JNI headers
- Build the native library
- Run the tests

## Usage

Add the library to your project dependencies and make sure the native library is in your library path.

Basic example:

```java
import op.legends.enet.ENetLibrary;

public class Example {
    public static void main(String[] args) {
        // Initialize ENet
        if (ENetLibrary.initialize() < 0) {
            System.err.println("Failed to initialize ENet");
            return;
        }

        try {
            // Create a server host
            long serverHost = ENetLibrary.createHost("127.0.0.1", 34567, 32, 2, 0, 0);
            if (serverHost == 0) {
                System.err.println("Failed to create server host");
                return;
            }

            // Create an event handle
            long event = ENetLibrary.createEvent();

            // Service the host
            while (true) {
                if (ENetLibrary.hostService(serverHost, event, 1000) > 0) {
                    switch (ENetLibrary.getEventType(event)) {
                        case ENetLibrary.EVENT_TYPE_CONNECT:
                            System.out.println("A new client connected!");
                            break;
                        case ENetLibrary.EVENT_TYPE_DISCONNECT:
                            System.out.println("A client disconnected!");
                            break;
                        case ENetLibrary.EVENT_TYPE_RECEIVE:
                            byte[] data = ENetLibrary.getEventPacketData(event);
                            System.out.println("Received message: " + new String(data));
                            break;
                    }
                }
            }
        } finally {
            ENetLibrary.deinitialize();
        }
    }
}
```

## Event Types

The library defines the following event types:
- `EVENT_TYPE_NONE` (0): No event occurred
- `EVENT_TYPE_CONNECT` (1): A peer has connected
- `EVENT_TYPE_DISCONNECT` (2): A peer has disconnected
- `EVENT_TYPE_RECEIVE` (3): A peer has sent data

## API Reference

The `ENetLibrary` class provides the following main methods:

### Initialization
- `initialize()`: Initialize the ENet library
- `deinitialize()`: Cleanup the ENet library

### Host Management
- `createHost(String address, int port, int peerCount, int channelLimit, long incomingBandwidth, long outgoingBandwidth)`: Create a host for sending/receiving data
- `hostDestroy(long host)`: Destroy a host
- `hostService(long host, long event, int timeout)`: Service a host for events

### Peer Management
- `connect(long host, String address, int port, int channelCount, long data)`: Connect to a remote host
- `peerDisconnect(long peer, long data)`: Gracefully disconnect from a peer
- `peerDisconnectNow(long peer, long data)`: Force immediate disconnection
- `peerReset(long peer)`: Reset the peer's connection
- `peerSend(long peer, int channelID, byte[] data)`: Send data to a peer

### Event Handling
- `createEvent()`: Create an event handle
- `destroyEvent(long event)`: Destroy an event handle
- `getEventType(long event)`: Get the type of the event
- `getEventPeer(long event)`: Get the peer associated with the event
- `getEventChannelID(long event)`: Get the channel ID for the event
- `getEventData(long event)`: Get the event data
- `getEventPacketData(long event)`: Get the packet data from the event

### Packet Management
- `createPacket(byte[] data, int flags)`: Create a packet
- `destroyPacket(long packet)`: Destroy a packet

### Statistics and Monitoring
- `getPeerState(long peer)`: Get the current state of a peer
- `getPeerRoundTripTime(long peer)`: Get the current round trip time
- `getPeerLastRoundTripTime(long peer)`: Get the last round trip time
- `getPeerLastSendTime(long peer)`: Get the last send time
- `getPeerLastReceiveTime(long peer)`: Get the last receive time
- `getPeerPacketsSent(long peer)`: Get the number of packets sent
- `getPeerPacketsLost(long peer)`: Get the number of packets lost

## Testing

The project includes comprehensive unit tests that demonstrate the functionality of the library. Run the tests using:

```bash
./gradlew test
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the SKID License - see the LICENSE file for details. 
