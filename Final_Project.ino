// Jacob Decker - Computer Engineering
// Nick Allen - Mechanical Engineering
// Miami University

// Part of this code is used for cycling through 6 LEDs connected to the outputs
// of a SN74HC595N Shift Register by pushing a button.
// Code was adapted and used from Arduino's StateChangeDetection code and ShftOut12 code
// www.arduino.cc/en/Tutorial/StateChangeDetection
// www.arduino.cc/en/Tutorial/ShftOut12

// Register digital pins
int latchPin = 5;
int clockPin = 6;
int dataPin = 4;

// Button digital pin
int buttonPin = 10;

// Motor pins
int rev = 8;
int fwd = 9;

// Variables for counting state changes in the button
int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

// Variables for guitar frequency
unsigned long energy;
unsigned long voltread = 0;
unsigned long count = 0;

float gFrequency = 0;


void setup() {
  // Initializing the pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(rev, OUTPUT);
  pinMode(fwd, OUTPUT);

  Serial.begin(9600);
  Serial.println("reset");
}

void loop() {

  buttonState = digitalRead(buttonPin); // Read in current state

  if (buttonState != lastButtonState) { // If the state has changed
    if (buttonState == HIGH) {
      buttonPushCounter++;
      if (buttonPushCounter > 6) { // There's only 6 LEDs so if it reaches 7, go back to 1
        buttonPushCounter = 1;
      }

      Serial.print("Button counter: ");
      Serial.println(buttonPushCounter); // Prints the current counter

      // Send the counter number to the Write method
      int bitToSet = buttonPushCounter;
      registerWrite(bitToSet, HIGH);
    }

    if (buttonPushCounter == 6) { // At LED 6

      int bitToSet = buttonPushCounter;
      registerWrite(bitToSet, HIGH);
      
      gFrequency = 0;
      do { // Check the frequency 
        gFrequency = guitarFrequency();
      } while (gFrequency > 100 || gFrequency < 30);


      Serial.print("gFrequency: ");
      Serial.println(gFrequency);

      // Find whether the frequency is too high or too low
      // Will continously check and adjust until it's 81-83
      while (gFrequency < 81) {
        digitalWrite(fwd, HIGH);
        delay(250);
        digitalWrite(fwd, LOW);

        do {
          gFrequency = guitarFrequency();
        } while (gFrequency > 100 || gFrequency < 30);

      }

      while (gFrequency > 83) {
        digitalWrite(rev, HIGH);
        delay(250);
        digitalWrite(rev, LOW);

        do {
          gFrequency = guitarFrequency();
        } while (gFrequency > 100 || gFrequency < 30);

      }

      // Go through a second time

      while (gFrequency < 81) {
        digitalWrite(fwd, HIGH);
        delay(125);
        digitalWrite(fwd, LOW);

        do {
          gFrequency = guitarFrequency();
        } while (gFrequency > 100 || gFrequency < 30);

      }

      while (gFrequency > 83) {
        digitalWrite(rev, HIGH);
        delay(125);
        digitalWrite(rev, LOW);

        do {
          gFrequency = guitarFrequency();
        } while (gFrequency > 100 || gFrequency < 30);

      }

      Serial.println("DONE!");
      buttonPushCounter++;

      delay(50); // Short delay
    }

    if (buttonPushCounter == 5) { // LED 5
      
      gFrequency = 0;
      do {
        gFrequency = guitarFrequency();
      } while (gFrequency > 150 || gFrequency < 30);


      Serial.print("gFrequency: ");
      Serial.println(gFrequency);

      while (gFrequency < 109) {
        digitalWrite(fwd, HIGH);
        delay(250);
        digitalWrite(fwd, LOW);

        do {
          gFrequency = guitarFrequency();
        } while (gFrequency > 150 || gFrequency < 30);

      }

      while (gFrequency > 111) {
        digitalWrite(rev, HIGH);
        delay(250);
        digitalWrite(rev, LOW);

        do {
          gFrequency = guitarFrequency();
        } while (gFrequency > 150 || gFrequency < 30);

      }

      // Go through a second time

      while (gFrequency < 109) {
        digitalWrite(fwd, HIGH);
        delay(125);
        digitalWrite(fwd, LOW);

        do {
          gFrequency = guitarFrequency();
        } while (gFrequency > 150 || gFrequency < 30);

      }

      while (gFrequency > 111) {
        digitalWrite(rev, HIGH);
        delay(125);
        digitalWrite(rev, LOW);

        do {
          gFrequency = guitarFrequency();
        } while (gFrequency > 150 || gFrequency < 30);

      }

      Serial.println("DONE! NEXT STRING");
      buttonPushCounter++;
      
      delay(3000); // Short delay
    }
    
    // Update the button state
    lastButtonState = buttonState;

  }
}

// ================ Register Write Function ==============

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

// ================ Frequency Function =================

float guitarFrequency() {
  // ---------- Samples ---------
  // Start the timer
  unsigned long t1 = millis();

  energy = 0;

  // Takes 4500 samples of the wave
  for (int i = 0; i < 4500; i++) {
    voltread = analogRead(A1);

    energy = energy + sq(voltread - 460);
  }

  // End the timer
  unsigned long t2 = millis();

  //  Serial.print("Voltage is: ");
  //  Serial.println(voltread);
  // Serial.print("Energy: ");
  // Serial.println(energy);
  //  Serial.print("Time taken (in milliseconds): ");
  //  Serial.println(t2 - t1);

  // --------------------------

  // ---------- Finding Frequency ---------
  unsigned long t3;
  unsigned long t4;
  if (energy > 25000000) {

    Serial.println("Getting frequency");

    if (analogRead(A1) > 480) {
      delay(1000);
      count = 0;
      while (count < 20) {
        // While voltage is above zero crossing
        while (analogRead(A1) - 6 > 460); // Wait

        // Records time right after it crosses
        t3 = micros();

        delayMicroseconds(1000);

        // Wait for voltage to cross back
        while (analogRead(A1) - 6 < 460); // Wait

        count++;

        delayMicroseconds(1000);

        // Wait for another cross, completing the period
        while (analogRead(A1) - 6 > 460);

        count++;
      }

      // Records the time after the 20 crosses
      t4 = micros();

    }

    float timeSec = t4 - t3;

    float timeSec2 = timeSec / 100000;

    // Calculate the period
    float period = timeSec2 / 10;

    // Calculate the frequency
    float frequency = 1 / period;

    Serial.print("Frequency: ");
    Serial.println(frequency);
    Serial.println("=============");

    // delay(600);

    return frequency;
  }

  // Reset count for next loop through
  count = 0;

  return 0;

  // -----------------------------------
}

