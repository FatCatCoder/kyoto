#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

// old calulation
//  intensity = (map(val0, 0, 255, 0, 5000)) / 1000.0;

const int solenoidPin = 4;  // solenoid control signal

unsigned long refreshInterval = 50;  // the last time the output pin was toggled
unsigned long lastRefreshTime = 0;    // the debounce time; increase if the output flickers

unsigned long lastDripTime = 0;    // the debounce time; increase if the output flickers

float openTime = 45;    // 45 ms, enough for a drop to form
float delayTime = 5000; // 5 seconds untill next drop

float dripRate;

int slideVal;
int prevSlideVal;

int arrayPlace = 0;
float timings[] = { 0.0, 0.2, 0.4, 0.6,
    0.8, 1.0, 1.2, 1.4,
    1.6, 1.8, 2.0, 2.2,
    2.4, 2.6, 2.8, 3.0,
    3.2, 3.4, 3.6, 3.8,
    4.0, 4.2, 4.4, 4.6,
    4.8, 5.0, 5.2, 5.4,
    5.6, 5.8, 6.0 };

void setup() {
  Serial.begin(9600);

  prevSlideVal = analogRead(A0) >> 2;
  
  lcd.init();
  lcd.clear();         
  lcd.backlight();

  lcd.setCursor(2,0);   // Set cursor to character 2 on line 0
  lcd.print("Robo Kytoto!");
  
  pinMode(solenoidPin, OUTPUT); // setup solenoid
}

void pot(){
  if(millis() - lastRefreshTime >= refreshInterval){
    lastRefreshTime += refreshInterval;
    slideVal = analogRead(A0) >> 2;
    
    if (slideVal != prevSlideVal) {
      prevSlideVal =  analogRead(A0) >> 2;
  
      int arrayPlace = (map(slideVal, 0, 255, 0, 31));
      dripRate = int(timings[arrayPlace] * 1000); // set new drip speed (i.e. delay)
      
      lcd.setCursor(2,1);   // LCD display seconds
      lcd.print("Time: ");
      lcd.setCursor(8,1);   
      lcd.print(timings[arrayPlace]);
    }
  }
}

void valve(){
   if(dripRate <= 0.2){
      digitalWrite(solenoidPin, LOW);
   }
   else if(millis() - lastDripTime >= dripRate){
      // Serial.println("Drip!");
      lastDripTime += dripRate;
      digitalWrite(solenoidPin, HIGH);
      delay(45); 
  }
  digitalWrite(solenoidPin, LOW);
}

void loop() {
  pot();
  valve();

//  digitalWrite(solenoidPin, HIGH); // ON
//  delay(45);                       // wait # milliseconds (quick close to let only a drop through)
//  digitalWrite(solenoidPin, LOW);  // off
//  delay(dripRate);                 // wait # seconds
}
