#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <DabbleESP32.h>

#define IN1 16
#define IN2 17
#define IN3 18
#define IN4 19

#define ENA 5
#define ENB 23

#define TRIG_PIN 25
#define ECHO_PIN 26

#define DISTANCIA_SEGURANCA 20

bool movendoParaFrente = false;

void setup()
{
    Serial.begin(115200);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    digitalWrite(ENA, HIGH);
    digitalWrite(ENB, HIGH);

    Dabble.begin("My Bluetooth Car");

    Serial.println("Carrinho Robo v0.2 iniciado!");
}

void loop()
{
    Dabble.processInput();

    float distancia = medirDistancia();

    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    if (movendoParaFrente &&
        distancia > 0 &&
        distancia <= DISTANCIA_SEGURANCA)
    {
        stopMotors();

        Serial.println("OBSTACULO DETECTADO!");

        delay(50);
        return;
    }

    if (GamePad.isUpPressed())
    {
        if (distancia > DISTANCIA_SEGURANCA || distancia <= 0)
        {
            moveForward();
        }
        else
        {
            stopMotors();
        }
    }

    else if (GamePad.isDownPressed())
    {
        moveBackward();
    }

    else if (GamePad.isLeftPressed())
    {
        turnLeft();
    }

    else if (GamePad.isRightPressed())
    {
        turnRight();
    }

    else
    {
        stopMotors();
    }

    delay(30);
}

float medirDistancia()
{
    // Garante pulso LOW antes da medição
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // Envia pulso de 10 microssegundos
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duracao == 0)
    {
        return -1;
    }

    float distancia = duracao * 0.0343 / 2;

    return distancia;
}

void moveForward()
{
    movendoParaFrente = true;

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void moveBackward()
{
    movendoParaFrente = false;

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void turnLeft()
{
    movendoParaFrente = false;

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void turnRight()
{
    movendoParaFrente = false;

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void stopMotors()
{
    movendoParaFrente = false;

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}