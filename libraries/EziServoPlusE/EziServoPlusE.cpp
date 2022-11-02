#include "EziServoPlusE.h"

byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 10);

EthernetClient client;
EthernetClient client1;

EziServoPlusE::EziServoPlusE()
{
}

EziServoPlusE::~EziServoPlusE()
{
}

bool EziServoPlusE::encode(char *des, const char *source, const uint8_t len)
{
    char *buffer = (char *)malloc(len + 1);

    memcpy((void *)buffer, (const void *)header, 1); // coppy 1 byte header vào vị trí 0 buffer.
    memcpy((void *)buffer + 1, (const void *)source, len);
    memcpy((void *)des, (const void *)buffer, len + 1);
    /*
    #ifdef DEBUG
        Debug_Serial.print("frame:");
        for (int i = 0; i < len + 1; i++)
        {
            Debug_Serial.print(buffer[i], HEX);
        }
    #endif
    */
    free(buffer);
    return true;
}
bool EziServoPlusE::decode(char *des, const char *source, const uint8_t len)
{
    if (source[0] == header[0])
    {
        memcpy((void *)des, (const void *)source + 5, len - 5);
        return true;
    }
    else
    {
        return false;
    }
}
bool EziServoPlusE::processData(IPAddress ip, char *des, const char *source, const uint8_t len_request, const uint8_t len_respond)
{
    if (ip == _ip1)
    {
        char buffer[256];
        char length_buffer = 0;
        if (client.connected())
        {
            client.write(source, len_request);
            delay(1);
            // Debug_Serial.println(len_request);
        }
        while (client.available())
        {
            char c = client.read();
            buffer[length_buffer++] = c;
        }
        memcpy((void *)des, (const void *)buffer, (size_t)(length_buffer));
    }
    if (ip == _ip2)
    {
        char buffer[256];
        char length_buffer = 0;
        if (client1.connected())
        {
            client1.write(source, len_request);
            delay(1);
            // Debug_Serial.println(len_request);
        }
        while (client1.available())
        {
            char c = client1.read();
            buffer[length_buffer++] = c;
        }
        memcpy((void *)des, (const void *)buffer, (size_t)(length_buffer));
    }
    // Udp.beginPacket(ip, localPort);
    // Udp.write(source, len_request);
    // Udp.endPacket();

    // int packetSize = Udp.parsePacket();
    // char packetBuffer[len_respond];
    // if (packetSize)
    //{
    /*
    #ifdef DEBUG
            Debug_Serial.print("Received packet of size ");
            Debug_Serial.println(packetSize);
            Debug_Serial.print("From ");
            IPAddress remote = Udp.remoteIP();
            for (int i = 0; i < 4; i++)
            {
                Debug_Serial.print(remote[i], DEC);
                if (i < 3)
                {
                    Debug_Serial.print(".");
                }
            }
            Debug_Serial.print(", port ");
            Debug_Serial.println(Udp.remotePort());
    #endif
    */
    // read the packet into packetBufffer
    // Udp.read(packetBuffer, len_respond);
    //    memcpy((void *)des, (const void *)packetBuffer, len_respond);
    /*
    #ifdef DEBUG
            for (int i = 0; i < packetSize; i++)
            {
                // Debug_Serial.print("Contents:");
                Debug_Serial.print(packetBuffer[i], HEX);
                Debug_Serial.print(" ");
            }
    #endif
    */
    //    return true;
    //}
    return true;
}

bool EziServoPlusE::initServo(IPAddress ip1, IPAddress ip2)
{
    // You can use Ethernet.init(pin) to configure the CS pin
    // Ethernet.init(10);  // Most Arduino shields
    // Ethernet.init(5);   // MKR ETH shield
    // Ethernet.init(0);   // Teensy 2.0
    // Ethernet.init(20);  // Teensy++ 2.0
    // Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
    // Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
    _ip1 = ip1;
    _ip2 = ip2;
    Ethernet.begin(mac, ip);
    /*
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        Debug_Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        while (true)
        {
            delay(1); // do nothing, no point running without Ethernet hardware
        }
        return false;
    }
    while (Ethernet.linkStatus() == LinkOFF)
    {
        Debug_Serial.println("Ethernet cable is not connected.");
        delay(500);
        return false;
    }
    */
    // give the Ethernet shield a second to initialize:
    delay(1000);

    while (!client.connected())
    {
        Debug_Serial.println("connecting server...");
        // if you get a connection, report back via serial:
        if (client.connect(ip1, 2001))
        {
            Debug_Serial.println("connected");
        }
        else
        {
            // if you didn't get a connection to the server:
            Debug_Serial.println("connection failed");
        }
    }
    while (!client1.connected())
    {
        Debug_Serial.println("connecting server1...");
        if (client1.connect(ip2, 2001))
        {
            Debug_Serial.println("connected_1");
        }
        else
        {
            // if you didn't get a connection to the server:
            Debug_Serial.println("connection failed _1");
        }
    }
    return true;
}

