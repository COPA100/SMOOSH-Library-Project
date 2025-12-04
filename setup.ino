    int SenseTrig1=7;
    int SenseEcho1=6;
    int SenseTrig2=5;
    int SenseEcho2=4;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    pinMode(SenseTrig1, OUTPUT);
    pinMode(SenseEcho1, INPUT);
    pinMode(SenseTrig2, OUTPUT);
    pinMode(SenseEcho2, INPUT);
}

void loop() {
    long duration1, duration2;
    float distanceCm1, distanceCm2;
    digitalWrite(SenseTrig1, LOW);
    delayMicroseconds(2);
    digitalWrite(SenseTrig2, LOW);
    delayMicroseconds(2);
    digitalWrite(SenseTrig1, HIGH);
    delayMicroseconds(10);
    digitalWrite(SenseTrig1, LOW);
    duration1 = pulseIn(SenseEcho1, HIGH);
    distanceCm1 = duration1 * 0.034 / 2;

    digitalWrite(SenseTrig2, HIGH);
    delayMicroseconds(10);
    digitalWrite(SenseTrig2, LOW);
    duration2 = pulseIn(SenseEcho2, HIGH);
    distanceCm2 = duration2 * 0.034 / 2;
    float averageD = (distanceCm1+distanceCm2)/2;
    Serial.print(abs(distanceCm2 - distanceCm1));
    Serial.print(" "); 
  // Print the average (reoved (char) so you see the actual number)
    Serial.println(averageD);    
    delay(400);
  // put your main code here, to run repeatedly:

}
