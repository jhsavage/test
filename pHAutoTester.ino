// Motor A connections
void loop() __attribute__((optimize("-O0")));
int enA = 3;
int in1 = 28;
int in2 = 29;
// Motor B connections
int enB = 11;
int in3 = 40;
int in4 = 41;


int stir1 = 32;
int stir2 = 33;

int pinLED = 22; //byte pin for color sensor

//#include <ezButton.h>
//ezButton fillWaterBtn(11);
//ezButton fillReagentBtn(10);

boolean didTenDrops = false; //set to false when doing a test

boolean dosedFive = false; //set to false when doing a test

boolean didFiveReagentDrops = false;


#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <ColorName.hpp>

const boolean COMMON_ANODE = false;
const int DELAY_MS = 10;

const int MAX_COLOR_VALUE = 255;
const boolean USE_GAMMA_CORRECTION = true;

Adafruit_TCS34725 _colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


byte _gammaTable[256];


//THE FOLLOWING LIST NEEDS TO BE CHANGED AND CALIBRATED BASED ON THE COLOUR ON THE LIQUID INSIDE THE TEST VIALS

// Measured values. Change these according to the values measured
// and printed out via Serial.print to the screen
const int NUM_COLOR_NAMES = 8;
ColorName _colorNames[NUM_COLOR_NAMES] = {
  /*
  ColorName("Red", 150, 40, 40),
  ColorName("Red", 160, 50, 50),
  ColorName("Red", 255, 0, 0),
  ColorName("Orange", 160, 66, 34),
  ColorName("Yellow", 116, 93, 37),
  ColorName("Green", 0, 255, 0),
  ColorName("Green", 50, 130, 50),
  ColorName("Blue", 0, 0, 255),
  ColorName("Blue", 50, 50, 130),
  ColorName("Cyan", 0, 255, 255),
  ColorName("Magenta", 255, 0, 255),
  ColorName("Black", 122, 82, 72),
  ColorName("Black", 125, 82, 72),
  ColorName("White", 87, 75, 81),
  */
  ColorName("N/A", 40, 40, 40),
  ColorName("N/A", 220, 180, 200),
  ColorName("7.4", 225, 186, 85),
  ColorName("7.8", 119, 91, 73),
  ColorName("8", 104, 75, 65),
  ColorName("8.2", 85, 74, 80),
  ColorName("8.4", 77, 67, 101),
  ColorName("8.8", 55, 70, 110)

  };

//int current pH = 0;