char EziServoPlusE::ServoEnable(IPAddress ip, bool state, uint8_t *Com_status)
{
    char *buffer = (char *)malloc(4 + sizeof(state) + 1);

    buffer[0] = this->length + sizeof(state);
    buffer[1] = this->syncNo++;
    buffer[2] = this->reserved;
    buffer[3] = FAS_ServoEnable;
    buffer[4] = (char)state;
    this->encode(buffer, (const char *)buffer, 5);

    char *buff_respond = (char *)malloc(5 + 1);
    this->processData(ip, buff_respond, (const char *)buffer, (const uint8_t)(5 + 1), (const uint8_t)(5 + 1));
    free(buffer);

    char *buff_rec = (char *)malloc(1);
    this->decode(buff_rec, (const char *)buff_respond, (const uint8_t)(6));
    free(buff_respond);
    memcpy((void *)Com_status, (const void *)&buff_rec, 1);
    free(buff_rec);
    return true;
}
/*
    malloc() cấp phát bộ nhớ động trả về dạng con trỏ void có thể ép kiểu bất cứ dữ liệu nào.
    free() giải phóng vùng nhớ.
*/
char EziServoPlusE::MoveStop(IPAddress ip, uint8_t *Com_status)
{
    char *buffer = (char *)malloc(4 + 1); // 4 byte ( header + length + syncNo + reserved) + 1 byte Frame type.

    buffer[0] = this->length;
    buffer[1] = this->syncNo++;
    buffer[2] = this->reserved;
    buffer[3] = FAS_MoveStop;
    this->encode(buffer, (const char *)buffer, 4);

    char *buff_respond = (char *)malloc(5 + 1); // 4 byte ( header + length + syncNo + reserved) + 1 byte Frame type + 1 byte communication Status.
    this->processData(ip, buff_respond, (const char *)buffer, (const uint8_t)(4 + 1), (const uint8_t)(5 + 1));
    free(buffer);

    char *buff_rec = (char *)malloc(1);
    this->decode(buff_rec, (const char *)buff_respond, (const uint8_t)(6));
    free(buff_respond);
    memcpy((void *)Com_status, (const void *)&buff_rec, 1);
    free(buff_rec);
    return true;
}

char EziServoPlusE::MoveSingleAxisAbsPos(IPAddress ip, int32_t posVal, uint32_t speed_pps, uint8_t *Com_status)
{
    char *buffer = (char *)malloc(4 + 8 + 1);

    buffer[0] = this->length + 8;
    buffer[1] = this->syncNo++;
    buffer[2] = this->reserved;
    buffer[3] = FAS_MoveSingleAxisAbsPos;
    buffer[4] = (char)posVal;
    buffer[5] = (char)(posVal >> 8);
    buffer[6] = (char)(posVal >> 16);
    buffer[7] = (char)(posVal >> 24);
    buffer[8] = (char)speed_pps;
    buffer[9] = (char)(speed_pps >> 8);
    buffer[10] = (char)(speed_pps >> 16);
    buffer[11] = (char)(speed_pps >> 24);
    this->encode(buffer, (const char *)buffer, 12);

    char *buff_respond = (char *)malloc(5 + 1);
    this->processData(ip, buff_respond, (const char *)buffer, (const uint8_t)(12 + 1), (const uint8_t)(5 + 1));
    free(buffer);

    char *buff_rec = (char *)malloc(1);
    this->decode(buff_rec, (const char *)buff_respond, (const uint8_t)(6));
    free(buff_respond);
    memcpy((void *)Com_status, (const void *)&buff_rec, 1);
    free(buff_rec);
    return true;
}

char EziServoPlusE::ServoAlarmReset(IPAddress ip, uint8_t *Com_status)
{
    char *buffer = (char *)malloc(4 + 1);

    buffer[0] = this->length;
    buffer[1] = this->syncNo++;
    buffer[2] = this->reserved;
    buffer[3] = FAS_ServoAlarmReset;
    this->encode(buffer, (const char *)buffer, 4);

    char *buff_respond = (char *)malloc(5 + 1); // malloc cấp phát vùng nhớ size buffer.
    this->processData(ip, buff_respond, (const char *)buffer, (const uint8_t)(4 + 1), (const uint8_t)(5 + 1));
    free(buffer);

    char *buff_rec = (char *)malloc(1);
    this->decode(buff_rec, (const char *)buff_respond, (const uint8_t)(6));
    free(buff_respond);
    memcpy((void *)Com_status, (const void *)&buff_rec, 1);
    free(buff_rec);
    return true;
}

