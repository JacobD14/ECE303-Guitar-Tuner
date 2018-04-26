// Jacob Decker and Nick Allen
// Miami University 

// This code is used to find the frequency of the particular
// guitar string plucked. It first takes samples to find the energy, 
// then analyzes the voltage wave to find the period, and then the frequency.

unsigned long energy;
int voltread = 0;
int count = 0;
int period = 0;
int frequency = 0;
unsigned long t3;
unsigned long t4;

void setup() {
  Serial.begin(9600);
}

void loop() {

// ---------- Samples ---------
  // Start the timer
  unsigned long t1 = millis();

  energy = 0;

  // Takes 4500 samples of the wave
  for (int i = 0; i < 4500; i++) {
    voltread = analogRead(A1);

    energy = energy + sq(voltread-512);
  }

  // End the timer
  unsigned long t2 = millis();

  Serial.print("Voltage is: ");
  Serial.println(voltread);
  Serial.print("Energy: ");
  Serial.println(energy);
  Serial.print("Time taken (in milliseconds): ");
  Serial.println(t2 - t1);

// --------------------------

// ---------- Finding Frequency --------- 

  if (analogRead(A1) > 600) {
    
  while (count < 20) {
    // While voltage is above zero crossing
    while (analogRead(A1) > 512); // Wait

    // Records time right after it crosses
    t3 = micros();
    
    delayMicroseconds(625);

    // Wait for voltage to cross back
    while (analogRead(A1) < 512); // Wait

    count++;

    delayMicroseconds(625);

    // Wait for another cross, completing the period
    while (analogRead(A1) > 512);

    count++;
  }

    // Records the time after the 20 crosses
    t4 = micros();
  
  }

  // Calculate the period 
  period = ((t4 - t3) / 10) * 1000000;

  frequency = (1/period);

  Serial.print("The frequency is: ");
  Serial.println(frequency);
  
// -----------------------------------
}

