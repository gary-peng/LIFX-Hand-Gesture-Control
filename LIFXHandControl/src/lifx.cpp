#include "lifx.h"

lifx::lifx() {

}

void lifx::setUDP(UDP &udpRef) {
    _Udp = udpRef;
}

void lifx::setBroadcastIP(IPAddress broadcastIP) {
    _broadcastIP = broadcastIP;
}

void lifx::getStatus() {
    /* header */
    Header header = Header();
    int16_t headerSize = sizeof(header);

    /* payload */
    // N/A

    /* UDP Packet */
    uint8_t udpPacket[headerSize];

    /* build header */
    header.size = headerSize;
    //header.origin = 0;
    header.tagged = 0;
    header.addressable = 1;
    header.protocol = 1024;
    header.source = _myID;
    header.target[0] = 0xD0;
    header.target[1] = 0x73;
    header.target[2] = 0xD5;
    header.target[3] = 0x23;
    header.target[4] = 0x0C;
    header.target[5] = 0x36;
    //header.target[6] = 0;
    //header.target[7] = 0;
    //header.reservedA[0] = 0;
    //header.reservedA[1] = 0;
    //header.reservedA[2] = 0;
    //header.reservedA[3] = 0;
    //header.reservedA[4] = 0;
    //header.reservedA[5] = 0;
    //header.reservedB = 0;
    //header.ack_required = 0;
    //header.res_required = 0;
    //header.sequence = 0;
    //header.reservedC = 0;
    header.type = 101;
    //header.reservedD = 0;

    /* build udp packet */
    /* header */
    memcpy(&udpPacket, &header, headerSize);

    _Udp.beginPacket(_broadcastIP, lifx_port);
    _Udp.write(udpPacket, sizeof(udpPacket));
    _Udp.endPacket();
    msg_sent = true;
    _msgSentTime = millis();
}

// Power
void lifx::getPower() {
    /* header */
    Header header = Header();
    int headerSize = sizeof(header);

    /* payload */
    // N/A

    /* UDP Packet */
    uint8_t udpPacket[headerSize];

    /* build header */
    header.size = headerSize;
    //header.origin = 0;
    header.tagged = 1;
    header.addressable = 1;
    header.protocol = 1024;
    header.source = _myID;
    //header.target[0] = 0;
    //header.target[1] = 0;
    //header.target[2] = 0;
    //header.target[3] = 0;
    //header.target[4] = 0;
    //header.target[5] = 0;
    //header.target[6] = 0;
    //header.target[7] = 0;
    //header.reservedA[0] = 0;
    //header.reservedA[1] = 0;
    //header.reservedA[2] = 0;
    //header.reservedA[3] = 0;
    //header.reservedA[4] = 0;
    //header.reservedA[5] = 0;
    //header.reservedB = 0;
    //header.ack_required = 0;
    //header.res_required = 0;
    //header.sequence = 0;
    //header.reservedC = 0;
    header.type = 20;
    //header.reservedD = 0;

    /* build payload */
    // N/A

    /* build udp packet */
    memcpy(&udpPacket, &header, headerSize);

    // Send a packet on startup
    _Udp.beginPacket(_broadcastIP, lifx_port);
    _Udp.write(udpPacket, sizeof(udpPacket));
    _Udp.endPacket();
    msg_sent = true;
    _msgSentTime = millis();
}

