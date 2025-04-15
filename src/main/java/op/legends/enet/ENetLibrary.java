package op.legends.enet;

public class ENetLibrary {
    public static final int EVENT_TYPE_NONE = 0;
    public static final int EVENT_TYPE_CONNECT = 1;
    public static final int EVENT_TYPE_DISCONNECT = 2;
    public static final int EVENT_TYPE_RECEIVE = 3;

    static {
        System.loadLibrary("enetjni");
    }

    public static native int initialize();
    public static native void deinitialize();
    
    public static native long createHost(String address, int port, int peerCount, int channelLimit, long incomingBandwidth, long outgoingBandwidth);
    public static native void hostDestroy(long host);
    public static native int hostService(long host, long event, int timeout);
    
    public static native long createEvent();
    public static native void destroyEvent(long event);
    public static native int getEventType(long event);
    public static native long getEventPeer(long event);
    public static native int getEventChannelID(long event);
    public static native long getEventData(long event);
    public static native byte[] getEventPacketData(long event);
    
    public static native long connect(long host, String address, int port, int channelCount, long data);
    public static native void peerDisconnect(long peer, long data);
    public static native void peerDisconnectNow(long peer, long data);
    public static native void peerReset(long peer);
    
    public static native int peerSend(long peer, int channelID, byte[] data);
    public static native long createPacket(byte[] data, int flags);
    public static native void destroyPacket(long packet);
    
    public static native int getPeerState(long peer);
    public static native long getPeerRoundTripTime(long peer);
    public static native long getPeerLastRoundTripTime(long peer);
    public static native long getPeerLastSendTime(long peer);
    public static native long getPeerLastReceiveTime(long peer);
    public static native long getPeerPacketsSent(long peer);
    public static native long getPeerPacketsLost(long peer);
} 