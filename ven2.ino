    #include <LiquidCrystal_I2C.h>
    int Green =12;
    int Red=10;
    int Yellow=11;


    int SenseTrig1=7;
    int SenseEcho1=6;
    int SenseTrig2=5;
    int SenseEcho2=4;

    int Distance = 50;
    int peopleCount = 0;
    double maxCapacity =10; //of brown library?

    bool pass1 =false;
    bool pass2 = false;

    LiquidCrystal_I2C lcd(0x27, 16, 2);
    void setup() {
    Serial.begin(9600);
    pinMode(Green, OUTPUT);
    pinMode(Yellow, OUTPUT);
    pinMode(Red,OUTPUT);

    pinMode(SenseTrig1, OUTPUT);
    pinMode(SenseEcho1, INPUT);
    pinMode(SenseTrig2, OUTPUT);
    pinMode(SenseEcho2, INPUT);
    // put your setup code here, to run once:
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("In Library: 0");
    lcd.setCursor(0, 1);
    lcd.print("Capacity: 0%");  
    digitalWrite(Green, HIGH);
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
        bool justDetected1 = distanceCm1<Distance;
        bool justDetected2 = distanceCm2<Distance;

    if(justDetected1&&!pass1 && !pass2){
        pass1=true;
        Serial.println("Sensor 1 (Outer) triggered");
    }

    else if(justDetected2&&!pass1 && !pass2){
        pass2=true;
        Serial.println("Sensor 2 (Inner) triggered");
    }

    else if(justDetected1&&!pass1 && pass2){
        pass1=true;
        peopleCount++;
        Serial.println("ENTER sequence (1 -> 2). Count +1");
    }

    else if(justDetected2&&pass1 && !pass2){
        pass2=true; 
        Serial.println("EXIT sequence (2 -> 1). Count -1");
        peopleCount--;
    }


    else if (!justDetected1 && !justDetected2) {
        if (pass1 || pass2) { 
        Serial.println("Sensors clear. Resetting flags.");
        }
        pass1 = false;
        pass2 = false;
    }
    if(peopleCount<0){
        peopleCount =0;
    }  
    float currentPercentage = 0.0;
    currentPercentage = ((float)peopleCount / (float)maxCapacity) * 100.0;
    
    
    // --- NEW: Call our custom functions ---
    // All that old LCD code is now just one line:
    updateLCD(peopleCount, (int)currentPercentage);
    
    // All that old LED code is now just one line:
    updateLED((int)currentPercentage);

    delay(100);

    }

    void updateLCD(int count, int percentage) {
    lcd.clear();
    
    lcd.setCursor(0, 0); 
    lcd.print("In Library: ");
    lcd.print(count);
    
    lcd.setCursor(0, 1);
    lcd.print("Est Capacity: ");
    lcd.print(percentage);
    lcd.print("%");
    }
    void updateLED(int percentage){
    if (percentage > 70) {
        digitalWrite(Green, LOW);
        digitalWrite(Yellow, LOW);
        digitalWrite(Red, HIGH);
    } else if (percentage > 40) {
        digitalWrite(Green, LOW);
        digitalWrite(Yellow, HIGH);
        digitalWrite(Red, LOW);
    } else { 
        digitalWrite(Green, HIGH);
        digitalWrite(Yellow, LOW);
        digitalWrite(Red, LOW);
    }
    }