char EziServoPlusE::EmergencyStop(IPAddress ip, uint8_t *Com_status)
{
    char *buffer = (char *)malloc(4 + 1);

    buffer[0] = this->length;
    buffer[1] = this->syncNo++;
    buffer[2] = this->reserved;
    buffer[3] = FAS_EmergencyStop;
    this->encode(buffer, (const char *)buffer, 4);

    char *buff_respond = (char *)malloc(5 + 1); // malloc cấp phát vùng nhớ size buffer.
    this->processData(ip, buff_respond, (const char *)buffer, (const uint8_t)(4 + 1), (const uint8_t)(5 + 1));
    free(buffer);

    char *buff_rec = (char *)malloc(1);
    this->decode(buff_rec, (const char *)buff_respond, (const uint8_t)(6));
    free(buff_respond);
    memcpy((void *)Com_status, (const void *)&buff_rec, 1);
    free(buff_rec);
    return true;
}

char EziServoPlusE::MoveVelocity(IPAddress ip, uint32_t speed_pps, bool jog_direction, uint8_t *Com_status)
{
    char *buffer = (char *)malloc(4 + 5 + 1);

    buffer[0] = this->length + 5;
    buffer[1] = this->syncNo++;
    buffer[2] = this->reserved;
    buffer[3] = FAS_MoveVelocity;
    buffer[4] = (char)speed_pps;
    buffer[5] = (char)(speed_pps >> 8);
    buffer[6] = (char)(speed_pps >> 16);
    buffer[7] = (char)(speed_pps >> 24);
    buffer[8] = (char)jog_direction;
    this->encode(buffer, (const char *)buffer, 9);

    char *buff_respond = (char *)malloc(5 + 1);
    this->processData(ip, buff_respond, (const char *)buffer, (const uint8_t)(9 + 1), (const uint8_t)(5 + 1));
    free(buffer);

    char *buff_rec = (char *)malloc(1);
    this->decode(buff_rec, (const char *)buff_respond, (const uint8_t)(6));
    free(buff_respond);
    memcpy((void *)Com_status, (const void *)&buff_rec, 1);
    free(buff_rec);
    return true;
}

char EziServoPlusE::VelocityOverride(IPAddress ip, uint32_t new_speed_cmd, uint8_t *Com_status)
{
    char *buffer = (char *)malloc(4 + 4 + 1);

    buffer[0] = this->length + 4;
    buffer[1] = this->syncNo++;
    buffer[2] = this->reserved;
    buffer[3] = FAS_VelocityOverride;
    buffer[4] = (char)new_speed_cmd;
    buffer[5] = (char)(new_speed_cmd >> 8);
    buffer[6] = (char)(new_speed_cmd >> 16);
    buffer[7] = (char)(new_speed_cmd >> 24);

    this->encode(buffer, (const char *)buffer, 8);

    char *buff_respond = (char *)malloc(5 + 1);
    this->processData(ip, buff_respond, (const char *)buffer, (const uint8_t)(8 + 1), (const uint8_t)(5 + 1));
    free(buffer);

    char *buff_rec = (char *)malloc(1);
    this->decode(buff_rec, (const char *)buff_respond, (const uint8_t)(6));
    free(buff_respond);
    memcpy((void *)Com_status, (const void *)&buff_rec, 1);
    free(buff_rec);
    return true;
}

char EziServoPlusE::GetActualPos(IPAddress ip, int32_t *cmdPosValueRt, uint8_t *Com_status)
{
    char *buffer = (char *)malloc(4 + 1); // 4 byte ( header + length + syncNo + reserved) + 1 byte Frame type.

    buffer[0] = this->length;
    buffer[1] = this->syncNo++;
    buffer[2] = this->reserved;
    buffer[3] = FAS_GetActualPos;
    this->encode(buffer, (const char *)buffer, 4);

    char *buff_respond = (char *)malloc(5 + 1 + 4); // 4 byte ( header + length + syncNo + reserved) + 1 byte Frame type + 1 byte communication Status.
    this->processData(ip, buff_respond, (const char *)buffer, (const uint8_t)(4 + 1), (const uint8_t)(5 + 1 + 4));
    free(buffer);

    char *buff_rec = (char *)malloc(5);
    this->decode(buff_rec, (const char *)buff_respond, (const uint8_t)(10));
    free(buff_respond);
    memcpy((void *)cmdPosValueRt, (const void *)&buff_rec[1], 4);
    /*
    Debug_Serial.println("start");
    Debug_Serial.println(ip);
    Debug_Serial.println(buff_rec[0], HEX);
    Debug_Serial.println(buff_rec[1], HEX);
    Debug_Serial.println(buff_rec[2], HEX);
    Debug_Serial.println(buff_rec[3], HEX);
    Debug_Serial.println(buff_rec[4], HEX);
    Debug_Serial.println("stop");
    */
    free(buff_rec);
    return true;
}