void setup() {
  /*
  if (!_colorSensor.begin()) {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  */

  //fillWaterBtn.setDebounceTime(10); // set debounce time to 50 milliseconds
  //fillReagentBtn.setDebounceTime(10);

  pinMode (pinLED, OUTPUT); //color sensor led pin 
  digitalWrite(pinLED, LOW); //turn color sensor led pin off to stop algae

  Serial.begin(9600);

  // Start TCS34725 color sensor
  
  // Gamma table code from: https://learn.adafruit.com/chameleon-scarf/code
  // Corrects color output based on human perception
  for (int i = 0; i < 256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255;

    _gammaTable[i] = x;
  }

  pinMode(stir1, OUTPUT);
	pinMode(stir2, OUTPUT);
  digitalWrite(stir1, LOW);
	digitalWrite(stir2, LOW);

	// Set all the motor control pins to outputs
	pinMode(enA, OUTPUT);
	pinMode(enB, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

  Serial.println(F("only to be printed once"));
 
  
}

int i = 0; //0
int j = 0;
void loop() {

  
  runTest();
  //fillWaterBtn.loop(); // MUST call the loop() function first
  //fillReagentBtn.loop();
  /*
  if(fillWaterBtn.isPressed()){
    Serial.println("The button is pressed");
    //fillCircuit();
    //runTest();
  }
    
  
  if(fillWaterBtn.isReleased()){
    Serial.println("The button is released");

    i = 0;
    j = 0;
  }
 

  /////////
    
  if(fillReagentBtn.isPressed()){
    Serial.println("The reagent is pressed");
    digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	
  
	analogWrite(enA, 0);
	analogWrite(enB, 200);
	//delay(20);
	//delete after
  delay(200);
  
 
	
	// Now turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  }
  
  if(fillReagentBtn.isReleased()){
    Serial.println("The reagent is released");
  }
  */
}

void magnetStir(){
  digitalWrite(stir1, LOW);
	digitalWrite(stir2, HIGH);
  delay(5000);
  digitalWrite(stir1, LOW);
	digitalWrite(stir2, LOW);

}

void runTest(){
  //empty container first, etc.
 
    for (i; i<10; i++){ //adds 10ml of tank water || change to 42 after tests
      doseFiveMl();
      Serial.println(i);
      delay(500);
    }

  for (j; j<1; j++){
      FiveReagentDropsAtOnce();
       Serial.println(j);
       delay(500);


       magnetStir();
       detectPh(); //detect pH after reagent
    }
  delay(500);
}

void detectPh(){

   //turn on color sensor led to test water.

  float sensedRed, sensedGreen, sensedBlue;

  // Read from color sensor
  //_colorSensor.setInterrupt(false);      // turn on LED
  digitalWrite(pinLED, HIGH); //turn off color sensor led to test water.

  delay(50);                    // takes 50ms to read
  digitalWrite(pinLED, LOW); //turn off color sensor led to test water.

  //_colorSensor.setInterrupt(true);       // turn off LED
  _colorSensor.getRGB(&sensedRed, &sensedGreen, &sensedBlue);

  // Set the RGB LED color
  int rawRed = (int)sensedRed;
  int rawGreen = (int)sensedGreen;
  int rawBlue = (int)sensedBlue;

  // Get the name of closest color and print to serial
  ColorName *closestColorName = ColorName::getClosestColorName(_colorNames, 
    NUM_COLOR_NAMES, rawRed, rawGreen, rawBlue);
  Serial.println("pH is "+ closestColorName->getName()); //This is the only text you want printed.
  
  

  delay(DELAY_MS);

  digitalWrite(pinLED, LOW); //turn off color sensor led to test water.


  
}

void fillCircuit(){
  doseFiveMl();
}

/*

void setRgbLedColor(int red, int green, int blue)
{
  int gammaRed, gammaGreen, gammaBlue;
  
  if (USE_GAMMA_CORRECTION == false) {
    // If a common anode LED, invert values
    if (COMMON_ANODE == true) {
      red = MAX_COLOR_VALUE - red;
      green = MAX_COLOR_VALUE - green;
      blue = MAX_COLOR_VALUE - blue;
    }
   
  } else {

    if (COMMON_ANODE == true) {
      gammaRed = MAX_COLOR_VALUE - _gammaTable[red];
      gammaGreen = MAX_COLOR_VALUE - _gammaTable[green];
      gammaBlue = MAX_COLOR_VALUE - _gammaTable[blue];
    } else {
      gammaRed = _gammaTable[red];
      gammaGreen = _gammaTable[green];
      gammaBlue = _gammaTable[blue];
    }
    
  }
}
*/

// This function lets you control speed of the motors
void doseFiveMl() { //ten calcium reagent drops are needed to be added to 5ml of tank water
	// Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	
  
	analogWrite(enA, 200);
	analogWrite(enB, 0);
	//delay(20);
	//delete after
  delay(150);
	
	// Now turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);

  //dosedFive = true;


}

void singleDrop(){
  digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	
  
	analogWrite(enA, 100);
	analogWrite(enB, 0);
	//delay(20);
	//delete after
  delay(80);
	
	// Now turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

void FiveReagentDropsAtOnce() { //ten calcium reagent drops are needed to be added to 5ml of tank water
	// Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
	
  /*
	analogWrite(enA, 0);
	analogWrite(enB, 200);
	//delay(20);
	//delete after
  delay(200);
  */
  analogWrite(enA, 0);
	analogWrite(enB, 200);
	//delay(20);
	//delete after
  delay(400);
	
	// Now turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);


}