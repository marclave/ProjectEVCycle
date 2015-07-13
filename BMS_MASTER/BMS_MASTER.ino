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

#define BLINDSPOT_PWM_PIN_10 // TODO m: something else
#define BLINDSPOT_PWM_PIN_11 // TODO m: something else

/**** Defines for Identifiers and other CAN related items ****/
#define ESS_IDENTIFIER             0x300
#define TRACTIVE_SYSTEM_IDENTIFIER 0x200
#define WARNINGS_IDENTIFIER        0x100


// demo: CAN-BUS Shield, send data
#include <mcp_can.h>
#include <SPI.h>

unsigned char buf[8];

//MCP_CAN CAN(10);                                      // Set CS to pin 10

void fault(char* location, char* message)
{
 // Do something with the location... kind of awkward if its the CAN place :3 
}

void underVoltageMessage(void)
{
    buf[0] = 0x20;
    CAN.sendMsgBuf(WARNINGS_IDENTIFIER, 0, 8, buf);
    delay(100);
    buf[0] = 0x00;
    // TODO m: go to fault state 
}

void overVoltageMessage(void)
{
  buf[0] = 0x04;
  CAN.sendMsgBuf(WARNINGS_IDENTIFIER, 0, 8, buf);
  delay(100);
  buf[0] = 0x00;
  // TODO m: go to fault state
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
    underVoltageMessage();
  }
  else if(cellGroup0 > GROUP_MAX_VOLTAGE)
  {
    overVoltageMessage();
  } 
  
  if(cellGroup1 < GROUP_MIN_VOTLAGE)
  {
     underVoltageMessage();
  } 
  else if(cellGroup1 > GROUP_MAX_VOLTAGE)
  {
     overVoltageMessage();
  } 
  
  if(cellGroup2 < GROUP_MIN_VOTLAGE)
  {
     underVoltageMessage();
  }
  else if(cellGroup2 > GROUP_MAX_VOLTAGE)
  {
     overVoltageMessage();
  } 
  
  if(cellGroup3 < GROUP_MIN_VOTLAGE)
  {
     underVoltageMessage();
  }
  else if(cellGroup3 > GROUP_MAX_VOLTAGE)
  {
     overVoltageMessage();
  } 
  
  if(cellGroup4 < GROUP_MIN_VOTLAGE)
  {
     underVoltageMessage();
  }
  else if(cellGroup4 > GROUP_MAX_VOLTAGE)
  {
     overVoltageMessage();
  } 
  
  // Now send total voltage
  buf[1] = cellGroup0 + cellGroup1 + cellGroup2 + cellGroup3 + cellGroup4;
  CAN.sendMsgBuf(ESS_IDENTIFIER, 0, 8, buf);
  delay(100);
  buf[1] = 0x00;
}

void checkTemperature(void)
{
  uint16_t ambientTemp = analogRead(AMBIENT_TEMP_PIN_13);
  if (ambientTemp < TEMPERATURE_MIN)
  {
    buf[0] = 0x10;
    CAN.sendMsgBuf(WARNINGS_IDENTIFIER, 0, 8, buf);
    delay(100);
    buf[0] = 0x00;
  // TODO m: go to fault state
  }
  else if (ambientTemp > TEMPERATURE_MAX)
  {
    buf[0] = 0x10;
    CAN.sendMsgBuf(WARNINGS_IDENTIFIER, 0, 8, buf);
    delay(100);
    buf[0] = 0x00;
  // TODO m: go to fault state
  }
  else
  {
    // Now send total temp
    buf[0] = ambientTemp;
    CAN.sendMsgBuf(ESS_IDENTIFIER, 0, 8, buf);
    delay(100);
    buf[0] = 0x00;
  }
}

void setup(void)
{
  Serial.begin(9600);

  // Initialize the can bus at 500KHz
  if(CAN_OK == CAN.begin(CAN_500KBPS))
  {
       Serial.print("CAN init OK!!\r\n");
  }
  else 
  {
    Serial.print("Can init fail!!\r\n");
    // TODO m: need to do some CAN handling properly
  }
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
