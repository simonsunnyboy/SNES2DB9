// Basic DB9 Joystick simulator
// (c) 2020 by Matthias Arndt <marndt@asmsoftware.de>
//
// - digital lins to DB9 port from Arduino are given in array below
// - lines are active low, e.q. GND if joystick action has been taken
// - simulates joystick clicks every s for each direction and fire
//

#define MAX_SIGNALS 5

// UP DOWN LEFT RIGHT FIRE

int db9_signal_pin[MAX_SIGNALS] = { 3, 4, 5, 6, 7 };

int current_pin_active = 1;

void setup() {
    // put your setup code here, to run once:

    for (int i = 0; i < MAX_SIGNALS; i++)
    {
        pinMode(db9_signal_pin[i], OUTPUT);
        digitalWrite(db9_signal_pin[i], 1);
    }

}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(db9_signal_pin[current_pin_active], 0);
    delay(100);
    digitalWrite(db9_signal_pin[current_pin_active], 1);
    delay(900);

    current_pin_active++;

    if (current_pin_active >= MAX_SIGNALS)
    {
        current_pin_active = 0;
    }


}
