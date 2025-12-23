#include <Servo.h>

Servo myservo1;

// Entry gate IR sensors
int ir_s1 = 2;
int ir_s2 = 4;

// Parking slot IR sensors
int ir_p_1 = 13;
int ir_p_2 = 9;
int ir_p_3 = 10;

int total = 0;
int slot = 3; // Total parking capacity
int flag1 = 0;
int flag2 = 0;

int s1 = 0, s2 = 0, s3 = 0;

void setup()
{
    Serial.begin(9600);  // Serial Monitor শুরু

    pinMode(ir_s1, INPUT);
    pinMode(ir_s2, INPUT);
    pinMode(ir_p_1, INPUT);
    pinMode(ir_p_2, INPUT);
    pinMode(ir_p_3, INPUT);

    myservo1.attach(3);   // Servo on pin 3
    myservo1.write(180);  // Gate closed
}

void loop()
{
    read_sensor();

    // Parking status print
    Serial.print("Available Slots: ");
    Serial.println(slot);

    Serial.print("S1: ");
    Serial.print(s1 ? "Full" : "Empty");
    Serial.print(" | S2: ");
    Serial.print(s2 ? "Full" : "Empty");
    Serial.print(" | S3: ");
    Serial.println(s3 ? "Full" : "Empty");

    // Entry sensor logic
    if (digitalRead(ir_s1) == LOW && flag1 == 0)
    {
        if (slot > 0)
        {
            flag1 = 1;
            if (flag2 == 0)
            {
                myservo1.write(90); // Gate open
                Serial.println("Gate Opened for Entry");
                slot = slot - 1;
            }
        }
        else
        {
            Serial.println("Parking Full! Entry Denied");
            delay(1500);
        }
    }

    // Exit sensor logic
    if (digitalRead(ir_s2) == LOW && flag2 == 0)
    {
        flag2 = 1;
        if (flag1 == 0)
        {
            myservo1.write(90); // Gate open
            Serial.println("Gate Opened for Exit");
            slot = slot + 1;
        }
    }

    // Close gate after both flags are triggered
    if (flag1 == 1 && flag2 == 1)
    {
        delay(1000);
        myservo1.write(180); // Gate closed
        Serial.println("Gate Closed");
        flag1 = 0;
        flag2 = 0;
    }

    delay(500);
}

void read_sensor()
{
    s1 = (digitalRead(ir_p_1) == LOW) ? 1 : 0;
    s2 = (digitalRead(ir_p_2) == LOW) ? 1 : 0;
    s3 = (digitalRead(ir_p_3) == LOW) ? 1 : 0;
}
