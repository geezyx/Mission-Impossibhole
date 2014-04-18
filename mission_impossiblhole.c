// Mission Impossibhole
// 2.13.2013
// Mike Gee

int sensorLaser[8] = {};         //{sensor pin, laser pin, threshold, triggered millis}
int laserPins[8] = {             // pins with lasers attached
  6, 7, 8, 9, 10, 11, 12, 13 };  // O3, D7, D8, O2, O1, O0, D12, D13 ports on tinkerkit
int sensorPins[8] = {            // pins with sensors attached
  0, 1, 2, 3, 4, 5, 6, 7 };      // I0, I1, I2, I3, I4, I5, I6, I7 ports on tinkerkit
int alarmPin = 2;
int threshold[8] = {
  15, 15, 15, 15, 15, 15, 15, 15 };
long timeTriggered[8] = {
  0, 0, 0, 0, 0, 0, 0, 0 };
int numPins = 1;                 // pin count used for testing
int buttonPin = 31;               // D2 IO port on tinkerkit used for calibration
long mainAlarm = 0;
long time = 0;
long alarmTime = 0;

void setup()
{
  Serial.begin(9600);
  pinMode( buttonPin, INPUT );
  pinMode( alarmPin, OUTPUT );
  digitalWrite( buttonPin, HIGH );
  digitalWrite( alarmPin, LOW );
  for ( int i=0; i < numPins; i++ ) {
    
    pinMode( laserPins[i], OUTPUT );
  };
  //calibrateSystem();
}

void loop()
{
  for ( int i=0; i < numPins; i++ ) {
    time = millis()-timeTriggered[i];
    if ( time > 3000 ) {
      digitalWrite( sensorLaser[i], LOW );
      delay(10);
      analogRead(sensorPins[i]); //quick reading to prime sensor
      if ( analogRead(sensorPins[i]) < threshold[i] ) {
        Serial.print( "alarm triggered at " );
        timeTriggered[i] = millis();
        mainAlarm = millis();
        Serial.println( timeTriggered[i] );
      }
    }
    if ( time <= 3000 ) {
      digitalWrite( sensorLaser[i], (millis() % 500) > 250 ? HIGH : LOW ); //blink while triggered
    }
  }
  alarmTime = millis()-mainAlarm;
  Serial.println( alarmTime );
  if ( alarmTime < 1000 ) {
    digitalWrite( alarmPin, (millis() % 500) > 250 ? HIGH : LOW );
  }
  else {
    digitalWrite( alarmPin, LOW );
  }
  if ( digitalRead(buttonPin) == LOW ) {
    calibrateSystem();
  }
}

void calibrateSystem() {
  Serial.println( "calibrating" );
  digitalWrite( alarmPin, LOW );
  for ( int i=0; i < numPins; i++ ) { // turn on all the lasers
    digitalWrite( laserPins[i], LOW );
  }
  delay ( 2000 ); // delay to prevent calibrate from beginning prematurely
  Serial.println( "press any key" );
  while ( digitalRead(buttonPin) == HIGH ) {}; //wait for button press
  Serial.println( "key pressed" );
  Serial.println( "turning off lasers" );
  for ( int i=0; i < numPins; i++ ) { // turn off all the lasers
    digitalWrite( laserPins[i], HIGH );
  }
  for ( int i=0; i < numPins; i++ ) {
    sensorPins[i] = sensorPins[i]; //set sensor pin
    timeTriggered[i] = 0;
    Serial.print( "setting sensor pin " );
    Serial.print( i );
    Serial.print( " threshold " );
    analogRead(sensorPins[i]);
    delay(1000);
    Serial.println( analogRead(sensorPins[i])+5 );
    threshold[i] = analogRead(sensorPins[i])+5; //set threshold for sensor
  }
  Serial.println( "pairing lasers and sensors" );
  for ( int i=0; i < numPins; i++ ) {
    for ( int j=0; j < numPins; j++ ) {
      digitalWrite( laserPins[j], LOW );
      delay(250);
      if ( analogRead(sensorPins[i]) > threshold[i] ) {
        sensorLaser[i] = laserPins[j];
        delay(2000);
        digitalWrite( laserPins[j], HIGH );
        break;
      }
      delay(250);
      digitalWrite( laserPins[j], HIGH );
    }
  }
}
