#include <jni.h>
#include <enet/enet.h>
#include "op_legends_enet_ENetLibrary.h"

extern "C" {

JNIEXPORT jint JNICALL Java_op_legends_enet_ENetLibrary_initialize(JNIEnv* env, jclass cls) {
    return enet_initialize();
}

JNIEXPORT void JNICALL Java_op_legends_enet_ENetLibrary_deinitialize(JNIEnv* env, jclass cls) {
    enet_deinitialize();
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_createHost(JNIEnv* env, jclass cls, jstring address, jint port, jint peerCount, jint channelLimit, jlong incomingBandwidth, jlong outgoingBandwidth) {
    ENetAddress addr;
    ENetHost* host;
    
    if (address != nullptr) {
        const char* addressStr = env->GetStringUTFChars(address, nullptr);
        enet_address_set_host(&addr, addressStr);
        env->ReleaseStringUTFChars(address, addressStr);
    }
    addr.port = static_cast<enet_uint16>(port);
    
    host = enet_host_create(address != nullptr ? &addr : nullptr, 
                          static_cast<size_t>(peerCount), 
                          static_cast<size_t>(channelLimit), 
                          static_cast<enet_uint32>(incomingBandwidth), 
                          static_cast<enet_uint32>(outgoingBandwidth));
    return reinterpret_cast<jlong>(host);
}

JNIEXPORT void JNICALL Java_op_legends_enet_ENetLibrary_hostDestroy(JNIEnv* env, jclass cls, jlong host) {
    enet_host_destroy(reinterpret_cast<ENetHost*>(host));
}

JNIEXPORT jint JNICALL Java_op_legends_enet_ENetLibrary_hostService(JNIEnv* env, jclass cls, jlong host, jlong event, jint timeout) {
    return enet_host_service(reinterpret_cast<ENetHost*>(host), reinterpret_cast<ENetEvent*>(event), timeout);
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_createEvent(JNIEnv* env, jclass cls) {
    ENetEvent* event = new ENetEvent();
    return reinterpret_cast<jlong>(event);
}

JNIEXPORT void JNICALL Java_op_legends_enet_ENetLibrary_destroyEvent(JNIEnv* env, jclass cls, jlong event) {
    delete reinterpret_cast<ENetEvent*>(event);
}

JNIEXPORT jint JNICALL Java_op_legends_enet_ENetLibrary_getEventType(JNIEnv* env, jclass cls, jlong event) {
    return reinterpret_cast<ENetEvent*>(event)->type;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_getEventPeer(JNIEnv* env, jclass cls, jlong event) {
    return reinterpret_cast<jlong>(reinterpret_cast<ENetEvent*>(event)->peer);
}

JNIEXPORT jint JNICALL Java_op_legends_enet_ENetLibrary_getEventChannelID(JNIEnv* env, jclass cls, jlong event) {
    return reinterpret_cast<ENetEvent*>(event)->channelID;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_getEventData(JNIEnv* env, jclass cls, jlong event) {
    return reinterpret_cast<ENetEvent*>(event)->data;
}

JNIEXPORT jbyteArray JNICALL Java_op_legends_enet_ENetLibrary_getEventPacketData(JNIEnv* env, jclass cls, jlong event) {
    ENetEvent* evt = reinterpret_cast<ENetEvent*>(event);
    if (evt->packet == nullptr) return nullptr;
    
    jbyteArray result = env->NewByteArray(static_cast<jsize>(evt->packet->dataLength));
    env->SetByteArrayRegion(result, 0, static_cast<jsize>(evt->packet->dataLength), reinterpret_cast<jbyte*>(evt->packet->data));
    return result;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_connect(JNIEnv* env, jclass cls, jlong host, jstring address, jint port, jint channelCount, jlong data) {
    ENetAddress addr;
    const char* addressStr = env->GetStringUTFChars(address, nullptr);
    enet_address_set_host(&addr, addressStr);
    env->ReleaseStringUTFChars(address, addressStr);
    addr.port = static_cast<enet_uint16>(port);
    
    ENetPeer* peer = enet_host_connect(reinterpret_cast<ENetHost*>(host), &addr, static_cast<size_t>(channelCount), static_cast<enet_uint32>(data));
    return reinterpret_cast<jlong>(peer);
}

JNIEXPORT void JNICALL Java_op_legends_enet_ENetLibrary_peerDisconnect(JNIEnv* env, jclass cls, jlong peer, jlong data) {
    enet_peer_disconnect(reinterpret_cast<ENetPeer*>(peer), static_cast<enet_uint32>(data));
}

JNIEXPORT void JNICALL Java_op_legends_enet_ENetLibrary_peerDisconnectNow(JNIEnv* env, jclass cls, jlong peer, jlong data) {
    enet_peer_disconnect_now(reinterpret_cast<ENetPeer*>(peer), static_cast<enet_uint32>(data));
}

JNIEXPORT void JNICALL Java_op_legends_enet_ENetLibrary_peerReset(JNIEnv* env, jclass cls, jlong peer) {
    enet_peer_reset(reinterpret_cast<ENetPeer*>(peer));
}

JNIEXPORT jint JNICALL Java_op_legends_enet_ENetLibrary_peerSend(JNIEnv* env, jclass cls, jlong peer, jint channelID, jbyteArray data) {
    jsize length = env->GetArrayLength(data);
    jbyte* bytes = env->GetByteArrayElements(data, nullptr);
    
    ENetPacket* packet = enet_packet_create(bytes, static_cast<size_t>(length), ENET_PACKET_FLAG_RELIABLE);
    int result = enet_peer_send(reinterpret_cast<ENetPeer*>(peer), static_cast<enet_uint8>(channelID), packet);
    
    env->ReleaseByteArrayElements(data, bytes, JNI_ABORT);
    return result;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_createPacket(JNIEnv* env, jclass cls, jbyteArray data, jint flags) {
    jsize length = env->GetArrayLength(data);
    jbyte* bytes = env->GetByteArrayElements(data, nullptr);
    
    ENetPacket* packet = enet_packet_create(bytes, static_cast<size_t>(length), flags);
    env->ReleaseByteArrayElements(data, bytes, JNI_ABORT);
    
    return reinterpret_cast<jlong>(packet);
}

JNIEXPORT void JNICALL Java_op_legends_enet_ENetLibrary_destroyPacket(JNIEnv* env, jclass cls, jlong packet) {
    enet_packet_destroy(reinterpret_cast<ENetPacket*>(packet));
}

JNIEXPORT jint JNICALL Java_op_legends_enet_ENetLibrary_getPeerState(JNIEnv* env, jclass cls, jlong peer) {
    return reinterpret_cast<ENetPeer*>(peer)->state;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_getPeerRoundTripTime(JNIEnv* env, jclass cls, jlong peer) {
    return reinterpret_cast<ENetPeer*>(peer)->roundTripTime;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_getPeerLastRoundTripTime(JNIEnv* env, jclass cls, jlong peer) {
    return reinterpret_cast<ENetPeer*>(peer)->lastRoundTripTime;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_getPeerLastSendTime(JNIEnv* env, jclass cls, jlong peer) {
    return reinterpret_cast<ENetPeer*>(peer)->lastSendTime;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_getPeerLastReceiveTime(JNIEnv* env, jclass cls, jlong peer) {
    return reinterpret_cast<ENetPeer*>(peer)->lastReceiveTime;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_getPeerPacketsSent(JNIEnv* env, jclass cls, jlong peer) {
    return reinterpret_cast<ENetPeer*>(peer)->packetsSent;
}

JNIEXPORT jlong JNICALL Java_op_legends_enet_ENetLibrary_getPeerPacketsLost(JNIEnv* env, jclass cls, jlong peer) {
    return reinterpret_cast<ENetPeer*>(peer)->packetsLost;
}

} 