int raw_adc;
float voltage, K, C;

const float adc_max_mV  = 5000.0f;
const float adc_max_val = 1023.0f;
const float invT0 = 1.00 / 298.15;
const float invBeta = 1.00 / 4450.00;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  raw_adc = analogRead(A0);
  voltage = (adc_max_mV / adc_max_val) * raw_adc;
  K = 1.00 / (invT0 + invBeta*(log ( adc_max_val / (float) raw_adc - 1.00)));
  C = K - 273.15;
  Serial.print("Gerilim: "); Serial.println(voltage);
  Serial.print("Celcius: "); Serial.println(C);
  Serial.println();
  delay(500);
}
