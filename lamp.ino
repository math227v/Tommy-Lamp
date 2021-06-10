#define FAN_PIN 6
#define LED_SIG_PIN 5
#define POT_PIN 15 // A0, A1, A2 = 14, 15, 16
#define MODE_SWITCH_PIN 1
#define MAIN_SWITCH_PIN 19
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

void setup() {
	for (int i = 0; i < NUMBER_OF_PINS; i++) {
		pinMode(pins[i].pinNum, pins[i].pinType);
		digitalWrite(pins[i].pinNum, LOW);
	}
};

void loop() {
	if (digitalRead(MAIN_SWITCH_PIN) == HIGH) {
		digitalWrite(FAN_PIN, HIGH);
		if (digitalRead(MODE_SWITCH_PIN) == HIGH) {
			analogWrite(LED_SIG_PIN, map(analogRead(POT_PIN), 0, 1023, 0, 255));
			blinkPin(STATUS_LED_PIN);
		} else {
			digitalWrite(LED_SIG_PIN, HIGH);
			digitalWrite(STATUS_LED_PIN, HIGH);
		}
	} else {
		digitalWrite(FAN_PIN, LOW);
		digitalWrite(STATUS_LED_PIN, LOW);
		digitalWrite(LED_SIG_PIN, LOW);
	}
};

void blinkPin(uint8_t pin) {
	if ((millis() % 1000) < 500) digitalWrite(pin, HIGH);
	else digitalWrite(pin, LOW);
}