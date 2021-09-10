int raw_adc;
float voltage;
float temp;

const float adc_max_mV  = 5000.0f;
const float adc_max_val = 4095.0f;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  raw_adc = analogRead(A5);
  voltage = (adc_max_mV / adc_max_val) * raw_adc;
  temp = ((10.888 - sqrt( sq(-10.888) + 4 * 0.00347 * (1777.3 - voltage) )) / (2 * (-0.00347))) + 30;
  
  Serial.print("ham adc: "); Serial.println(raw_adc);
  Serial.print("gerilim: "); Serial.println(voltage);
  Serial.print("sıcaklık: "); Serial.println(temp);
  Serial.println();
  
  delay(500);
}
