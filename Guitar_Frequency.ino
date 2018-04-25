int energy = 0;
int voltread = 0;
int count = 0;
int period = 0;
int frequency = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

// ---------- Samples ---------
  // Start the timer
  unsigned long t1 = millis();

  for (int i = 0; i < 20; i++) {
    voltread = analogRead(A0);
    energy = energy + sq(voltread-512);
  }

  // End the timer
  unsigned long t2 = millis();
  
  Serial.print("Energy: ");
  Serial.println(energy);
  Serial.print("Time taken (in milliseconds): ");
  Serial.println(t2 - t1);

// --------------------------

// ---------- Finding Frequency ---------

  if (voltread > 600) {
    
  while (count < 20) {
    // While voltage is above zero crossing
    while (voltread > 512); // Wait

    // Start timer
    unsigned long t3 = micros();
    
    delayMicroseconds(625);

    // Wait for voltage to cross back
    while (voltread < 512); // Wait

    count++;

    delayMicroseconds(625);

    while (voltread > 512);

    count++;
  }
  
  unsigned long t4 = micros();
  
  }

  period = ((t4 - t3) / 10) * 1000000;

  frequency = 1 / period;

  Serial.print("The frequency is: ");
  Serial.println(frequency);
  
// -----------------------------------
}

