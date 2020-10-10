#include "application.h"
#include "lifx.h"

SYSTEM_MODE(AUTOMATIC);

// LIFX
lifx LIFX;
bool lifx_on = false;

UDP Udp;
uint16_t udp_packet_size = 0;
IPAddress broadcastIP(255, 255, 255, 255);

// Ultrasonic Sensor
const int trigPin = 1;
const int echoPin = 2;

long duration;
int distance;

// Control
uint16_t bri;
uint16_t lastBri;
long pollInterv = 100;
long lastPoll;
bool off;

void doHand() {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    distance= duration*0.034/2;

    if (millis() - lastPoll >= pollInterv) {
        if (distance <= 50)
            bri = distance*1310;

        if (lastBri != bri) {
            LIFX.setBrightness(bri);

        Serial.print("Brightness: ");
        Serial.println(bri);

        if (bri < 4000) {
            LIFX.setPower(0);
            off = true;
        }

        if (bri >= 4000 && off == true) {
            LIFX.setPower(65535);
            off = false;
        }
    }

    lastBri = bri;
    lastPoll = millis();
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // 4092
    Udp.begin(56700);
    Udp.joinMulticast(broadcastIP);
    LIFX.setUDP(Udp);
    LIFX.setBroadcastIP(broadcastIP);
    Serial.println("Initializing UDP");

    LIFX.getStatus();
}

void loop() {
    doHand();

    if ((LIFX.msg_sent == true) && (millis() - LIFX._msgSentTime >= 1000)) {
        // Serial.printlnf("%lu - Status update...", now);
        LIFX.getStatus();
        LIFX.msg_sent = false;
    }

    // Check if data has been received
    udp_packet_size = Udp.parsePacket();
    if (udp_packet_size > 0) {
    byte packetBuffer[128]; //buffer to hold incoming packet

    // Read first 128 of data received
    Udp.read(packetBuffer, 128);

    // Ignore other chars
    Udp.flush();

    // Translate data
    LIFX.msgIn(packetBuffer);
    }
}