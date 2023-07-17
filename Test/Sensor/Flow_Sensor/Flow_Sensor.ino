volatile int pulsa_sensor;
unsigned int literPerjam;
unsigned char pinFlowsensor = 2;
unsigned long waktuAktual;
unsigned long waktuLoop;
double liter;

void cacahPulsa()
{
  pulsa_sensor++;
}

void setup()
{
  pinMode(pinFlowsensor, INPUT);
  digitalWrite(pinFlowsensor, HIGH);
  Serial.begin(9600);
  attachInterrupt(0, cacahPulsa, RISING);
  waktuAktual = millis();
  waktuLoop = waktuAktual;
}

void loop()
{
  waktuAktual = millis();
  if(waktuAktual >= (waktuLoop + 1000))
  {
    waktuLoop = waktuAktual;
    literPerjam = (pulsa_sensor*60/7.5);
    pulsa_sensor = 0;
    Serial.print(literPerjam, DEC);
    Serial.println(" L/jam");
  }
}
