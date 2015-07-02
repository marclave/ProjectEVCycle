/*
*  Developed by: Marc Laventure
*
*/

/**** Defines for battery ****/
#define CELL_MAX_VOLTAGE 4.2*1023
#define CELL_MIN_VOLTAGE 3.2*1023
#define CELL_GROUP_VALUE 6

// TODO m: need to change the threshold fur shur
#define GROUP_MAX_VOLTAGE CELL_GROUP_VALUE*CELL_MAX_VOLTAGE
#define GROUP_MIN_VOTLAGE CELL_GROUP_VALUE*CELL_MIN_VOLTAGE

#define CELL_GROUP_PIN_0 0
#define CELL_GROUP_PIN_1 1
#define CELL_GROUP_PIN_2 2
#define CELL_GROUP_PIN_3 3
#define CELL_GROUP_PIN_4 4

/**** Defines for temperature ****/
#define TEMPERATURE_MAX 180
#define TEMPERATURE_MIN 30
#define AMBIENT_TEMP_PIN_13 13

/**** Defines for current ****/
#define CURRENT_MAX 200
#define CURRENT_MIN 100

/**** Defines for blindspot ****/
#define BLINDSPOT_MONITOR_PIN_5 5
#define BLINDSPOT_MONITOR_PIN_6 6
#define BLINDSPOT_MONITOR_PIN_7 7
#define BLINDSPOT_MONITOR_PIN_8 8
#define BLINDSPOT_MONITOR_PIN_9 9
#define BLINDSPOT_MONITOR_PIN_10 10

#define BLINDSPOT_PWM_PIN_10
#define BLINDSPOT_PWM_PIN_11

// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>

unsigned char len = 0;
unsigned char buf[8];
unsigned char flagReceive = 0;
uint32_t canID = 0;

void MCP2515_ISR()
{
     flagReceive = 1;
}

void fault(char* location, char* message)
{
 // Do something with the location... kind of awkward if its the CAN place :3 
}

void checkVoltage(void)
{
  uint16_t cellGroup0 = analogRead(CELL_GROUP_PIN_0);
  uint16_t cellGroup1 = analogRead(CELL_GROUP_PIN_1);
  uint16_t cellGroup2 = analogRead(CELL_GROUP_PIN_2);
  uint16_t cellGroup3 = analogRead(CELL_GROUP_PIN_3);
  uint16_t cellGroup4 = analogRead(CELL_GROUP_PIN_4);
  
  if(cellGroup0 < GROUP_MIN_VOTLAGE)
  {
    // Send can message
    // Go to fault state
  }
  else if(cellGroup0 > GROUP_MAX_VOLTAGE)
  {
     // Send can message
     // Go to fault state
  } 
  
  if(cellGroup1 < GROUP_MIN_VOTLAGE)
  {
    // Send can message
    // Go to fault state
  } 
  else if(cellGroup1 > GROUP_MAX_VOLTAGE)
  {
     // Send can message
     // Go to fault state
  } 
  
  if(cellGroup2 < GROUP_MIN_VOTLAGE)
  {
    // Send can message
    // Go to fault state
  }
  else if(cellGroup2 > GROUP_MAX_VOLTAGE)
  {
     // Send can message
     // Go to fault state
  } 
  
  if(cellGroup3 < GROUP_MIN_VOTLAGE)
  {
    // Send can message
    // Go to fault state
  }
  else if(cellGroup3 > GROUP_MAX_VOLTAGE)
  {
     // Send can message
     // Go to fault state
  } 
  
  if(cellGroup4 < GROUP_MIN_VOTLAGE)
  {
    // Send can message
    // Go to fault state
  }
  else if(cellGroup4 > GROUP_MAX_VOLTAGE)
  {
     // Send can message
     // Go to fault state
  } 
  
  // ALL IS GOOD
  // TODO m: need to figure out this bad boy
  unsigned char canMsg[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  //CAN.sendMsgBuf(ID, 0, 8, canMsg);
 // TODO m: also send total voltage rating? 
}

void checkTemperature(void)
{
  uint16_t ambientTemp = analogRead(AMBIENT_TEMP_PIN_13);
  if (ambientTemp < TEMPERATURE_MIN)
  {
    // Send underT can message
  }
  else if (ambientTemp > TEMPERATURE_MAX)
  {
    // Send overT can message
  }
  else
  {
    // ALL IS GOOD  // TODO m: need to figure out this bad boy
     unsigned char canMsg[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
     //CAN.sendMsgBuf(ID, 0, 8, canMsg);
    // TODO m: also send temp 
  }
}

void setup(void)
{
  Serial.begin(9600);

  // Initialize the can bus at 500KHz
  if(CAN.begin(CAN_500KBPS) == CAN_OK)
  {
       Serial.print("CAN init OK!!\r\n");
  }
  else 
  {
    Serial.print("Can init fail!!\r\n");
    // TODO m: need to do some CAN handling properly
  }
  attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
  checkVoltage();
  checkTemperature();
  
  // At this point if errors we would be at fault, so now send OK to pi and wait for recv OK while checking others
  // CAN.sendMsgBuf(0x07B, 0, 8, canMsg);  
  while((flagReceive && 0x01) != 0x01)
  {
    // Wait till recieved flag while checking other things
    checkVoltage();
    checkTemperature();
  }
  
  // Received a message!
  CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
  canID = CAN.getCanId();
} 

void checkPressure(void)
{
  // TODO m: check pressure
}

void checkBlindSpots(void)
{
 // TODO m: implement blind spot code
}
 
void loop(void)
{
  checkVoltage();
  checkTemperature();
  checkBlindSpots();
  checkPressure();
}
