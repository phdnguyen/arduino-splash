int pin_out_solenoid=6;
int pin_out_flash=5;

int log_level=50;  // for future use, could make the program less chatty

// default values
int delay_before_solenoid=1000;
int flash_duration=10;
int delay_after_solenoid=50;

#define INLENGTH 20
char inString[INLENGTH+1];
int inCount;

#define INTERMINATOR 13

void setup()  
{  
  
  Serial.begin(19200); 
  Serial.println("Splash"); 
  Serial.println(); 
   
  pinMode(pin_out_solenoid,OUTPUT);
  pinMode(pin_out_flash,OUTPUT);
  
}  

void waitForEnter() {
  Serial.println("---Press Enter to Continue>");
  // read string until EOF
  inCount=0;
  do {
    while (Serial.available()==0);
    inString[inCount]=Serial.read();
    if (inString[inCount]==INTERMINATOR) 
      break;
  } while (++inCount<INLENGTH);

}


void readInt(char *what, int old_value, int *p_new_value) {
  Serial.print("Enter "); 
  Serial.print(what); 
  Serial.print(" (old value is: "); 
  Serial.print(old_value);  
  Serial.print(") >"); 
  
  // read string until EOF
  inCount=0;
  do {
    while (Serial.available()==0);
    inString[inCount]=Serial.read();
    if (inString[inCount]==INTERMINATOR) 
      break;
  } while (++inCount<INLENGTH);
  // if empty string, return old value
  if (inCount==0) {
    *p_new_value=old_value;
  } else {
    // convert string to number
    *p_new_value=atoi(inString);
  }
  Serial.println();
  
} // readInt

void myDelay(int ms) {
  Serial.print("waiting for ");
  Serial.print(ms);
  Serial.println();
  delay(ms);
} // myDelay
  
void delayAfterFiring(int firing_duration, int delay_duration) {
  int d=delay_duration-firing_duration;
  if (d>0) {
    myDelay(d);
  } else {
    Serial.println("*** couldn't delay after firing, duration larger than delay");
  }
}

void fireRelay(int pin, char *what, int duration) {
  Serial.print("firing ");
  Serial.print(what);
  Serial.println("...");
  digitalWrite(pin,HIGH);
  myDelay(duration);  
  digitalWrite(pin,LOW);  
}
  
void readValues() {
  readInt("delay_before_solenoid",delay_before_solenoid,&delay_before_solenoid);
  readInt("delay_after_solenoid    ",delay_after_solenoid,&delay_after_solenoid);
  readInt("flash_duration     ",flash_duration,&flash_duration);

}
  
void loop()  
{ 
  
  // Action!
  Serial.println("here we go again...");

  // read values  
  readValues();
  
  // wait for Start
  waitForEnter();
  
  // 1. wait until solenoid
  delayAfterFiring(0,delay_before_solenoid);
  
  // 2. fire solenoid
  fireRelay(pin_out_solenoid,"solenoid",delay_after_solenoid);
  
  // 3. wait until flash
  //delayAfterFiring(delay_after_solenoid,delay_after_solenoid);

  // 4. fire flash
  fireRelay(pin_out_flash,"flash",flash_duration);
  if (Serial.available() > 0)
  {
    if (Serial.read() == '@')
    {
      Serial.println("Rebooting. . .");
      delay(100); // Give the computer time to receive the "Rebooting. . ." message, or it won't show up
      void (*reboot)(void) = 0; // Creating a function pointer to address 0 then calling it reboots the board.
      reboot();
    }
  }
  
} 
