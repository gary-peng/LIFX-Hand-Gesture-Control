/*
 * lifx.h - library for talking to the light blubs.
 */
#ifndef _lifx_h
    #define _lifx_h

    /* includes */
    #include "application.h"

    class lifx
    {
        public:
        	/* Members Functions */
        	lifx();

          void addLight();

          void setUDP(UDP &udpRef);
          void setBroadcastIP(IPAddress broadcastIP);

          void msgIn(byte packetBuffer[128]);
          //void setPower(uint16_t level);
          void getPower();
          void getStatus();

          void setPower(uint16_t level);
          void togglePower();
          void setColor(uint16_t hue, uint16_t saturation, uint16_t brightness, uint16_t kelvin, uint32_t duration);
          void setBrightness(uint16_t brightness);

        	/* Members */
          struct Header
          {
              // frame
              uint16_t size;
              uint16_t protocol:12;
              uint8_t  addressable:1;
              uint8_t  tagged:1;
              uint8_t  origin:2;
              uint32_t source;
              // frame address
              uint8_t  target[8];
              uint8_t  reservedA[6];
              uint8_t  res_required:1;
              uint8_t  ack_required:1;
              uint8_t  reservedB:6;
              uint8_t  sequence;
              // protocol header
              uint64_t reservedC;
              uint16_t type;
              uint16_t reservedD;
          }__attribute__ ((__packed__));

          bool msg_sent;
          uint32_t _msgSentTime;

        private:
        	/* Members Functions */

        	/* Members */
          IPAddress _broadcastIP;
          UDP _Udp;
          uint16_t lifx_port = 56700;
          uint32_t _myID = 0;

          uint16_t _level;
          uint16_t _hue;
          uint16_t _sat;
          uint16_t _bri;
          uint16_t _kelv;
          uint16_t _hsb_increments = 65535 / 20;
          //uint16_t _change_hsbk(uint16_t _hsbk, bool _add, uint16_t _increments, uint16_t _max, uint16_t _min);

          int groups[];
          uint8_t lights[];
    };

#endif
