// =====================================================================
//  CP2 - Teste de hardware do carrinho com ESP32-S3-DevKitC-1
//  Comandos pelo Serial Monitor (115200):
//    F = frente | R = ré | E = esquerda | D = direita | P = parar
//    S = liga/desliga leitura do sensor | L = LED | B = buzzer | C = RGB
//  Project-based Maker Lab - FIAP
// =====================================================================

// ---------------------- PINAGEM ----------------------
// Ponte H L298N (jumpers ENA/ENB mantidos, PWM nos pinos IN)
const int IN1 = 4;   // Motor ESQUERDO
const int IN2 = 5;
const int IN3 = 6;   // Motor DIREITO
const int IN4 = 7;

// HC-SR04 (ECHO passa pelo divisor 1k/2k)
const int TRIGGER = 15;
const int ECHO    = 16;

const int LED_ALERTA = 17;
const int BUZZER     = 18;
const int PINO_RGB   = 48;   // em placas v1.1 trocar para 38

const int VELOCIDADE = 200;  // 0 a 255

bool lerSensor = false;
bool ledLigado = false;
bool buzzerLigado = false;
int corRGB = 0;
unsigned long ultimaLeitura = 0;

// ---------------------- MOTORES ----------------------
// vE e vD: velocidade de cada lado (0 a 255). Negativo = para trás.
void motores(int vE, int vD) {
  if (vE >= 0) { analogWrite(IN1, vE); analogWrite(IN2, 0);   }
  else         { analogWrite(IN1, 0);  analogWrite(IN2, -vE); }

  if (vD >= 0) { analogWrite(IN3, vD); analogWrite(IN4, 0);   }
  else         { analogWrite(IN3, 0);  analogWrite(IN4, -vD); }
}

// ---------------------- SENSOR ----------------------
long medirDistancia() {
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  long t = pulseIn(ECHO, HIGH, 30000UL);     // timeout de 30 ms (~5 m)
  if (t == 0) return 999;                    // sem eco = nada à frente
  return t / 58;                             // tempo -> cm
}

// ---------------------- RGB ----------------------
void escreverRGB(int r, int g, int b) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  rgbLedWrite(PINO_RGB, r, g, b);
#else
  neopixelWrite(PINO_RGB, r, g, b);
#endif
}

void trocarCorRGB() {
  corRGB = (corRGB + 1) % 4;
  switch (corRGB) {
    case 0: escreverRGB(0, 0, 0);   Serial.println("RGB apagado");  break;
    case 1: escreverRGB(0, 40, 0);  Serial.println("RGB verde");    break;
    case 2: escreverRGB(40, 25, 0); Serial.println("RGB amarelo");  break;
    case 3: escreverRGB(40, 0, 0);  Serial.println("RGB vermelho"); break;
  }
}

// ---------------------- COMANDOS ----------------------
void processarComando(char c) {
  switch (toupper(c)) {
    case 'F': motores( VELOCIDADE,  VELOCIDADE); Serial.println("Frente");   break;
    case 'R': motores(-VELOCIDADE, -VELOCIDADE); Serial.println("Re");       break;
    case 'E': motores(-VELOCIDADE,  VELOCIDADE); Serial.println("Esquerda"); break;
    case 'D': motores( VELOCIDADE, -VELOCIDADE); Serial.println("Direita");  break;
    case 'P': motores(0, 0);                     Serial.println("Parado");   break;
    case 'S':
      lerSensor = !lerSensor;
      Serial.println(lerSensor ? "Sensor ligado" : "Sensor desligado");
      break;
    case 'L':
      ledLigado = !ledLigado;
      digitalWrite(LED_ALERTA, ledLigado);
      Serial.println(ledLigado ? "LED ligado" : "LED desligado");
      break;
    case 'B':
      buzzerLigado = !buzzerLigado;
      digitalWrite(BUZZER, buzzerLigado);
      Serial.println(buzzerLigado ? "Buzzer ligado" : "Buzzer desligado");
      break;
    case 'C': trocarCorRGB(); break;
    default: return;                         // ignora \r, \n e outras teclas
  }
}

// ---------------------- SETUP / LOOP ----------------------
void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_ALERTA, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(TRIGGER, LOW);
  digitalWrite(LED_ALERTA, LOW);
  digitalWrite(BUZZER, LOW);

  motores(0, 0);
  escreverRGB(0, 0, 0);

  Serial.begin(115200);
  delay(500);
  Serial.println("Teste de hardware - ESP32-S3");
  Serial.println("F/R/E/D/P = motores | S = sensor | L = LED | B = buzzer | C = RGB");
}

void loop() {
  if (Serial.available() > 0) {
    processarComando(Serial.read());
  }

  if (lerSensor && millis() - ultimaLeitura >= 200) {
    ultimaLeitura = millis();
    Serial.print("Distancia: ");
    Serial.print(medirDistancia());
    Serial.println(" cm");
  }
}
