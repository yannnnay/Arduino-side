/* Program conceived for an arduino uno and an AT28C64B-15PU directly connected, not passing through 8 bits registers */
/* Author: Yann Ferry */
/* 10/05/2020 */

#include <SoftwareSerial.h>
#define SIZE_ADDRESS 4
#define SIZE_DATA 3

int address_pins[] = {16,17,18,19};
int io_pins[] = {8,9,10};
int outputEnable = 4;
int writeEnable = 3;
int chipEnable = 5;

static const int CMD_LENGTH = 32;
char cmd[CMD_LENGTH];

void selectAddress(int address);
void writeOutput(int value);
int write_mem(int address, int value);
int readOutput();
int read_mem(int address);
int power(int a, int b);

void setup() {
  for(int j = 0; j< SIZE_ADDRESS; j++){
    pinMode(address_pins[j],OUTPUT);
  }
  pinMode(outputEnable, OUTPUT);
  pinMode(writeEnable, OUTPUT);
  digitalWrite(outputEnable, HIGH);
  digitalWrite(writeEnable, HIGH);
  digitalWrite(chipEnable, HIGH);
  Serial.begin(115200);
}

int address;
int value;
char* token;
byte rbyte;
int i;
void loop() {
  while(Serial.available()) {
         rbyte = Serial.read();
         // commands start with $ and ends with ;
         // a command can be CMD_LENGTH bytes lonc
         if(rbyte == '$'){
            // fill cmd buffer
            memset(cmd,0,CMD_LENGTH);
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
                   address = 0;
                   value = 0;
                   token = strtok(cmd, ":");
                   token = strtok(NULL, ":");
                   address = atoi(token);
                   value = read_mem(address);
                   Serial.print("READ address: ");
                   Serial.print(address);
                   Serial.print(" => value : ");
                   Serial.println(value);
                   digitalWrite(outputEnable, HIGH);
                   digitalWrite(chipEnable, HIGH);
                   break;
              case 'w':
                   address = 0;
                   value = 0;
                   token = strtok(cmd, ":");
                   token = strtok(NULL, ":");
                   address = atoi(token);
                   token = strtok(NULL, ":");
                   value = atoi(token);
                   write_mem(address, value);
                   Serial.print("Write at address: ");
                   Serial.print(address);
                   Serial.print(" value : ");
                   Serial.println(value);
                   digitalWrite(writeEnable, HIGH);
                   digitalWrite(chipEnable, HIGH); 
                   break;
              default:
                 Serial.println("Error: unrecognized command");
            }

         }
  }
}

// Allow to read the memory at a desired address
int read_mem(int address){
  int value = 99;
  selectAddress(address);
  value = readOutput();
  return value;
}


// Write the desired address in binary 
// Need to be change if shift register is used
//TODO CAN BE IMPROVED, BETA VERSION 
void selectAddress(int address){
  int k = 0;
  for (int c = 0; c < SIZE_ADDRESS ; c++)
  { 
    k = bitRead(address, c);
    if (k==1){
      digitalWrite(address_pins[c], HIGH);
    } else {
      digitalWrite(address_pins[c], LOW);
    }
  }
}

// Read the output of the memory and convert it into binary
int readOutput(){
  int value = 0;
  int k =0;
  
  for(int j = 0; j< SIZE_DATA; j++){
    pinMode(io_pins[j],INPUT);
  }
  
  digitalWrite(outputEnable, LOW);
  digitalWrite(writeEnable, HIGH);
  digitalWrite(chipEnable, LOW);
  
  for(int c = 0; c< SIZE_DATA; c++)
  {

    k = digitalRead(io_pins[c]);
    if(k == HIGH){
      value = value + power(2,c);
    }
  }
  delay(1000);
  return value;
}

int write_mem(int address, int value){
  selectAddress(address);
  writeOutput(value);
  
}

void writeOutput(int value){
  int k = 0;
  for(int j = 0; j< SIZE_DATA; j++){
    pinMode(io_pins[j],OUTPUT);
  }

  
  for (int c = 0; c < SIZE_DATA ; c++)
  { 
    k = bitRead(value, c);
    if (k == 1){
      digitalWrite(io_pins[c], HIGH);
      } else {
      digitalWrite(io_pins[c], LOW);
    }
  }
  delay(1000);
  digitalWrite(writeEnable, LOW);
  digitalWrite(chipEnable, LOW);
  digitalWrite(writeEnable,HIGH);
  delay(5);
  
}

int power(int a, int b){
  int res =1;
  for(int q =0; q<b;q++)
  {
    res = res*a;
  }
  return res;
}
