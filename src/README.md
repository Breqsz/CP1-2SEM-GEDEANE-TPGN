# Software — Carrinho-Robô

Código-fonte: [`codigo.ino`](codigo.ino) (Arduino IDE, placa ESP32).

## Bibliotecas utilizadas

- [`DabbleESP32`](https://thestempedia.com/docs/dabble/game-pad-module/) — permite que o ESP32 se comunique via **Bluetooth** com o
  aplicativo **Dabble** (STEMpedia), usando o módulo **Gamepad** para receber
  os comandos de direção do celular.

## Visão geral do funcionamento

```
setup()
 ├─ configura pinos do L298N (IN1-IN4, ENA, ENB) e do HC-SR04 (TRIG, ECHO)
 └─ inicializa o Bluetooth (Dabble.begin)

loop()  [executa continuamente]
 ├─ Dabble.processInput()      → atualiza o estado do gamepad Bluetooth
 ├─ medirDistancia()            → lê o sensor ultrassônico (cm)
 ├─ se estiver avançando E obstáculo ≤ 20 cm → stopMotors() e aborta o ciclo
 └─ senão, verifica qual botão do Dabble está pressionado:
      ↑ (Up)    → moveForward()   (só se não houver obstáculo próximo)
      ↓ (Down)  → moveBackward()
      ← (Left)  → turnLeft()
      → (Right) → turnRight()
      nenhum    → stopMotors()
```

## Controle dos motores (Ponte H)

Cada motor é acionado por um par de pinos digitais (`IN1`/`IN2` para o motor
esquerdo, `IN3`/`IN4` para o motor direito) ligados ao módulo **L298N**. A
combinação HIGH/LOW desses pinos define o sentido de rotação de cada motor:

| Função | IN1 | IN2 | IN3 | IN4 | Resultado |
| :--- | :---: | :---: | :---: | :---: | :--- |
| `moveForward()` | HIGH | LOW | HIGH | LOW | Ambos os motores giram para frente |
| `moveBackward()` | LOW | HIGH | LOW | HIGH | Ambos os motores giram para trás |
| `turnLeft()` | LOW | HIGH | HIGH | LOW | Motor esquerdo para trás, direito para frente → gira à esquerda |
| `turnRight()` | HIGH | LOW | LOW | HIGH | Motor esquerdo para frente, direito para trás → gira à direita |
| `stopMotors()` | LOW | LOW | LOW | LOW | Ambos os motores param |

Os pinos `ENA`/`ENB` (enable/velocidade) são mantidos sempre em `HIGH` no
`setup()`, ou seja, o carrinho opera sempre em velocidade máxima — não há
controle fino de velocidade (PWM) na versão atual.

## Comunicação sem fio (Bluetooth)

O ESP32 anuncia-se como o dispositivo Bluetooth `"My Bluetooth Car"`
(`Dabble.begin(...)`). O usuário conecta o app **Dabble** a esse dispositivo
e usa o módulo **Gamepad** do app como um controle direcional. A cada
iteração do `loop()`, `Dabble.processInput()` atualiza o estado dos botões, e
as funções `GamePad.isUpPressed()`, `isDownPressed()`, `isLeftPressed()` e
`isRightPressed()` indicam qual comando foi enviado pelo celular.

## Uso do sensor ultrassônico (HC-SR04)

A função `medirDistancia()` implementa a leitura padrão de um sensor
HC-SR04:
1. Garante o pino `TRIG` em `LOW` por 2 µs;
2. Envia um pulso `HIGH` de 10 µs no `TRIG` (dispara o ultrassom);
3. Mede, com `pulseIn`, o tempo (em µs) que o pino `ECHO` fica em `HIGH`
   (tempo de ida e volta do som), com timeout de 30 ms;
4. Converte o tempo em distância: `distancia = duracao * 0.0343 / 2`
   (0,0343 cm/µs é a velocidade do som; divide-se por 2 porque o tempo
   medido é de ida **e volta**);
5. Se não houver eco dentro do timeout (`duracao == 0`), retorna `-1`
   (sem leitura válida / obstáculo fora de alcance).

Essa distância é usada como uma **trava de segurança**: se o carrinho está
avançando (`movendoParaFrente == true`) e a distância lida é maior que 0 e
menor ou igual a `DISTANCIA_SEGURANCA` (20 cm), o programa chama
`stopMotors()` imediatamente e ignora o restante do ciclo — impedindo a
colisão mesmo que o usuário continue segurando o botão "para frente" no app.

## Possíveis evoluções futuras

- Controlar a velocidade dos motores via PWM (`ledcWrite` no ESP32) nos
  pinos `ENA`/`ENB`, em vez de mantê-los sempre em `HIGH`;
- Usar o sensor ultrassônico também para desviar automaticamente do
  obstáculo (modo semiautônomo), e não apenas parar.
