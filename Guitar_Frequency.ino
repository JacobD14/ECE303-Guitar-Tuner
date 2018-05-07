// Jacob Decker and Nick Allen
// Miami University

// This code is used to find the frequency of the particular
// guitar string plucked. It first takes samples to find the energy,
// then analyzes the voltage wave to find the period, and then the frequency.

unsigned long energy;
unsigned long voltread = 0;
unsigned long count = 0;


void setup() {
  Serial.begin(9600);
}

void loop() {

  // First find the total energy of the plucked string

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
  //  Serial.print("Energy: ");
  // Serial.println(energy);
  //  Serial.print("Time taken (in milliseconds): ");
  //  Serial.println(t2 - t1);

  // --------------------------


  // ---------- Finding Frequency ---------
  // This part only starts if the energy is above 25,000,000 and if
  // the voltage being read in is greater than 480.
  // This counts the amount of times the wave crosses the zero line.
  // Once it crosses 20 times, it breaks out of the loop.
  // Using the time it took to go through that, we can calculate period,
  // and then the frequency.

  unsigned long t3;
  unsigned long t4;

  if (energy > 25000000) {

    Serial.println("Getting Frequency");

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
  }

  // Reset count for next loop through
  count = 0;

  // Time taken in microseconds
  float timeSec = t4 - t3;

  // Converting from microseconds to seconds
  float timeSec2 = timeSec / 100000;

  // Calculate the period
  float period = timeSec2 / 10;

  // Calculate the frequency
  float frequency = 1 / period;

  Serial.print("The frequency is: ");
  Serial.println(frequency);
  Serial.println("=============");

  delay(600);

  // -----------------------------------

}



