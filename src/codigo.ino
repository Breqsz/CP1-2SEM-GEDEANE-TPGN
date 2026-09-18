#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3);

const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 9;
const int IN4 = 10;

const int TRIGGER = 12;
const int ECHO = 11;

const int LED_ALERTA = 4;

const int DISTANCIA_LED = 20;
const int DISTANCIA_PARADA = 10;
const unsigned long TEMPO_RE = 600;
const unsigned long INTERVALO_LEITURA = 60;

char estado = 'P';
unsigned long ultimaLeitura = 0;

void frente()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void re()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void esquerda()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void direita()
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void parar()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

long medirDistancia()
{
    digitalWrite(TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER, LOW);

    long t = pulseIn(ECHO, HIGH, 30000UL);
    if (t == 0)
        return 999;
    return t / 58;
}

void desviarObstaculo(long d)
{
    parar();
    Serial.print("Obstaculo a ");
    Serial.print(d);
    Serial.println(" cm");
    bluetooth.println("Obstaculo! Parando e dando re...");
    delay(200);

    re();
    delay(TEMPO_RE);
    parar();

    estado = 'P';
    bluetooth.println("Parado. Envie F para seguir.");
}

void processarComando(char c)
{
    c = toupper(c);
    switch (c)
    {
    case 'F':
        estado = 'F';
        frente();
        bluetooth.println("Frente");
        break;
    case 'R':
        estado = 'R';
        re();
        bluetooth.println("Re");
        break;
    case 'E':
        estado = 'E';
        esquerda();
        bluetooth.println("Esquerda");
        break;
    case 'D':
        estado = 'D';
        direita();
        bluetooth.println("Direita");
        break;
    case 'P':
        estado = 'P';
        parar();
        bluetooth.println("Parado");
        break;
    default:
        return;
    }
    Serial.print("Comando: ");
    Serial.println(c);
}

void setup()
{
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(TRIGGER, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(LED_ALERTA, OUTPUT);
    digitalWrite(TRIGGER, LOW);
    digitalWrite(LED_ALERTA, LOW);

    parar();

    Serial.begin(9600);
    bluetooth.begin(9600);

    Serial.println("Carrinho parado. Comandos: F=frente R=re E=esq D=dir P=parar");
    bluetooth.println("Carrinho parado. Comandos: F=frente R=re E=esq D=dir P=parar");
}

void loop()
{
    if (bluetooth.available() > 0)
    {
        processarComando(bluetooth.read());
    }
    if (Serial.available() > 0)
    {
        processarComando(Serial.read());
    }

    if (millis() - ultimaLeitura >= INTERVALO_LEITURA)
    {
        ultimaLeitura = millis();
        long d = medirDistancia();

        if (d < DISTANCIA_LED)
        {
            digitalWrite(LED_ALERTA, HIGH);
        }
        else
        {
            digitalWrite(LED_ALERTA, LOW);
        }

        if (estado == 'F' && d <= DISTANCIA_PARADA)
        {
            desviarObstaculo(d);
        }
    }
}