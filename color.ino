#define S2 2
#define S3 3
#define OUT 4

unsigned long count;

// Valori minimi e massimi rilevati
const unsigned long R_min = 100;  // nero
const unsigned long R_max = 285;  // bianco
const unsigned long G_min = 80;
const unsigned long G_max = 274;
const unsigned long B_min = 125;
const unsigned long B_max = 408;

void setup() {
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  Serial.begin(9600);
}

// Misura la frequenza su 100 ms
unsigned long readFreq(bool s2, bool s3) {
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  delay(10);

  count = 0;
  unsigned long start = millis();

  while (millis() - start < 10) {  
    if (digitalRead(OUT) == HIGH) {
      while (digitalRead(OUT) == HIGH); // aspetta fronte
      count++;
    }
  }
  return count;
}

// Normalizza tra 0 e 255
int normalize(long val, long minVal, long maxVal) {
  long v = map(val, minVal, maxVal, 0, 255);
  if(v < 0) v = 0;
  if(v > 255) v = 255;
  return (int)v;
}

void loop() {
  unsigned long rawR = readFreq(LOW, LOW);
  unsigned long rawB = readFreq(LOW, HIGH);
  unsigned long rawG = readFreq(HIGH, HIGH);

  int R = normalize(rawR, R_min, R_max);
  int G = normalize(rawG, G_min, G_max);
  int B = normalize(rawB, B_min, B_max);

  Serial.print("R: "); Serial.print(R);
  Serial.print(" G: "); Serial.print(G);
  Serial.print(" B: "); Serial.println(B);

  // Determina colore
  if(R > 150 && G > 150 && B > 150) {
    Serial.println("Colore dominante: BIANCO");
  }
  else if (R < 75 && G < 75 && B < 75) {
    Serial.println("Colore dominante: NERO");
  } 
  else if (R > 150 && G > 150 && B < 150) {
    Serial.println("Colore dominante: GIALLO");
  } 
  else if (R > G && R > B) {
    Serial.println("Colore dominante: ROSSO");
  } 
  else if (G > R && G > B) {
    Serial.println("Colore dominante: VERDE");
  } 
  else if (B > R && B > G) {
    Serial.println("Colore dominante: BLU");
  } 
  else {
    Serial.println("Colore dominante: NON DEFINITO");
  }

  delay(500);
}
