// =====================================================================
//  CP2 - Carrinho-Robô com TinyML (Edge Impulse) no ESP32-S3
//  Sensor HC-SR04 -> modelo de classificação -> ação no carrinho
//
//  Classes do modelo e ações:
//    longe -> anda em velocidade normal  | RGB verde
//    medio -> anda devagar               | RGB amarelo, LED piscando
//    perto -> para, dá ré e gira         | RGB vermelho, LED + buzzer
//
//  Botão BOOT (ou 'G' no Serial) liga/desliga os motores.
//  Com os motores desligados o modelo continua rodando (LED/RGB/Serial).
//  Project-based Maker Lab - FIAP
// =====================================================================
#include <tinyml-carrinho-tpgn_inferencing.h>

// ---------------------- PINAGEM ----------------------
const int IN1 = 4;   // Motor ESQUERDO
const int IN2 = 5;
const int IN3 = 6;   // Motor DIREITO
const int IN4 = 7;

const int TRIGGER = 15;
const int ECHO    = 16;  // via divisor de tensão

const int LED_ALERTA = 17;
const int BUZZER     = 18;
const int PINO_RGB   = 48;  // em placas v1.1 trocar para 38
const int BOTAO_BOOT = 0;   // botão BOOT da placa

// ---------------------- AJUSTES ----------------------
// Pré-processamento: IGUAL ao usado na coleta do dataset
const float DISTANCIA_MAX = 200.0;            // cm: acima disso (ou sem eco) vira 200
const float DISTANCIA_MIN = 2.0;              // cm: limite inferior do sensor
const int   JANELA_MEDIA  = 5;                // leituras na média móvel
const unsigned long INTERVALO_AMOSTRA = 100;  // ms entre inferências

// Velocidades (0 a 255). Abaixo de ~120 o motor não vence o atrito.
const int VEL_NORMAL  = 160;
const int VEL_DEVAGAR = 125;
const int VEL_MANOBRA = 170;

// Manobra de desvio (classe "perto")
const unsigned long TEMPO_PARADA = 200;  // ms parado antes da ré
const unsigned long TEMPO_RE     = 600;  // ms de ré
const unsigned long TEMPO_GIRO   = 550;  // ms girando para a direita

// Confiança mínima para aceitar a classe; abaixo disso mantém a anterior
const float CONFIANCA_MINIMA = 0.60;

// ---------------------- ESTADO ----------------------
enum Classe { LONGE = 0, MEDIO = 1, PERTO = 2 };
enum Manobra { SEM_MANOBRA, PARANDO, DANDO_RE, GIRANDO };

float leituras[JANELA_MEDIA];
int indiceLeitura = 0;

Classe classeAtual = LONGE;
float confiancaAtual = 0;
Manobra manobra = SEM_MANOBRA;
unsigned long inicioManobra = 0;

bool motoresLigados = false;
bool botaoAnterior = HIGH;
unsigned long ultimaAmostra = 0;
unsigned long ultimoPisca = 0;
bool ledPisca = false;

// ---------------------- MOTORES ----------------------
// vE e vD: velocidade de cada lado (0 a 255). Negativo = para trás.
void motores(int vE, int vD) {
  if (!motoresLigados) { vE = 0; vD = 0; }

  if (vE >= 0) { analogWrite(IN1, vE); analogWrite(IN2, 0);   }
  else         { analogWrite(IN1, 0);  analogWrite(IN2, -vE); }

  if (vD >= 0) { analogWrite(IN3, vD); analogWrite(IN4, 0);   }
  else         { analogWrite(IN3, 0);  analogWrite(IN4, -vD); }
}

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

// ---------------------- INFERÊNCIA ----------------------
// Executa o modelo TinyML com a distância filtrada e atualiza a classe
bool classificar(float distancia, ei_impulse_result_t &resultado) {
  float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { distancia };

  signal_t sinal;
  numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &sinal);

  EI_IMPULSE_ERROR erro = run_classifier(&sinal, &resultado, false);
  if (erro != EI_IMPULSE_OK) {
    Serial.print("Erro na inferencia: "); Serial.println(erro);
    return false;
  }

  // Classe com maior probabilidade (ordem do modelo: longe, medio, perto)
  int melhor = 0;
  for (int i = 1; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    if (resultado.classification[i].value > resultado.classification[melhor].value) melhor = i;
  }

  if (resultado.classification[melhor].value >= CONFIANCA_MINIMA) {
    classeAtual = (Classe)melhor;
    confiancaAtual = resultado.classification[melhor].value;
  }
  return true;
}

// ---------------------- SAÍDAS VISUAIS ----------------------
void escreverRGB(int r, int g, int b) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
  rgbLedWrite(PINO_RGB, r, g, b);
#else
  neopixelWrite(PINO_RGB, r, g, b);
#endif
}

