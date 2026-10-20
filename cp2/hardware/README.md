# Hardware — Carrinho-Robô CP2 (ESP32-S3)

No CP2 o carrinho passa a usar a placa **ESP32-S3-DevKitC-1** (módulo
ESP32-S3-WROOM-1, duas portas USB-C), necessária para embarcar o modelo
TinyML treinado no Edge Impulse. Chassi, motores, ponte H, bateria e sensor
são os mesmos do CP1.

## Lista de componentes

| Componente | Modelo | Observação |
| :--- | :--- | :--- |
| Microcontrolador | ESP32-S3-DevKitC-1 | Substitui o microcontrolador do CP1 |
| Ponte H | L298N | Jumpers ENA/ENB mantidos |
| Motores | 2x motor DC com redução | Tração diferencial |
| Sensor | HC-SR04 | Fonte dos dados do dataset TinyML |
| Divisor de tensão | Resistores 1 kΩ + 2 kΩ | Baixa o ECHO de 5 V para 3,3 V |
| Semáforo | LEDs verde, amarelo e vermelho + 3 resistores 220 Ω | Indica a classe do modelo (longe / medio / perto) |
| LED RGB | Integrado à placa (GPIO 48) | Mostra a classe do modelo por cor |
| Capacitor (recomendado) | 470–1000 µF | Evita reset do ESP32 na partida dos motores |
| Bateria | 2x 18650 em série (8,4 V) | Alimenta o L298N |

## Pinagem

Todos os sinais ficam no conector **J1** (lado esquerdo da placa, com as
portas USB para baixo), em sequência: `3V3, 3V3, RST, 4, 5, 6, 7, 15, 16,
17, 18, 8, 3, 46, 9, 10, 11, 12, 13, 14, 5V, GND`.

| Sinal | GPIO | Ligação |
| :--- | :---: | :--- |
| IN1 | 4 | L298N — motor esquerdo |
| IN2 | 5 | L298N — motor esquerdo |
| IN3 | 6 | L298N — motor direito |
| IN4 | 7 | L298N — motor direito |
| TRIG | 15 | HC-SR04 TRIG (direto) |
| ECHO | 16 | HC-SR04 ECHO **via divisor de tensão** |
| LED vermelho | 17 | Resistor 220 Ω → LED → GND (classe `perto`) |
| LED amarelo | 18 | Resistor 220 Ω → LED → GND (classe `medio`) |
| LED verde | 8 | Resistor 220 Ω → LED → GND (classe `longe`) |
| LED RGB | 48 | Integrado (em placas v1.1 é o GPIO 38) |

### Divisor de tensão do ECHO

O HC-SR04 responde com 5 V e os GPIOs do ESP32-S3 suportam no máximo 3,3 V.

```
ECHO ──[1 kΩ]──┬──► GPIO 16
               │
            [2 kΩ]
               │
              GND        5 V × 2/3 ≈ 3,3 V
```

Na montagem foram usados **três resistores de 1 kΩ**: um entre o ECHO e o
ponto do meio, e dois em série (2 kΩ) entre o ponto do meio e o GND.

### Pinos que não devem ser usados

| GPIO | Motivo |
| :--- | :--- |
| 0, 3, 45, 46 | Pinos de boot (strapping) |
| 19, 20 | USB nativo (porta "USB") |
| 26–32 | Memória flash |
| 35, 36, 37 | PSRAM octal (módulos N8R8 / N16R8) |
| 43, 44 | UART0 — Serial da porta "UART" |
| 48 (ou 38) | LED RGB integrado |

## Alimentação

```
Bateria (+) 8,4 V ──► L298N "12V"
Bateria (−) ────────► L298N "GND" ──┬──► ESP32-S3 GND
                                     └──► HC-SR04 GND
L298N "5V" (jumper 5V-EN colocado) ──┬──► ESP32-S3 5V
                                     └──► HC-SR04 VCC
OUT1/OUT2 → motor esquerdo | OUT3/OUT4 → motor direito
```

- O **GND precisa ser comum** entre bateria, L298N, ESP32-S3 e sensor.
- Capacitor de 470–1000 µF entre o pino 5V e o GND da placa (negativo no
  GND) para segurar a queda de tensão quando os motores arrancam.
- Ao gravar pelo USB, **desconectar o fio do pino 5V** para não alimentar a
  placa por duas fontes.
- Motor girando ao contrário: inverter os dois fios dele no OUT do L298N.

## Diagrama de conexões

```
                 ┌───────────────────┐
  Bateria 8,4V ─►│   L298N (Ponte H) │──► Motor esquerdo (OUT1/OUT2)
                 │                   │──► Motor direito  (OUT3/OUT4)
                 └──▲──▲──▲──▲───┬───┘
               IN1 IN2 IN3 IN4   │5V
                 │  │  │  │      ▼
  ┌──────────────┴──┴──┴──┴─────────────┐
  │ ESP32-S3-DevKitC-1                  │
  │  GPIO 4,5,6,7 ──► IN1..IN4          │
  │  GPIO 15 ───────► TRIG ┐            │
  │  GPIO 16 ◄─ divisor ◄─ ECHO HC-SR04 │
  │  GPIO 17 ───────► LED vermelho      │
  │  GPIO 18 ───────► LED amarelo       │
  │  GPIO 8  ───────► LED verde         │
  │  GPIO 48 (RGB integrado)            │
  └─────────────────────────────────────┘
```

## Arduino IDE

1. Instalar a Arduino IDE **no Windows** (o WSL não acessa a porta USB por padrão);
2. Gerenciador de Placas → **esp32 by Espressif Systems**;
3. Placa: **ESP32S3 Dev Module Octal (WROOM2)** — a placa usada tem memória
   flash octal (o boot mostra `Octal Flash Mode Enabled`). Se essa opção não
   existir, usar **ESP32S3 Dev Module** com **Flash Mode: OPI 80MHz** e
   **PSRAM: OPI PSRAM**. A porta aparece como "Silicon Labs CP210x" (driver
   CP210x da Silicon Labs);
4. Cabo na porta **UART** da placa e, em Ferramentas, **USB CDC On Boot: Disabled**;
5. Se a porta COM não aparecer, instalar o driver **CP210x** ou **CH343**
   (conforme o chip ao lado da porta UART);
6. Se o upload travar em `Connecting...`, segurar **BOOT**, apertar e soltar
   **RST**, e soltar **BOOT**;
7. Serial Monitor em **115200**.

## Observação sobre Bluetooth

O ESP32-S3 tem apenas **Bluetooth Low Energy**, sem Bluetooth clássico. Por
isso a biblioteca DabbleESP32 / BluetoothSerial não funciona nesta placa. No
CP2 o carrinho opera de forma **autônoma**, com o comportamento definido
pela classificação do modelo TinyML.

## Teste do hardware

Sketch de teste: [`../testes/teste_hardware/teste_hardware.ino`](../testes/teste_hardware/teste_hardware.ino).
Ordem recomendada: placa sozinha no USB → motores com as rodas suspensas →
sensor (conferir com régua) → LEDs → tudo na bateria.
