#include <SoftwareSerial.h>
#include "MeOrion.h"

#SIZE_ADDRESS 8
#SIZE_DATA 3

int address_pins[] = {1,2,3,4,5,6};
int io_pins[] = {13,14,15};

void setup() {
  pinMode(8, OUTPUT);
  pinMode(2, OUTPUT);
  Serial.begin(115200);  
}

int address;
int value;
char* token;
void loop() {
  while(Serial.available()) {
         rbyte = Serial.read();
         // commands start with $ and ends with ;
         // a command can be CMD_LENGTH bytes lonc
         if(rbyte == '$'){
            // fill cmd buffer
            i = 0;
            do{
              if (Serial.available()) {
              rbyte = Serial.read();
              if (rbyte == ';') break;
              cmd[i++] = rbyte;
              if (i==CMD_LENGTH) break;
              }
            } while(1);
             if (i == CMD_LENGTH ){
               Serial.println("Error: format error");
               break;
            }

            switch(cmd[0]) {
              case 'r':
                   token = strtok(cmd, ":");
                   token = strtok(NULL, ":");
                   address = atoi(token);
                   value = read_mem(address);
                   Serial.println("READ address: %d => value :%d", address, value);
                   break;
              case 'w':
                   token = strtok(cmd, ":");
                   token = strtok(NULL, ":");
                   address = atoi(token);
                   token = strtok(NULL, ":");
                   value = atoi(token);
                   write_mem(address, value);
                   Serial.println("Write at address: %d  value :%d", address, value);
                 break;
              
              default:
                 Serial.println("Error: unrecognized command");
            }

         }
  }
}

// Allow to read the memory at a desired address
int read_mem(int address){
  int value = 0;
  selectAdress(address);
  digitalWrite(OutputEnable, HIGH);
  value = readOutput();
  return value;
}


// Write the desired address in binary 
// Need to be change if shift register is used
//TODO CAN BE IMPROVED, BETA VeRSI 
void selectAdress(int address){
  int k = 0;
  for (int c = 0; c =< SIZE_ADDRESS ; c++)
  { 
    k = bitRead(address, c);
    if (k)
      digitalWrite(address_pins[c], HIGH);
      printf("1");
    else
      digitalWrite(address_pins[c], LOW);
      printf("0");
  }
}

// Read the output of the memory and convert it into binary
int readOutput(){
  int value = 0;
  int k =0;
  for(int c = ; c=< DATA_SIZE; c++)
  {
    k = digitalRead(io_pins[c]);
    value += pow(2,c)*k;
  }
  return value;
}

int write_mem(int address, int value){
  selectAddress(address);
  writeOutput(value);
  digitalWrite(writeEnable, HIGH);
}

void writeOutput(int value){
  int k = 0;
  for (int c = 0; c =< SIZE_DATA ; c++)
  { 
    k = bitRead(value, c)
    if (k)
      digitalWrite(io_pins[c], HIGH);
      printf("1");
    else
      digitalWrite(io_pins[c], LOW);
      printf("0");
  }

}