// Traffic Light Control - with pedLight and ambulance buttons

#define westButton 3       // Button to trigger west traffic flow
#define eastButton 13      // Button to trigger east traffic avere
#define westRed 2          // West red light
#define westYellow 1       // West yellow light
#define westGreen 0        // West green light
#define eastRed 12         // East red light
#define eastYellow 11      // East yellow light
#define eastGreen 10       // East green light
#define pedLight 8         // Pedestrian crossing light
#define pedSequence 9      // Button to trigger pedestrian sequence
#define westAmbulance 4    // West ambulance button
#define eastAmbulance 5    // East ambulance button

#define yellowBlinkTime 500  // Time for yellow light to blink 
#define ambulanceDelay 500   // Small delay for ambulance sequence


boolean trafficWest = true; // True = west green, false = east green
int flowTime = 1000;       // Delay before switching lights
int changeDelay = 2000;    // Delay for yellow light during transition

// Function to switch from green to red via yellow
void greenRed(int green, int yellow, int red) {
  digitalWrite(green, LOW);   // Turn off green
  digitalWrite(yellow, HIGH); // Turn on yellow
  delay(changeDelay);         // Wait for transition
  digitalWrite(yellow, LOW);  // Turn off yellow
  digitalWrite(red, HIGH);    // Turn on red
}

// Function to blink yellow light before it switches to green
void yellowBlink(int yellow) {
  for (int a = 0; a < 5; a++) { // Blink 5 times
    digitalWrite(yellow, HIGH);
    delay(yellowBlinkTime);
    digitalWrite(yellow, LOW);
    delay(yellowBlinkTime);
  }
}

// Function to switch from red to green 
void redGreen(int red, int yellow, int green) {
  digitalWrite(red, LOW);     // Turn off red
  digitalWrite(yellow, HIGH); // Turn on yellow
  delay(changeDelay);         // Wait for transition
  digitalWrite(yellow, LOW);  // Turn off yellow
  digitalWrite(green, HIGH);  // Turn on green
}

// Function for west ambulance sequence
void westAmbulanceSequence() {
  // If east is green, transition east to red
  if (!trafficWest) {
    digitalWrite(eastGreen, LOW);    // Turn off east green
    digitalWrite(eastYellow, HIGH);  // East yellow on
    delay(ambulanceDelay);           // Small delay
    digitalWrite(eastYellow, LOW);   // East yellow off
    digitalWrite(eastRed, HIGH);     // East red on
    // Set west to green
    digitalWrite(westRed, LOW);      // Turn off west red
    digitalWrite(westGreen, HIGH);   // West green on
    trafficWest = true;              // Update traffic state
  }
}

// Function for east ambulance sequence
void eastAmbulanceSequence() {
  // If west is green, transition west to red
  if (trafficWest) {
    digitalWrite(westGreen, LOW);    // Turn off west green
    digitalWrite(westYellow, HIGH);  // West yellow on
    delay(ambulanceDelay);           // Small delay
    digitalWrite(westYellow, LOW);   // West yellow off
    digitalWrite(westRed, HIGH);     // West red on
    // Set east to green
    digitalWrite(eastRed, LOW);      // Turn off east red
    digitalWrite(eastGreen, HIGH);   // East green on
    trafficWest = false;             // Update traffic state
  }
}

// Function for ped crossing sequence
void pedButton() {
  // Stop current traffic flow
  if (trafficWest) {
    greenRed(westGreen, westYellow, westRed); // West goes to red
    digitalWrite(eastRed, HIGH);              // Ensure east stays red
  } else {
    greenRed(eastGreen, eastYellow, eastRed); // East goes to red
    digitalWrite(westRed, HIGH);              // Ensure west stays red
  }
  delay(2000);                  
  digitalWrite(pedLight, HIGH); // Turn on ped light
  delay(7000);                  
  digitalWrite(pedLight, LOW);  // Turn off ped light
  delay(2000);                  // Short pause after crossing
  // Resume traffic flow from before
  if (trafficWest) {
    redGreen(westRed, westYellow, westGreen); // West goes back to green
  } else {
    redGreen(eastRed, eastYellow, eastGreen); // East goes back to green
  }
}

void setup() {
  // Set buttons as inputs
  pinMode(westButton, INPUT);
  pinMode(eastButton, INPUT);
  pinMode(pedSequence, INPUT);
  pinMode(westAmbulance, INPUT);
  pinMode(eastAmbulance, INPUT);
  // Set lights as outputs
  pinMode(westRed, OUTPUT);
  pinMode(westYellow, OUTPUT);
  pinMode(westGreen, OUTPUT);
  pinMode(eastRed, OUTPUT);
  pinMode(eastYellow, OUTPUT);
  pinMode(eastGreen, OUTPUT);
  pinMode(pedLight, OUTPUT);

  // Sets initial state: west green, east red
  digitalWrite(westRed, LOW);
  digitalWrite(westYellow, LOW);
  digitalWrite(westGreen, HIGH);
  digitalWrite(eastRed, HIGH);
  digitalWrite(eastYellow, LOW);
  digitalWrite(eastGreen, LOW);
}

void loop() {
  // If west ambulance button is pressed
  if (digitalRead(westAmbulance) == HIGH) {
    westAmbulanceSequence();
  }
  // If east ambulance button is pressed
  if (digitalRead(eastAmbulance) == HIGH) {
    eastAmbulanceSequence();
  }
  // If west button is pressed and east is green
  if (digitalRead(westButton) == HIGH) {
    if (!trafficWest) {
      trafficWest = true;                       // Switch to west flow
      delay(flowTime);                          // Wait before changing lights
      greenRed(eastGreen, eastYellow, eastRed); // East goes to red
      yellowBlink(westYellow);                  // West yellow blinks
      redGreen(westRed, westYellow, westGreen); // West goes to green
    }
  }
  // If east button is pressed and west is green
  if (digitalRead(eastButton) == HIGH) {
    if (trafficWest) {
      trafficWest = false;                      // Switch to east flow
      delay(flowTime);                          // Wait before changing lights
      greenRed(westGreen, westYellow, westRed); // West goes to red
      yellowBlink(eastYellow);                  // East yellow blinks
      redGreen(eastRed, eastYellow, eastGreen); // East goes to green
    }
  }
  // If pedestrian button is pressed
  if (digitalRead(pedSequence) == HIGH) {
    pedButton(); // Run ped sequence
  }
}