int pulse_count = 0;

void pulseInterrupt() {
  pulse_count++;
}

int32_t start_time = 0;
int32_t stop_time = 0;
int32_t pwm = 0;

void setup() {
    Serial.begin(115200);
    pinMode(3, OUTPUT);
    analogWrite(3, 0);

    pinMode(2, INPUT);

    attachInterrupt(0, pulseInterrupt, RISING);

    start_time = micros();
}

void loop() {
  if (Serial.available() > 0) {
    int n = Serial.parseInt();
    Serial.println(n);
    analogWrite(3, n);
  }

  if (micros() - start_time > 100000) {
    const int32_t target = 40;
    if (pulse_count < target) {
      pwm++;
      if (pwm > 500) {
        pwm = 500;
      }
    } else if (pulse_count > target) {
      pwm--;
      if (pwm < 0) {
        pwm = 0;
      }
    }
    
    Serial.print(pulse_count);
    Serial.print(" pwm=");
    Serial.println(pwm);

    pulse_count = 0;
    start_time = micros();

    analogWrite(3, pwm);
  }
}
