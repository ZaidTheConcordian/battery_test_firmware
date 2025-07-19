
#define CMD_STRT 0xB3

#define CMD_PING 0x00
#define CMD_ID 0x01
#define CMD_DATA 0x02
#define CMD_STND 0x04
#define CMD_DISC 0x05
#define CMD_CHAR 0x06
#define CMD_COMP 0x07

#define RX_CMD_LENGTH 3
#define TX_CMD_MAX_LENGTH 12

uint8_t rxBuffer[RX_CMD_LENGTH];

enum BatteryBenchState{
  Standby,
  Charge,
  Discharge
} bench_state;

enum CompletionStatus{
  Success,
  Fail,
  InProgress
} completion_status;

enum Command{
  Ping,
  ID,
  RequestData,
  SetStandBy,
  SetDischarge,
  SetCharge,
  RequestCompletion,
  BadCommand
};


void setup() {

  Serial.begin(19200);
  bench_state = BatteryBenchState::Standby;
}

void loop() {

  //has to be changed *********************************
  while(Serial.available() != 3);

  Command Incomming = ReadCommand();

  switch (Incomming){

      case Command::Ping:
        if (rxBuffer[2] = 0xFF){
          SetBenchId(); 
        }else {
          MirrorPing(): 
        }
       
        //SendBenchStatus();
        break;
         

      case Command::RequestData:
        SendData();
        break;

      case Command::SetStandBy:
        bench_state = BatteryBenchState::Standby;
        SendBenchStatus();
        break;

      case Command::SetDischarge:
        bench_state = BatteryBenchState::Discharge;
        SendBenchStatus();
        break;

      case Command::SetCharge:
        bench_state = BatteryBenchState::Charge;
        SendBenchStatus();
        break;

      case Command::RequestCompletion:
        SendCompletionStatus();
        break;

      case Command::BadCommand:
        SendBadRequest();
        break;

  }


}

Command ReadCommand(){

  int byte_num = 0;

  rxBuffer[byte_num++] = Serial.read();
  rxBuffer[byte_num++] = Serial.read();
  rxBuffer[byte_num++] = Serial.read();


  //Check DILIMITER
  if(rxBuffer[0] != CMD_STRT)
    return Command::BadCommand; //bad DILIMITER



  /*
  has to be changed ***********************************
  //Check CheckSum
  uint8_t checksum = CMD_STRT;
  for (int i = 1; i < RX_CMD_LENGTH; i++)
    checksum ^= rxBuffer[i];

  if(checksum != rxBuffer[RX_CMD_LENGTH - 1])
    return Command::BadCommand; //bad Checksum
  */


  if(rxBuffer[1] == CMD_PING)
    return Command::Ping;
    
  if(rxBuffer[1] == CMD_DATA)
    return Command::RequestData;
  
  if(rxBuffer[1] == CMD_STND)
    return Command::SetStandBy;

  if(rxBuffer[1] == CMD_DISC)
    return Command::SetDischarge;

  if(rxBuffer[1] == CMD_CHAR)
    return Command::SetCharge;

  if(rxBuffer[1] == CMD_COMP)
    return Command::RequestCompletion;

  return Command::BadCommand; //invalid Command id
}

Void MirrorPing(){
  //Respond to the Bench, confirming the ping
  IDeas = rxBuffer[2];
  byte buf[4] = {CMD_STRT , CMD_PING , IDeas,  CMD_STRT ^ CMD_ID ^ IDeas};
  Serial.write(buf, 4);
  //Need to update the GUI
  
}
Void SetBenchId(){
  uint8_t IDeas = 1; //All benches will have id = 1 until communication with gui is figured out
  //Set the id
  byte buf[4] = {CMD_STRT , CMD_ID , IDeas,  CMD_STRT ^ CMD_ID ^ IDeas};
  Serial.write(buf,4);
  //Need to upated the gui
  
}

void SendBenchStatus(){
  
  byte buf[3] = { CMD_STRT , bench_state  , CMD_STRT ^ bench_state };

  Serial.write(buf, 3);

}
void SendCompletionStatus(){

  byte buf[4] = { CMD_STRT , bench_state , completion_status , CMD_STRT ^ bench_state ^ completion_status };

    Serial.write(buf, 4);

}

void SendData(){

  //has to be changed **************************************************
  Serial.write(-1);
  Serial.write(-2);
  Serial.write(-3);
}

void SendBadRequest(){

  //has to be changed **************************************************
  Serial.write(-10);
  Serial.write(-20);
  Serial.write(-30);
}