void atualizarSinais() {
  switch (classeAtual) {
    case LONGE:
      escreverRGB(0, 40, 0);
      digitalWrite(LED_ALERTA, LOW);
      digitalWrite(BUZZER, LOW);
      break;

    case MEDIO:
      escreverRGB(40, 25, 0);
      if (millis() - ultimoPisca >= 250) {     // LED piscando
        ultimoPisca = millis();
        ledPisca = !ledPisca;
        digitalWrite(LED_ALERTA, ledPisca);
      }
      digitalWrite(BUZZER, LOW);
      break;

    case PERTO:
      escreverRGB(40, 0, 0);
      digitalWrite(LED_ALERTA, HIGH);
      digitalWrite(BUZZER, manobra != SEM_MANOBRA && motoresLigados);
      break;
  }
}

// ---------------------- COMPORTAMENTO ----------------------
// Decide o movimento a partir da classe produzida pelo modelo
void agir() {
  if (manobra != SEM_MANOBRA) {               // manobra de desvio em andamento
    unsigned long t = millis() - inicioManobra;
    if (manobra == PARANDO && t >= TEMPO_PARADA) {
      manobra = DANDO_RE;  inicioManobra = millis();
      motores(-VEL_MANOBRA, -VEL_MANOBRA);
    } else if (manobra == DANDO_RE && t >= TEMPO_RE) {
      manobra = GIRANDO;   inicioManobra = millis();
      motores(VEL_MANOBRA, -VEL_MANOBRA);     // gira para a direita
    } else if (manobra == GIRANDO && t >= TEMPO_GIRO) {
      manobra = SEM_MANOBRA;
      motores(0, 0);
    }
    return;
  }

  switch (classeAtual) {
    case LONGE: motores(VEL_NORMAL, VEL_NORMAL);   break;
    case MEDIO: motores(VEL_DEVAGAR, VEL_DEVAGAR); break;
    case PERTO:
      if (motoresLigados) {
        manobra = PARANDO;
        inicioManobra = millis();
      }
      motores(0, 0);
      break;
  }
}

const char* nomeAcao() {
  if (!motoresLigados) return "MOTORES DESLIGADOS";
  switch (manobra) {
    case PARANDO:  return "PARANDO";
    case DANDO_RE: return "RE";
    case GIRANDO:  return "DESVIANDO";
    default: break;
  }
  switch (classeAtual) {
    case LONGE: return "FRENTE NORMAL";
    case MEDIO: return "FRENTE DEVAGAR";
    default:    return "PARADO";
  }
}

// ---------------------- COMANDOS ----------------------
void alternarMotores() {
  motoresLigados = !motoresLigados;
  manobra = SEM_MANOBRA;
  motores(0, 0);
  Serial.println(motoresLigados ? "# Motores LIGADOS" : "# Motores DESLIGADOS");
}

void verificarComandos() {
  if (Serial.available() > 0) {
    char c = toupper(Serial.read());
    if (c == 'G') alternarMotores();
  }

  bool botao = digitalRead(BOTAO_BOOT);
  if (botaoAnterior == HIGH && botao == LOW) {  // apertou o BOOT
    alternarMotores();
    delay(50);                                  // debounce
  }
  botaoAnterior = botao;
}

// ---------------------- SETUP / LOOP ----------------------
void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_ALERTA, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BOTAO_BOOT, INPUT_PULLUP);
  digitalWrite(TRIGGER, LOW);
  digitalWrite(LED_ALERTA, LOW);
  digitalWrite(BUZZER, LOW);

  motores(0, 0);
  escreverRGB(0, 0, 40);   // azul = iniciando

  Serial.begin(115200);
  delay(500);

  // Preenche a média com leituras reais antes de começar
  for (int i = 0; i < JANELA_MEDIA; i++) {
    leituras[i] = medirDistancia();
    delay(30);
  }

  Serial.println("# Carrinho TinyML - ESP32-S3");
  Serial.print("# Modelo: "); Serial.print(EI_CLASSIFIER_PROJECT_NAME);
  Serial.print(" | classes: "); Serial.println(EI_CLASSIFIER_LABEL_COUNT);
  Serial.println("# Aperte BOOT (ou envie G) para ligar/desligar os motores");
}

void loop() {
  verificarComandos();

  if (millis() - ultimaAmostra >= INTERVALO_AMOSTRA) {
    ultimaAmostra = millis();

    float d = distanciaFiltrada();
    ei_impulse_result_t resultado = { 0 };
    if (classificar(d, resultado)) {
      // dist=42.0cm | longe=0.01 medio=0.97 perto=0.02 | classe=medio (0.97) | acao=FRENTE DEVAGAR
      Serial.print("dist="); Serial.print(d, 1); Serial.print("cm | ");
      for (int i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        Serial.print(resultado.classification[i].label); Serial.print("=");
        Serial.print(resultado.classification[i].value, 2); Serial.print(" ");
      }
      Serial.print("| classe="); Serial.print(resultado.classification[classeAtual].label);
      Serial.print(" ("); Serial.print(confiancaAtual, 2); Serial.print(")");
      Serial.print(" | tempo="); Serial.print(resultado.timing.classification); Serial.print("ms");
      Serial.print(" | acao="); Serial.println(nomeAcao());
    }
  }

  agir();
  atualizarSinais();
}
