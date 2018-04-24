// Jacob Decker
// Computer Engineering
// Miami University
// This code is used for cycling through 6 LEDs connected to the outputs 
// of a SN74HC595N Shift Register by pushing a button.
// Code was adapted and used from Arduino's StateChangeDetection code, and ShftOut12 code
// www.arduino.cc/en/Tutorial/StateChangeDetection
// www.arduino.cc/en/Tutorial/ShftOut12 

// Register digital pins
int latchPin = 5;
int clockPin = 6;
int dataPin = 4;

// Button digital pin
int buttonPin = 10;

// Variables for counting state changes in the button
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

void setup() {
  // Initializing the pins 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
  Serial.println("reset");
}

void loop() {
  // This code uses the Serial Monitor for input: entering 1 - 6
//if (Serial.available() > 0) {
//  // Read in the number
//    int bitToSet = Serial.read() - 48; 
//  Serial.println(bitToSet);
//  // write to the shift register with the correct bit set high:
//    registerWrite(bitToSet, HIGH);
//  }

// This code uses the button for input
buttonState = digitalRead(buttonPin); // Read in current state

if(buttonState != lastButtonState) { // If the state has changed
  if(buttonState == HIGH) { 
    buttonPushCounter++;
    if(buttonPushCounter == 7) { // There's only 6 LEDs so if it reaches 7, go back to 1
      buttonPushCounter = 1;
    }
    
    Serial.println(buttonPushCounter); // Prints the current counter

    // Send the counter number to the Write method
    int bitToSet = buttonPushCounter;
    registerWrite(bitToSet, HIGH);
  }
  delay(50); // Short delay 
}
// Update the button state
lastButtonState = buttonState;

}
// This method shifts the bits to the regsister.
  void registerWrite(int whichPin, int whichState) {
  byte bitsToSend = 0;

  // turn off the output
  digitalWrite(latchPin, LOW);

  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output
  digitalWrite(latchPin, HIGH);

}

