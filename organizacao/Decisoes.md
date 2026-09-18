# Decisões de Projeto e Evolução — Carrinho-Robô

Registro das principais decisões tomadas pela equipe ao longo do
desenvolvimento e de como o projeto evoluiu entre versões.

## 1. Microcontrolador: de Arduino Uno para ESP32

A modelagem 3D inicial do chassi (`cad/Carro.f3z`, ver
[`../cad/README.md`](../cad/README.md)) foi feita considerando um
**Arduino Uno**, conforme aparece na lista de peças dos desenhos técnicos
(sheet 1/6).

**Decisão:** trocar para **ESP32** no firmware final.
**Motivo:** o Arduino Uno não possui rádio Bluetooth/Wi-Fi integrado, o que
exigiria um módulo externo (ex.: HC-05) para atender ao requisito de
**controle remoto sem fio**. O ESP32 já traz Bluetooth nativo, permitindo
usar a biblioteca `DabbleESP32` sem hardware adicional, e mantém dimensões
compatíveis com o espaço já reservado no chassi para a placa controladora.

## 2. Evolução do firmware — v1 → v2

**Versão 1** (commit `51c554c`): controle via Bluetooth (app Dabble) usando
callbacks de evento (`GamePad.onPressed(...)`), sem leitura de sensores.
Apenas movimento (frente/trás/curvas) e parada manual.

**Versão 2** (commit `6ed4cc6`, "Codigo v2"): reescrita do loop principal
para o modelo de *polling* (`isUpPressed()`, `isDownPressed()`, etc., a cada
iteração), adição do **sensor ultrassônico HC-SR04** (pinos `TRIG`/`ECHO`) e
da trava de segurança que interrompe o avanço automaticamente ao detectar um
obstáculo a até 20 cm (`DISTANCIA_SEGURANCA`). Essa mudança foi necessária
para atender ao critério de **sensor integrado com função real no
projeto** — o sensor não apenas está instalado, mas efetivamente altera o
comportamento do robô (evita colisões mesmo com o comando de avanço ativo).

**Motivo da troca de `onPressed`/callback para polling:** o modelo de
callback do Dabble dispara a ação apenas na transição do botão (borda de
subida), o que dificultava manter o motor ligado continuamente enquanto o
botão permanece pressionado. O modelo de polling (`isXPressed()` a cada
ciclo do `loop()`) resolveu esse problema e também permitiu inserir a
verificação do sensor de obstáculo antes de decidir se o carrinho pode
avançar.

## 3. Velocidade fixa (sem PWM)

**Decisão:** manter `ENA`/`ENB` sempre em `HIGH` (velocidade máxima fixa),
em vez de usar PWM para variar a velocidade.
**Motivo:** simplicidade e tempo — o MVP definido em
[`MVP.md`](MVP.md) exigia apenas "avançar, recuar e virar" funcionando de
forma estável. Controle de velocidade por PWM foi definido como item
**Could Have** no [`MoSCoW.md`](MoSCoW.md) e ficou registrado como melhoria
futura em [`../src/README.md`](../src/README.md).

## 4. Priorização Must/Should/Could (MoSCoW)

Ver [`MoSCoW.md`](MoSCoW.md) para a lista completa. Em resumo: chassi +
motores + ponte H + bateria + movimento básico foram tratados como
inegociáveis (Must); Bluetooth e sensor ultrassônico como prioridade alta
(Should) — e acabaram entregues; carenagem final, LEDs e roda boba como
melhorias desejáveis (Could).

## 5. Itens não implementados nesta versão (Won't Have)

RFID, display OLED, GPS, câmera, buzzer e modo autônomo completo de
navegação foram conscientemente deixados de fora do escopo desta entrega
(ver [`MoSCoW.md`](MoSCoW.md)), para manter o foco nos requisitos
obrigatórios do Check Point dentro do tempo disponível.

## Próximas decisões pendentes

- `[PREENCHER]`: registrar aqui decisões tomadas durante a fabricação do
  chassi (ex.: ajustes de tolerância, escolha do material de impressão) e
  durante a montagem final (ex.: fixação da bateria, solução de carenagem
  adotada). Ver também [`../testes/TESTES.md`](../testes/TESTES.md).
