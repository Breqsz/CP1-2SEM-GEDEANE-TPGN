// =====================================================================
//  Carrinho 2 motores + Ponte H L298N + HC-SR04 + Bluetooth (HC-05/06)
//  + LED de alerta + controle de velocidade
//  Comandos: F = frente | R = ré | E = curva esquerda | D = curva direita
//            P = parar  | 1 a 9 e 0 = velocidade
//  Project-based Maker Lab - FIAP
// =====================================================================
#include <SoftwareSerial.h>

// ---------------------- PINAGEM ----------------------
// Bluetooth: pino 2 = RX do Arduino (TXD do módulo) | pino 3 = TX (RXD do módulo)
SoftwareSerial bluetooth(2, 3);

// Ponte H L298N (pinos PWM ~, jumpers ENA/ENB mantidos)
const int IN1 = 5;   // Motor A (motor ESQUERDO)
const int IN2 = 6;   // Motor A
const int IN3 = 9;   // Motor B (motor DIREITO)
const int IN4 = 10;  // Motor B

// Sensor ultrassônico HC-SR04
const int TRIGGER = 12;
const int ECHO    = 11;

// LED vermelho: anodo -> resistor 220 ohms -> pino 4 | catodo -> GND
const int LED_ALERTA = 4;

// ---------------------- AJUSTES ----------------------
const int DISTANCIA_OBSTACULO = 20;           // cm: acende o LED e para o carrinho
const unsigned long TEMPO_RE = 600;           // ms de ré após detectar obstáculo
const unsigned long INTERVALO_LEITURA = 60;   // ms entre leituras do sensor

// Curva: % da velocidade que a roda de DENTRO mantém.
// Menor = curva mais fechada. 0 faz a roda de dentro parar.
const int FATOR_CURVA = 40;

// Velocidade: 0 = parado, 255 = máxima. Abaixo de ~120 o motor não vence o atrito.
int velocidade = 255;

// Estado: F = frente | R = ré | E = curva esquerda | D = curva direita | P = parado
char estado = 'P';
unsigned long ultimaLeitura = 0;

// ---------------------- MOTORES ----------------------
// vE e vD: velocidade de cada lado (0 a 255). Negativo = para trás.
void motores(int vE, int vD) {
  if (vE >= 0) { analogWrite(IN1, vE);   digitalWrite(IN2, LOW); }
  else         { digitalWrite(IN1, LOW); analogWrite(IN2, -vE);  }

  if (vD >= 0) { analogWrite(IN3, vD);   digitalWrite(IN4, LOW); }
  else         { digitalWrite(IN3, LOW); analogWrite(IN4, -vD);  }
}

int velInterna() {
  return (int)((long)velocidade * FATOR_CURVA / 100);
}

void frente()   { motores( velocidade,   velocidade);  }
void re()       { motores(-velocidade,  -velocidade);  }
void esquerda() { motores( velInterna(), velocidade);  }  // curva à esquerda
void direita()  { motores( velocidade,   velInterna()); } // curva à direita
void parar()    { motores(0, 0); }

// Reaplica o movimento atual (usado ao trocar a velocidade)
void aplicarEstado() {
  switch (estado) {
    case 'F': frente();   break;
    case 'R': re();       break;
    case 'E': esquerda(); break;
    case 'D': direita();  break;
    default:  parar();    break;
  }
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

void desviarObstaculo(long d) {
  parar();
  Serial.print("Obstaculo a "); Serial.print(d); Serial.println(" cm");
  bluetooth.println("Obstaculo! Parando e dando re...");
  delay(200);

  re();
  delay(TEMPO_RE);
  parar();

  estado = 'P';
  bluetooth.println("Parado. Envie F para seguir.");
}

// ---------------------- COMANDOS ----------------------
void processarComando(char c) {
  c = toupper(c);

  // Teclas 1 a 9 = 10% a 90% | tecla 0 = 100%
  if (c >= '0' && c <= '9') {
    int porcento = (c == '0') ? 100 : (c - '0') * 10;
    velocidade = map(porcento, 0, 100, 0, 255);
    aplicarEstado();
    Serial.print("Velocidade: "); Serial.print(porcento); Serial.println("%");
    bluetooth.print("Velocidade: "); bluetooth.print(porcento); bluetooth.println("%");
    return;
  }

  switch (c) {
    case 'F': estado = 'F'; frente();   bluetooth.println("Frente");   break;
    case 'R': estado = 'R'; re();       bluetooth.println("Re");       break;
    case 'E': estado = 'E'; esquerda(); bluetooth.println("Esquerda"); break;
    case 'D': estado = 'D'; direita();  bluetooth.println("Direita");  break;
    case 'P': estado = 'P'; parar();    bluetooth.println("Parado");   break;
    default: return;                         // ignora \r, \n e outras teclas
  }
  Serial.print("Comando: "); Serial.println(c);
}

// ---------------------- SETUP / LOOP ----------------------
void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED_ALERTA, OUTPUT);
  digitalWrite(TRIGGER, LOW);
  digitalWrite(LED_ALERTA, LOW);

  parar();                  // começa parado, esperando comando

  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("Pronto: F=frente R=re E=esq D=dir P=parar | 1-9 e 0 = velocidade");
  bluetooth.println("Pronto: F=frente R=re E=esq D=dir P=parar | 1-9 e 0 = velocidade");
}

void loop() {
  if (bluetooth.available() > 0) {
    processarComando(bluetooth.read());
  }
  if (Serial.available() > 0) {
    processarComando(Serial.read());
  }

  // Sensor sempre ativo, para o LED funcionar em qualquer estado
  if (millis() - ultimaLeitura >= INTERVALO_LEITURA) {
    ultimaLeitura = millis();
    long d = medirDistancia();

    if (d < DISTANCIA_OBSTACULO) {
      digitalWrite(LED_ALERTA, HIGH);        // objeto detectado
      // Para nos movimentos que avançam: frente e curvas
      if (estado == 'F' || estado == 'E' || estado == 'D') {
        desviarObstaculo(d);
      }
    } else {
      digitalWrite(LED_ALERTA, LOW);
    }
  }
}
