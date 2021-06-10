#define FAN_PIN 6
#define LED_SIG_PIN 5
#define POT_PIN 15 // A0, A1, A2 = 14, 15, 16
#define MODE_SWITCH_PIN 19
#define MAIN_SWITCH_PIN 1
#define STATUS_LED_PIN 13

#define NUMBER_OF_PINS 6
struct pinArray
{
	uint8_t pinNum;
	uint8_t pinType;
};
pinArray pins[] = { 
	{FAN_PIN, OUTPUT},
	{LED_SIG_PIN, OUTPUT},
	{POT_PIN, INPUT},
	{MODE_SWITCH_PIN, INPUT},
	{MAIN_SWITCH_PIN, INPUT},
	{STATUS_LED_PIN, OUTPUT}
};

uint16_t blinkInterval = 1024;
uint8_t fadeVal = 255;
uint8_t mode = 0;
boolean modeSwitchPos;

void setup() {
	for (int i = 0; i < NUMBER_OF_PINS; i++) {
		pinMode(pins[i].pinNum, pins[i].pinType);
		digitalWrite(pins[i].pinNum, LOW);
	}
	modeSwitchPos = LOW;
};

void loop() {
	// Handle mode change
	if (modeSwitchPos != digitalRead(MODE_SWITCH_PIN)) {
		// Anti Bounce
		delay(10);
		if (modeSwitchPos != digitalRead(MODE_SWITCH_PIN)) {
			// Loop through all pins, and write them to LOW
			for (int i = 0; i < NUMBER_OF_PINS; i++) { digitalWrite(pins[i].pinNum, LOW); }
			
			mode++; 
			modeSwitchPos = digitalRead(MODE_SWITCH_PIN);
		}
	}


	// Force Fan On
	// digitalWrite(FAN_PIN, HIGH);


	switch ( mode ) {
		// Constant ON Mode
		case 0:
		case 2:
			// digitalWrite(LED_SIG_PIN, HIGH);
			controlLED( 255 );
			break;

		// Fade Mode		
		case 1:
			fadeVal = analogRead(POT_PIN) / 4;
			controlLED( fadeVal );
			break;
		
		// Blink Mode
		case 3:
			blinkInterval = analogRead( POT_PIN );
			blinkLED( blinkInterval, fadeVal );
			break;	

		default:
			mode = 0;
			break;
	}

	digitalWrite( STATUS_LED_PIN, digitalRead(MAIN_SWITCH_PIN) );
};

void blinkPin(uint8_t pin) {
	if ((millis() % 1000) < 500) digitalWrite(pin, HIGH);
	else digitalWrite(pin, LOW);
}

// uint16_t deltaTime = 0;
uint16_t lastBlink = 0;
uint16_t smoothedInterval = 0;

void blinkLED( uint16_t totalInterval, uint8_t powerLevel ) {
	// If the input deviates more than 25 from the current value, update it.
	// Prevents jumpy values
	if ( abs( totalInterval - smoothedInterval ) > 25 ) smoothedInterval = totalInterval;

	#define MIN_BLINK_PERIOD 30
	#define MAX_BLINK_PERIOD 1000

	uint16_t mappedInterval = map( smoothedInterval, 0, 1023, MIN_BLINK_PERIOD, MAX_BLINK_PERIOD );
	uint16_t deltaTime = millis() - lastBlink;

	if ( deltaTime < ( mappedInterval / 2 )) controlLED( powerLevel );
	else {
		controlLED( 0x0 );
		if ( deltaTime >= mappedInterval ) lastBlink = millis();
	}
}

void blockPin(uint8_t pin) { digitalWrite(pin, LOW); }

void controlLED(uint16_t amount) {
	if (digitalRead(MAIN_SWITCH_PIN) == HIGH) {
		analogWrite(LED_SIG_PIN, amount);
	} else {
		digitalWrite(LED_SIG_PIN, LOW);
	}
}