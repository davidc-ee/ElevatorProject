const int stepPin = A0;   // Using Analog PIN A0 as digital pin to pulse for steps
const int dirPin = 13;    // Using Digital PIN D13 to change step direction

#define BUTTON1 8         // First floor call button
#define BUTTON2 7         // Second floor call button
#define BUTTON3 3         // Third floor call button
#define MANUAL_UP 11      // Manually adjust the elevator position UP
#define LIMIT_SWITCH 2    // Limit switch to stop elevator

#define SPEED 9000        // This is how fast the elevator car will move

#define FIRST_FLOOR 35    // This is how many steps above 0 the first floor starts
#define FLOOR_STEPS 300   // This is how many motor steps there are between floors

int floorPosition = 1;    // Initialize elevator to the first floor

// Initialize all 3 call buttons to LOW
int floorOneButton   = LOW;
int floorTwoButton   = LOW;
int floorThreeButton = LOW;

// Initialize manual adjustment button to LOW
int manualUp         = LOW;

// Initialize limit switch to HIGH because the logic is in reverse
int limitSwitch      = HIGH;


void setup(){

  // A4988 pins will be outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Elevator call buttons will be inputs
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(MANUAL_UP, INPUT);
  pinMode(LIMIT_SWITCH, INPUT);

  //attachInterrupt(0, stopMotor, LOW);

  // Initialize elevator position
  initElevator();
  
}

void initElevator() {

  // Run the elevator car in reverse until the limit switch is pressed.
  // Once the limit switch is pressed, we know where the elevator car
  // is at.
  stepperREV();
  do {
      motorStep();
      delayMicroseconds(SPEED);
      limitSwitch = digitalRead(LIMIT_SWITCH);
  } while (limitSwitch);

  // Short delay
  delay(2000);

  // Now that the limit switch has been reached, run the elevator car
  // forwards until it is even with floor 1.
  stepperFWD();
  for(int i = 0; i < FIRST_FLOOR; i++){
      motorStep();
      delayMicroseconds(SPEED);
  }

  // Initialization complete. Elevator is now sitting at floor 1
  // ready to service the call requests.
}

void loop(){

  // Read the elevator call buttons
  floorOneButton   = digitalRead(BUTTON1);
  floorTwoButton   = digitalRead(BUTTON2);
  floorThreeButton = digitalRead(BUTTON3);
  manualUp         = digitalRead(MANUAL_UP);

  // This if statement handles the case when the first floor call button is pressed
  if (floorOneButton) {
      
      // If the elevator is currently at floor 2, run the stepper motor in
      // reverse to bring it back down to floor 1.
      if (floorPosition == 2) {
          stepperREV();
          for(int i = 0; i < FLOOR_STEPS; i++){
            motorStep();
            delayMicroseconds(SPEED);
          }

      // If the elevator is currently at floor 3, run the stepper motor in
      // reverse to bring it back down to floor 1.
      } else if (floorPosition == 3) {
          stepperREV();
          for(int i = 0; i < FLOOR_STEPS * 2; i++){
            motorStep();
            delayMicroseconds(SPEED);
          }
      }

      // Update position variable      
      floorPosition = 1;

  
  // This if statement handles the case when the second floor call button is pressed  
  } else if (floorTwoButton) {

      // If the elevator is currently at floor 1, run the stepper motor in
      // the forward direction to bring it up to floor 2.
      if (floorPosition == 1) {
          stepperFWD();
          for(int i = 0; i < FLOOR_STEPS; i++){
            motorStep();
            delayMicroseconds(SPEED);
          }

      // If the elevator is currently at floor 3, run the stepper motor in
      // reverse to bring it back down to floor 2.      
      } else if (floorPosition == 3) {
          stepperREV();
          for(int i = 0; i < FLOOR_STEPS; i++){
            motorStep();
            delayMicroseconds(SPEED);
          }
      }

      // Update position variable      
      floorPosition = 2;

  
  // This if statement handles the case when the third floor call button is pressed
  } else if (floorThreeButton) {

      // If the elevator is currently at floor 1, run the stepper motor in
      // the forward direction to bring it up to floor 3.      
      if (floorPosition == 1) {
          stepperFWD();
          for(int i = 0; i < FLOOR_STEPS * 2; i++){
            motorStep();
            delayMicroseconds(SPEED);
          }

      // If the elevator is currently at floor 2, run the stepper motor in
      // the forward direction to bring it up to floor 3.
      } else if (floorPosition == 2) {
          stepperFWD();
          for(int i = 0; i < FLOOR_STEPS; i++){
            motorStep();
            delayMicroseconds(SPEED);
          }
      }

      // Update position variable
      floorPosition = 3; 
  
  
  // This if statement handles the case when we need to manually adjust the
  // elevator position UP.
  } else if (manualUp) {
       stepperFWD();
       for(int i = 0; i < 10; i++){
           motorStep();
           delayMicroseconds(SPEED);
       }    
  }
}


//change the stepper direction to forward
void stepperFWD(){
  digitalWrite(dirPin, HIGH);
}

//change the stepper direction to reverse
void stepperREV(){
  digitalWrite(dirPin, LOW);

}

//have the stepper motor take one step
void motorStep(){
  digitalWrite(stepPin, HIGH);
  delay(1);
  digitalWrite(stepPin, LOW);
}
