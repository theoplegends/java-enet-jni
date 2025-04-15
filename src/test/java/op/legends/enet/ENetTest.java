package op.legends.enet;

import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.AfterAll;
import static org.junit.jupiter.api.Assertions.*;

public class ENetTest {
    private static final int TEST_PORT = 12345;
    private static final String TEST_HOST = "127.0.0.1";
    private static final int TIMEOUT = 1000; // Increased timeout to 1 second
    private static final int CHANNEL_COUNT = 2;
    private static final int MAX_PEERS = 32;
    
    @BeforeAll
    static void setup() {
        int result = ENetLibrary.initialize();
        assertTrue(result == 0, "ENet initialization failed");
    }
    
    @AfterAll
    static void cleanup() {
        ENetLibrary.deinitialize();
    }
    
    @Test
    public void testClientServerCommunication() {
        long serverHost = 0;
        long clientHost = 0;
        long event = 0;

        try {
            System.out.println("Creating server host...");
            serverHost = ENetLibrary.createHost(TEST_HOST, TEST_PORT, MAX_PEERS, CHANNEL_COUNT, 0, 0);
            assertTrue(serverHost != 0, "Server host creation failed");
            System.out.println("Server host created successfully at " + TEST_HOST + ":" + TEST_PORT);

            System.out.println("Creating client host...");
            clientHost = ENetLibrary.createHost(null, 0, 1, CHANNEL_COUNT, 0, 0);
            assertTrue(clientHost != 0, "Client host creation failed");
            System.out.println("Client host created successfully");

            System.out.println("Creating event object...");
            event = ENetLibrary.createEvent();
            assertTrue(event != 0, "Event creation failed");

            System.out.println("Connecting client to server at " + TEST_HOST + ":" + TEST_PORT);
            long peer = ENetLibrary.connect(clientHost, TEST_HOST, TEST_PORT, CHANNEL_COUNT, 0);
            assertTrue(peer != 0, "Client connection attempt failed");

            boolean connected = false;
            System.out.println("Waiting for connection events...");
            
            // Wait for connection event on both sides
            for (int i = 0; i < TIMEOUT && !connected; i++) {
                // Service client
                int clientResult = ENetLibrary.hostService(clientHost, event, 1);
                if (clientResult > 0) {
                    int eventType = ENetLibrary.getEventType(event);
                    System.out.println("Client received event type: " + eventType);
                    if (eventType == ENetLibrary.EVENT_TYPE_CONNECT) {
                        System.out.println("Client connected to server!");
                        connected = true;
                        break;
                    }
                }

                // Service server
                int serverResult = ENetLibrary.hostService(serverHost, event, 1);
                if (serverResult > 0) {
                    int eventType = ENetLibrary.getEventType(event);
                    System.out.println("Server received event type: " + eventType);
                    if (eventType == ENetLibrary.EVENT_TYPE_CONNECT) {
                        System.out.println("Server accepted client connection!");
                    }
                }
            }

            assertTrue(connected, "Connection failed");

            // Test sending data if connected
            if (connected) {
                System.out.println("Testing data transmission...");
                byte[] testData = "Hello, ENet!".getBytes();
                int sendResult = ENetLibrary.peerSend(peer, 0, testData);
                System.out.println("Send result: " + sendResult);

                // Wait for receive event
                boolean received = false;
                System.out.println("Waiting for data to be received...");
                for (int i = 0; i < TIMEOUT && !received; i++) {
                    int serverResult = ENetLibrary.hostService(serverHost, event, 1);
                    if (serverResult > 0) {
                        int eventType = ENetLibrary.getEventType(event);
                        System.out.println("Server received event type: " + eventType);
                        if (eventType == ENetLibrary.EVENT_TYPE_RECEIVE) {
                            byte[] receivedData = ENetLibrary.getEventPacketData(event);
                            System.out.println("Server received data: " + new String(receivedData));
                            received = true;
                            break;
                        }
                    }
                    // Also service the client to handle acknowledgments
                    int clientResult = ENetLibrary.hostService(clientHost, event, 1);
                    if (clientResult > 0) {
                        System.out.println("Client received event type: " + ENetLibrary.getEventType(event));
                    }
                }
                assertTrue(received, "Data transmission failed");
            }

        } finally {
            System.out.println("Cleaning up...");
            if (event != 0) ENetLibrary.destroyEvent(event);
            if (clientHost != 0) ENetLibrary.hostDestroy(clientHost);
            if (serverHost != 0) ENetLibrary.hostDestroy(serverHost);
        }
    }
} 