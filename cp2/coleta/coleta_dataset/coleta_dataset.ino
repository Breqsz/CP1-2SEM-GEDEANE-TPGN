// =====================================================================
//  CP2 - Coleta de dados do HC-SR04 para o Edge Impulse
//  ESP32-S3-DevKitC-1
//  Comandos pelo Serial (115200):
//    1 = grava classe "perto" | 2 = "medio" | 3 = "longe" | X = pausa
//  Saída: linhas "distancia,label" prontas para o CSV.
//  Linhas que começam com '#' são mensagens (não entram no CSV).
//  Project-based Maker Lab - FIAP
// =====================================================================

// ---------------------- PINAGEM ----------------------
const int IN1 = 4;   // Motores ficam parados durante a coleta
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;

const int TRIGGER = 15;
const int ECHO    = 16;  // via divisor de tensão

// ---------------------- AJUSTES ----------------------
const float DISTANCIA_MAX = 200.0;            // cm: acima disso (ou sem eco) vira 200
const float DISTANCIA_MIN = 2.0;              // cm: limite inferior do sensor
const int   JANELA_MEDIA  = 5;                // leituras na média móvel
const unsigned long INTERVALO_AMOSTRA = 100;  // ms entre amostras

const char* CLASSES[] = { "perto", "medio", "longe" };

float leituras[JANELA_MEDIA];
int indiceLeitura = 0;

int classeAtual = -1;                          // -1 = pausado
int contagem[3] = { 0, 0, 0 };
unsigned long ultimaAmostra = 0;

// ---------------------- SENSOR ----------------------
float medirDistancia() {
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);

  long t = pulseIn(ECHO, HIGH, 30000UL);     // timeout de 30 ms (~5 m)
  if (t == 0) return DISTANCIA_MAX;          // sem eco = nada à frente
  float d = t / 58.0;                         // tempo -> cm
  return constrain(d, DISTANCIA_MIN, DISTANCIA_MAX);
}

// Média das últimas JANELA_MEDIA leituras (remove o ruído do sensor)
float distanciaFiltrada() {
  leituras[indiceLeitura] = medirDistancia();
  indiceLeitura = (indiceLeitura + 1) % JANELA_MEDIA;

  float soma = 0;
  for (int i = 0; i < JANELA_MEDIA; i++) soma += leituras[i];
  return soma / JANELA_MEDIA;
}

// ---------------------- COMANDOS ----------------------
void mostrarContagem() {
  Serial.print("# Amostras -> perto: "); Serial.print(contagem[0]);
  Serial.print(" | medio: ");            Serial.print(contagem[1]);
  Serial.print(" | longe: ");            Serial.print(contagem[2]);
  Serial.print(" | total: ");
  Serial.println(contagem[0] + contagem[1] + contagem[2]);
}

void processarComando(char c) {
  c = toupper(c);
  if (c >= '1' && c <= '3') {
    classeAtual = c - '1';
    Serial.print("# Gravando classe: "); Serial.println(CLASSES[classeAtual]);
  } else if (c == 'X') {
    classeAtual = -1;
    Serial.println("# Pausado");
    mostrarContagem();
  }
}

// ---------------------- SETUP / LOOP ----------------------
void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  digitalWrite(TRIGGER, LOW);

  Serial.begin(115200);
  delay(500);

  // Preenche a média com leituras reais antes de começar
  for (int i = 0; i < JANELA_MEDIA; i++) {
    leituras[i] = medirDistancia();
    delay(30);
  }

  Serial.println("# Coleta HC-SR04 - ESP32-S3");
  Serial.println("# 1 = perto | 2 = medio | 3 = longe | X = pausa");
}

void loop() {
  if (Serial.available() > 0) {
    processarComando(Serial.read());
  }

  if (millis() - ultimaAmostra >= INTERVALO_AMOSTRA) {
    ultimaAmostra = millis();
    float d = distanciaFiltrada();      // sempre lê, para a média ficar atualizada

    if (classeAtual >= 0) {
      Serial.print(d, 1);
      Serial.print(",");
      Serial.println(CLASSES[classeAtual]);
      contagem[classeAtual]++;
    }
  }
}
