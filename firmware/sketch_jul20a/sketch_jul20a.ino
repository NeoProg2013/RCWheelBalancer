



void setup() {
    Serial.begin(115200);
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
}

void loop() {

    int a0 = analogRead(A0);
    int a1 = analogRead(A1);
    Serial.print(a0);
    Serial.print(" ");
    Serial.println(a1);
}