void lifx::setPower(uint16_t level) {
    /* header */
    Header header = Header();
    int16_t headerSize = sizeof(header);

    /* payload */
    uint8_t payload[6];
    int16_t payloadSize = sizeof(payload);
    //_lamp.level = 63353;

    /* UDP Packet */
    uint8_t udpPacket[headerSize + payloadSize];

    /* build header */
    header.size = headerSize + payloadSize;
    //header.origin = 0;
    header.tagged = 0;
    header.addressable = 1;
    header.protocol = 1024;
    header.source = _myID;
    header.target[0] = 0xD0;
    header.target[1] = 0x73;
    header.target[2] = 0xD5;
    header.target[3] = 0x23;
    header.target[4] = 0x0C;
    header.target[5] = 0x36;
    //header.target[6] = 0;
    //header.target[7] = 0;
    //header.reservedA[0] = 0;
    //header.reservedA[1] = 0;
    //header.reservedA[2] = 0;
    //header.reservedA[3] = 0;
    //header.reservedA[4] = 0;
    //header.reservedA[5] = 0;
    //header.reservedB = 0;
    //header.ack_required = 0;
    header.res_required = 1;
    //header.sequence = 0;
    //header.reservedC = 0;
    header.type = 117;
    //header.reservedD = 0;

    /* build payload */
    /* Level, 0 or 65535 uint16_t */
    payload[0] = (level) & 0xff;
    payload[1] = (level >> 8) & 0xff;
    /* Duration, transition time in milliseconds uint32_t */
    payload[2] = (0) & 0xff;
    payload[3] = (0 >> 8) & 0xff;
    payload[4] = (0 >> 16) & 0xff;
    payload[5] = (0 >> 24) & 0xff;

    /* build udp packet */
    /* header */
    memcpy(&udpPacket, &header, headerSize);
    /* payload */
    for (uint8_t i = 0; i < payloadSize; i++) {
        udpPacket[headerSize + i] = payload[i];
    }

    // Send a packet on startup
    _Udp.beginPacket(_broadcastIP, lifx_port);
    _Udp.write(udpPacket, sizeof(udpPacket));
    _Udp.endPacket();
    msg_sent = true;
    _msgSentTime = millis();
}

void lifx::togglePower() {
    getPower();

    _level = _level == 65535 ? 0 : 65535;

    /* header */
    Header header = Header();
    int16_t headerSize = sizeof(header);

    /* payload */
    uint8_t payload[6];
    int16_t payloadSize = sizeof(payload);
    //_lamp.level = 63353;

    /* UDP Packet */
    uint8_t udpPacket[headerSize + payloadSize];

    /* build header */
    header.size = headerSize + payloadSize;
    //header.origin = 0;
    header.tagged = 0;
    header.addressable = 1;
    header.protocol = 1024;
    header.source = _myID;
    header.target[0] = 0xD0;
    header.target[1] = 0x73;
    header.target[2] = 0xD5;
    header.target[3] = 0x23;
    header.target[4] = 0x0C;
    header.target[5] = 0x36;
    //header.target[6] = 0;
    //header.target[7] = 0;
    //header.reservedA[0] = 0;
    //header.reservedA[1] = 0;
    //header.reservedA[2] = 0;
    //header.reservedA[3] = 0;
    //header.reservedA[4] = 0;
    //header.reservedA[5] = 0;
    //header.reservedB = 0;
    //header.ack_required = 0;
    header.res_required = 1;
    //header.sequence = 0;
    //header.reservedC = 0;
    header.type = 117;
    //header.reservedD = 0;

    /* build payload */
    /* Level, 0 or 65535 uint16_t */
    payload[0] = (_level) & 0xff;
    payload[1] = (_level >> 8) & 0xff;
    /* Duration, transition time in milliseconds uint32_t */
    payload[2] = (0) & 0xff;
    payload[3] = (0 >> 8) & 0xff;
    payload[4] = (0 >> 16) & 0xff;
    payload[5] = (0 >> 24) & 0xff;

    /* build udp packet */
    /* header */
    memcpy(&udpPacket, &header, headerSize);
    /* payload */
    for (uint8_t i = 0; i < payloadSize; i++) {
        udpPacket[headerSize + i] = payload[i];
    }

    // Send a packet on startup
    _Udp.beginPacket(_broadcastIP, lifx_port);
    _Udp.write(udpPacket, sizeof(udpPacket));
    _Udp.endPacket();
    msg_sent = true;
    _msgSentTime = millis();
}

