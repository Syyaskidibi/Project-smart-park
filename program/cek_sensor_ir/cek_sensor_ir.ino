#define SENSOR1  13
#define SENSOR2  14
#define SENSOR3  22
#define SENSOR4  23

void setup() {
  Serial.begin(115200);

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
}

void loop() {
  int ir1 = !digitalRead(SENSOR1);
  int ir2 = !digitalRead(SENSOR2);
  int ir3 = !digitalRead(SENSOR3);
  int ir4 = !digitalRead(SENSOR4);

  Serial.print("IR1: ");
  Serial.print(ir1);
  Serial.print(" | IR2: ");
  Serial.print(ir2);
  Serial.print(" | IR3: ");
  Serial.print(ir3);
  Serial.print(" | IR4: ");
  Serial.println(ir4);

  delay(300);
}


