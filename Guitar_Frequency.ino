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

// ---------- Finding Frequency --------- // Continously check for voltage!!!

  if (analogRead(A1) > 600) {
    
  while (count < 20) {
    // While voltage is above zero crossing
    while (analogRead(A1) > 512); // Wait

    // Start timer
    t3 = micros();
    
    delayMicroseconds(625);

    // Wait for voltage to cross back
    while (analogRead(A1) < 512); // Wait

    count++;

    delayMicroseconds(625);

    while (analogRead(A1) > 512);

    count++;
  }
  
    t4 = micros();
  
  }

  period = ((t4 - t3) / 10) * 1000000;

  frequency = (1/period);

  Serial.print("The frequency is: ");
  Serial.println(frequency);
  
// -----------------------------------
}