// Color
void lifx::setColor(uint16_t hue, uint16_t saturation, uint16_t brightness, uint16_t kelvin, uint32_t duration) {
    /* header */
    Header header = Header();
    int16_t headerSize = sizeof(header);

    /* payload */
    uint8_t payload[13];
    int16_t payloadSize = sizeof(payload);
    _hue = hue;
    _sat = saturation;
    _bri = brightness;
    _kelv = kelvin;

    /* UDP Packet */
    uint8_t udpPacket[headerSize + payloadSize];

    /* build header */
    header.size = headerSize  + payloadSize;
    //header.origin = 0;
    header.tagged = 0;
    header.addressable = 1;
    header.protocol = 1024;
    header.source = _myID;
    header.target[0] = 0xD0;
    header.target[1] = 0x73;
    header.target[2] = 0xD5;
    header.target[3] = 0x23;
    header.target[4] = 0x0C;
    header.target[5] = 0x36;
    header.target[6] = 0;
    header.target[7] = 0;
    //header.reservedA[0] = 0;
    //header.reservedA[1] = 0;
    //header.reservedA[2] = 0;
    //header.reservedA[3] = 0;
    //header.reservedA[4] = 0;
    //header.reservedA[5] = 0;
    //header.reservedB = 0;
    //header.ack_required = 0;
    //header.res_required = 0;
    //header.sequence = 0;
    //header.reservedC = 0;
    header.type = 102;
    //header.reservedD = 0;

    /* build payload */
    /* Level, 0 - 65535 uint16_t */
    payload[0] = (_level) & 0xff;
    /* HSBK - Hue 0 - 65535 uint16_t */
    payload[1] = (_hue) & 0xff;
    payload[2] = (_hue >> 8) & 0xff;
    /* HSBK - Saturation 0 - 65535 uint16_t */
    payload[3] = (_sat) & 0xff;
    payload[4] = (_sat >> 8) & 0xff;
    /* HSBK - Brightness 0 - 65535 uint16_t */
    payload[5] = (_bri) & 0xff;
    payload[6] = (_bri >> 8) & 0xff;
    /* HSBK - Kelvin 2500 - 9000 uint16_t */
    payload[7] = (_kelv) & 0xff;
    payload[8] = (_kelv >> 8) & 0xff;
    /* Duration, transition time in milliseconds uint32_t */
    payload[9] = (duration) & 0xff;
    payload[10] = (duration >> 8) & 0xff;
    payload[11] = (duration >> 16) & 0xff;
    payload[12] = (duration >> 24) & 0xff;

    /* build udp packet */
    /* header */
    memcpy(&udpPacket, &header, headerSize);
    /* payload */
    for (uint8_t i = 0; i < payloadSize; i++) {
        udpPacket[headerSize + i] = payload[i];
    }

    // Send a packet on startup
    _Udp.beginPacket(_broadcastIP, lifx_port);
    _Udp.write(udpPacket, sizeof(udpPacket));
    _Udp.endPacket();
    msg_sent = true;
    _msgSentTime = millis();
}

void lifx::setBrightness(uint16_t brightness) {
    getStatus();
    /* header */
    Header header = Header();
    int16_t headerSize = sizeof(header);

    /* payload */
    uint8_t payload[13];
    int16_t payloadSize = sizeof(payload);
    _bri = brightness;

    /* UDP Packet */
    uint8_t udpPacket[headerSize + payloadSize];

    /* build header */
    header.size = headerSize  + payloadSize;
    //header.origin = 0;
    header.tagged = 0;
    header.addressable = 1;
    header.protocol = 1024;
    header.source = _myID;
    header.target[0] = 0xD0;
    header.target[1] = 0x73;
    header.target[2] = 0xD5;
    header.target[3] = 0x23;
    header.target[4] = 0x0C;
    header.target[5] = 0x36;
    header.target[6] = 0;
    header.target[7] = 0;
    //header.reservedA[0] = 0;
    //header.reservedA[1] = 0;
    //header.reservedA[2] = 0;
    //header.reservedA[3] = 0;
    //header.reservedA[4] = 0;
    //header.reservedA[5] = 0;
    //header.reservedB = 0;
    //header.ack_required = 0;
    //header.res_required = 0;
    //header.sequence = 0;
    //header.reservedC = 0;
    header.type = 102;
    //header.reservedD = 0;

    /* build payload */
    /* Level, 0 - 65535 uint16_t */
    payload[0] = (_level) & 0xff;
    /* HSBK - Hue 0 - 65535 uint16_t */
    payload[1] = (_hue) & 0xff;
    payload[2] = (_hue >> 8) & 0xff;
    /* HSBK - Saturation 0 - 65535 uint16_t */
    payload[3] = (_sat) & 0xff;
    payload[4] = (_sat >> 8) & 0xff;
    /* HSBK - Brightness 0 - 65535 uint16_t */
    payload[5] = (_bri) & 0xff;
    payload[6] = (_bri >> 8) & 0xff;
    /* HSBK - Kelvin 2500 - 9000 uint16_t */
    payload[7] = (_kelv) & 0xff;
    payload[8] = (_kelv >> 8) & 0xff;
    /* Duration, transition time in milliseconds uint32_t */
    payload[9] = (0) & 0xff;
    payload[10] = (0 >> 8) & 0xff;
    payload[11] = (0 >> 16) & 0xff;
    payload[12] = (0 >> 24) & 0xff;

    /* build udp packet */
    /* header */
    memcpy(&udpPacket, &header, headerSize);
    /* payload */
    for (uint8_t i = 0; i < payloadSize; i++) {
        udpPacket[headerSize + i] = payload[i];
    }

    // Send a packet on startup
    _Udp.beginPacket(_broadcastIP, lifx_port);
    _Udp.write(udpPacket, sizeof(udpPacket));
    _Udp.endPacket();
    msg_sent = true;
    _msgSentTime = millis();
}

