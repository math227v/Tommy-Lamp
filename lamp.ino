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

uint8_t blinkInterval = 255;
uint8_t fadeVal = 255;
uint8_t mode = 0;
boolean modeSwitchPos;

void setup() {
	for (int i = 0; i < NUMBER_OF_PINS; i++) {
		pinMode(pins[i].pinNum, pins[i].pinType);
		digitalWrite(pins[i].pinNum, LOW);
	}
	modeSwitchPos = LOW;

	Serial.begin(38400);
};

void loop() {
	if (modeSwitchPos != digitalRead(MODE_SWITCH_PIN)) {
		// Anti Bounce
		delay(10);
		if (modeSwitchPos != digitalRead(MODE_SWITCH_PIN)) {
			// Loop through all pins, and write them to LOW
			for (int i = 0; i < NUMBER_OF_PINS; i++) { digitalWrite(pins[i].pinNum, LOW); }
			
			mode++; 
			modeSwitchPos = digitalRead(MODE_SWITCH_PIN); 
			Serial.write(mode);
		}
	}

	// Force Fan On
	digitalWrite(FAN_PIN, HIGH);

	switch (mode) {
		// Constant ON Mode
		case 0:
		case 2:
			digitalWrite(LED_SIG_PIN, HIGH);
			break;

		// Fade Mode		
		case 1:
			fadeVal = analogRead(POT_PIN) / 4;
			analogWrite(LED_SIG_PIN, fadeVal);
			break;
		
		// Blink Mode
		case 3:
			blinkInterval = analogRead(POT_PIN);
			blinkPin(LED_SIG_PIN, blinkInterval, fadeVal);
			break;	

		default:
			mode = 0;
			break;
	}
};

void blinkPin(uint8_t pin) {
	if ((millis() % 1000) < 500) digitalWrite(pin, HIGH);
	else digitalWrite(pin, LOW);
}

// uint16_t deltaTime = 0;
// uint16_t lastBlink = 0;

void blinkPin(uint8_t pin, uint16_t totalInterval, uint8_t powerLevel) {
	uint8_t mappedInterval = map(totalInterval, 0, 255, 2500, 10);

	if ((millis() % totalInterval) < (totalInterval / 2)) analogWrite(pin, powerLevel);
	else digitalWrite(pin, LOW);
}