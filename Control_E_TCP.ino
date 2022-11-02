#include "EziServoPlusE.h"

IPAddress ip_slave1(192, 168, 0, 7);
IPAddress ip_slave2(192, 168, 0, 3);

#include "EziServoPlusE.h"
EziServoPlusE myservo;
long timeout = 0;
bool flag = true;
int32_t value1 = 0;
int32_t value2 = 0;
uint8_t com_status = 0;
void setup()
{
  Serial.begin(9600);
  myservo.initServo(ip_slave1, ip_slave2);
  myservo.ServoEnable(ip_slave1, 1, &com_status);
  myservo.ServoEnable(ip_slave2, 1, &com_status);
  myservo.MoveStop(ip_slave1, &com_status);
  myservo.MoveStop(ip_slave2, &com_status);
  Serial.println("done!");
}
void loop()
{
  if (millis() - timeout > 20000)
  {
    myservo.MoveStop(ip_slave1, &com_status);
    myservo.MoveStop(ip_slave2, &com_status);
    timeout = millis();
  }
  else
  {
    myservo.MoveVelocity(ip_slave1, 3000, 1, &com_status);
    myservo.MoveVelocity(ip_slave2, 3000, 1, &com_status);
    myservo.VelocityOverride(ip_slave1, 10000, &com_status);
    myservo.VelocityOverride(ip_slave2, 10000, &com_status);
  }
  /*
  if (flag)
  {
      myservo.MoveSingleAxisAbsPos(ip_slave1, 3000000, 5000);
      myservo.MoveSingleAxisAbsPos(ip_slave2, 3000000, 5000);
      flag = false;
  }
  if(millis() - timeout > 30000)
  {
      myservo.MoveStop(ip_slave1);
      myservo.MoveStop(ip_slave2);
      //Serial6.println("stop");
  }
  */
  Serial.print("value1 :      ");
  myservo.GetActualPos(ip_slave1, &value1, &com_status);
  Serial.println(value1);

  Serial.print("value2:       ");
  myservo.GetActualPos(ip_slave2, &value2, &com_status);
  Serial.println(value2);
  // delay(100);
  /*
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  if (client1.available()) {
    char c = client1.read();
    Serial.print(c);
  }

  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    if (client.connected()) {
      client.write(buff,5);
    }
    if (client1.connected()) {
      client1.write(buff,5);
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    // do nothing:
    while (true) {
      delay(1);
    }
  }
  */
}