// Feedback
void lifx::msgIn(byte packetBuffer[128]) {
    /*
        is it a lifx msg
        is it for me (senders mac)
        Check for return type Status
        send to light status function
    */
    // LIFX Header Decode
    // Frame
    uint16_t lifxSize       = packetBuffer[0] + (packetBuffer[1] << 8); //little endian
    uint8_t lifxOrigin      = ((packetBuffer[2] + (packetBuffer[3] << 8)) & 0xC000) >> 14;
    bool lifxTagged         = ((packetBuffer[2] + (packetBuffer[3] << 8)) & 0x2000) >> 13;
    bool lifxAddressable    = ((packetBuffer[2] + (packetBuffer[3] << 8)) & 0x1000) >> 12;
    uint16_t lifxProtocol   = ((packetBuffer[2] + (packetBuffer[3] << 8)) & 0x0FFF);
    uint32_t lifxSource     = packetBuffer[4] + (packetBuffer[5] << 8) + (packetBuffer[6] << 16) + (packetBuffer[7] << 24);
    // Frame Address
    uint8_t lifxTarget[8]   = {packetBuffer[8], packetBuffer[9], packetBuffer[10], packetBuffer[11], packetBuffer[12], packetBuffer[13], packetBuffer[14], packetBuffer[15]};
    uint8_t lifxReservedA[6]= {packetBuffer[16], packetBuffer[17], packetBuffer[18], packetBuffer[19], packetBuffer[20], packetBuffer[21]};
    uint8_t lifxReservedB   = (packetBuffer[22] & 0xFC) >> 2;
    bool lifxAckRequired    = (packetBuffer[22] & 0x02) >> 1;
    bool lifxResRequired    = packetBuffer[22] & 0x01;
    uint8_t lifxSequence    = packetBuffer[23];
    // Protocol Header
    uint64_t lifxReservedC  = (packetBuffer[24] + (packetBuffer[25] << 4) + (packetBuffer[26] << 8) + (packetBuffer[27] << 12)) + ((packetBuffer[28] + (packetBuffer[29] << 4) + (packetBuffer[30] << 8) + (packetBuffer[31] << 12)) << 16);
    uint16_t lifxPacketType = packetBuffer[32] + (packetBuffer[33] << 8);
    uint16_t lifxReservedD  = packetBuffer[34] + (packetBuffer[35] << 8);

    // Serial.printlnf("   PacketType: %d", lifxPacketType);

        #if (_DEBUG > 2)
            Serial.println("Header:");
            Serial.println("  Frame:");
            Serial.printlnf("   Size: %d", lifxSize);
            Serial.printlnf("   Origin: %d", lifxOrigin);
            Serial.printlnf("   Tagged: %s", lifxTagged ? "true" : "false");
            Serial.printlnf("   Addressable: %s", lifxAddressable ? "true" : "false");
            Serial.printlnf("   Protocol: %d", lifxProtocol);
            Serial.printlnf("   Source: %d", lifxSource);
            Serial.println("  Frame Address:");
            Serial.printlnf("   Target: 0x %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X", lifxTarget[0], lifxTarget[1], lifxTarget[2], lifxTarget[3], lifxTarget[4], lifxTarget[5], lifxTarget[6], lifxTarget[7]);
            Serial.printlnf("   ReservedA: 0x %02X %02X %02X %02X %02X %02X", lifxReservedA[0], lifxReservedA[1], lifxReservedA[2], lifxReservedA[3], lifxReservedA[4], lifxReservedA[5]);
            Serial.printlnf("   ReservedB: %d", lifxReservedB);
            Serial.printlnf("   AckRequired: %s", lifxAckRequired ? "true" : "false");
            Serial.printlnf("   ResRequired: %s", lifxResRequired ? "true" : "false");
            Serial.printlnf("   Sequence: %d", lifxSequence);
            Serial.println("  Protocol Header:");
            Serial.printlnf("   ReservedA: %d", lifxReservedC);
            Serial.printlnf("   PacketType: %d", lifxPacketType);
            Serial.printlnf("   ReservedD: %d", lifxReservedD);

            Serial.println("Payload:");
        #endif

    switch (lifxPacketType) {
        case 107: { // Sent by a device to provide the current light state.
          //HSBK _hsbk;
          //int16_t _reservedA;
          //uint16_t _power;
          //String _lable;
          //uint64_t _reservedB;
          //uint8_t _mac[6] = {lifxTarget[0], lifxTarget[1], lifxTarget[2], lifxTarget[3], lifxTarget[4], lifxTarget[5]};

          _hue = packetBuffer[36] + (packetBuffer[37] << 8);
          _sat = packetBuffer[38] + (packetBuffer[39] << 8);
          _bri = packetBuffer[40] + (packetBuffer[41] << 8);
          _kelv = packetBuffer[42] + (packetBuffer[43] << 8);
          //_reservedA          =   packetBuffer[44] + (packetBuffer[45] << 8);
          _level = packetBuffer[46] + (packetBuffer[47] << 8);
          //_lable              =   packetBuffer[48] + (packetBuffer[49] << 8) + (packetBuffer[50] << 16) + (packetBuffer[51] << 24) + (packetBuffer[52] << 32) + (packetBuffer[53] << 40) + (packetBuffer[54] << 48) + (packetBuffer[55] << 56) + (packetBuffer[56] << 64) + (packetBuffer[57] << 72) + (packetBuffer[58] << 80) + (packetBuffer[59] << 88) + (packetBuffer[60] << 96) + (packetBuffer[61] << 104) + (packetBuffer[62] << 112) + (packetBuffer[63] << 120) + (packetBuffer[64] << 128) + (packetBuffer[65] << 136) + (packetBuffer[66] << 144) + (packetBuffer[67] << 152) + (packetBuffer[68] << 160) + (packetBuffer[69] << 168) + (packetBuffer[70] << 176) + (packetBuffer[71] << 184) + (packetBuffer[72] << 192) + (packetBuffer[73] << 200) + (packetBuffer[74] << 208) + (packetBuffer[75] << 216) + (packetBuffer[76] << 224) + (packetBuffer[77] << 232) + (packetBuffer[78] << 240) + (packetBuffer[79] << 248);
          //_reservedB =	packetBuffer[80] + (packetBuffer[81] << 8) + (packetBuffer[82] << 16) + (packetBuffer[83] << 24) + (packetBuffer[84] << 32) + (packetBuffer[85] << 40) + (packetBuffer[86] << 48) + (packetBuffer[87] << 56);

          Serial.printlnf("Get Statues");
          Serial.printlnf("   Hue: %0.2f", (float)(_hue / (65535 / 359)));
          Serial.printlnf("   Saturation: %0.2f", ((float)_sat / 65535.00) * 100.00);
          Serial.printlnf("   Brightness: %0.2f", ((float)_bri / 65535.00) * 100.00);
          Serial.printlnf("   Kelvin: %d", _kelv);
          Serial.printlnf("   Level: %d", _level);

          break;
        }

        case 22: {
          _level = packetBuffer[36] + (packetBuffer[37] << 8);

          Serial.printlnf("Get Power");
          Serial.printlnf("Level: %d", _level);

          break;
        }
    }
}
