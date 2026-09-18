# Decisões de Projeto e Evolução — Carrinho-Robô

Registro das principais decisões tomadas pela equipe ao longo do
desenvolvimento e de como o projeto evoluiu entre versões.

## Linha do tempo

| Data | Marco | Referência |
| :--- | :--- | :--- |
| `[CONFIRMAR]` (arquivos datados de 15/04/2026) | Modelagem 3D e desenhos técnicos do chassi (Fusion 360) | [`../cad/`](../cad/README.md) |
| 14/08/2026 | Tabela dimensional dos componentes (primeiro README) | commit `3e8c0da` |
| Aula 16 | Montagem de bancada: L298N + 2 motores | [`../hardware/foto-montagem-circuito-motores.jpg`](../hardware/foto-montagem-circuito-motores.jpg) |
| 21/08/2026 | Firmware v1 (Bluetooth, sem sensor) | commit `51c554c` / [`../src/versoes/codigo_v1.ino`](../src/versoes/codigo_v1.ino) |
| 28/08/2026 | Firmware v2 (polling + sensor HC-SR04) e planilha de custos | commits `6ed4cc6`, `269bbc0` |
| 01/09/2026 | Backlog, dependências, Kanban e MVP | commit `cc4e512` |
| `[PREENCHER]` | Impressão/fabricação do chassi | [`../cad/README.md`](../cad/README.md) |
| `[PREENCHER]` | Montagem final e carenagem | [`../evidencias/`](../evidencias/EVIDENCIAS.md) |

## 1. Microcontrolador: de Arduino Uno para ESP32

A modelagem 3D inicial do chassi (`cad/Carro.f3z`) foi feita considerando um
**Arduino Uno**, conforme aparece na lista de peças dos desenhos técnicos
(sheet 1/6).

**Decisão:** usar **ESP32** no carrinho final.
**Motivo:** o Arduino Uno não possui rádio Bluetooth/Wi-Fi integrado, o que
exigiria um módulo externo (ex.: HC-05) para o controle remoto sem fio. O
ESP32 já traz Bluetooth e Wi-Fi nativos, dispensando esse módulo (a planilha
de custos registra o HC-05/HM-10 como "não aplicável"), e tem dimensões
compatíveis com o espaço reservado no chassi (6,91 × 5,08 cm).

## 2. Comunicação sem fio: de Wi-Fi para Bluetooth

No planejamento inicial ([`MoSCoW.md`](MoSCoW.md), versão 1), o controle
remoto previsto era por **Wi-Fi** (Should Have) e o **Bluetooth** estava
listado como Won't Have.

**Decisão:** implementar o controle remoto por **Bluetooth**, usando o app
**Dabble** (módulo Gamepad) e a biblioteca `DabbleESP32`.
**Motivos:**
- O Dabble já oferece uma interface de gamepad pronta no celular, sem
  precisar desenvolver página web ou app próprio;
- O Bluetooth conecta direto celular ↔ ESP32, sem depender de roteador ou
  da rede Wi-Fi do laboratório;
- Com o controle remoto tratado como requisito obrigatório, a solução mais
  simples e confiável dentro do prazo foi priorizada.

A priorização foi atualizada na versão 2 do [`MoSCoW.md`](MoSCoW.md).

## 3. Evolução do firmware — v1 → v2

**Versão 1** (commit `51c554c`, preservada em
[`../src/versoes/codigo_v1.ino`](../src/versoes/codigo_v1.ino)): controle via
Bluetooth usando callbacks `GamePad.onPressed(...)`, sem sensores.

**Versão 2** (commit `6ed4cc6`, arquivo atual
[`../src/codigo.ino`](../src/codigo.ino)). Alterações:

| Alteração | Motivo |
| :--- | :--- |
| `ENB` movido do **GPIO 6** para o **GPIO 23** | No ESP32, os GPIO 6–11 são ligados à memória flash interna e não podem ser usados como saída (causam travamento/reset) |
| `ENA`/`ENB` colocados em `HIGH` no `setup()` | No v1 os pinos eram configurados mas nunca ativados, então a ponte H não habilitava os motores |
| Inclusão de `#define CUSTOM_SETTINGS` e `INCLUDE_GAMEPAD_MODULE` | Configuração recomendada da biblioteca Dabble para carregar só o módulo Gamepad |
| Troca de callbacks (`onPressed`) por *polling* (`isUpPressed()` etc.) | O callback reage à transição do botão, dificultando manter o motor ligado enquanto o botão está pressionado; o polling também permitiu checar o sensor antes de avançar |
| Sensor **HC-SR04** (GPIO 25/26) e trava de 20 cm | Atender ao critério de sensor com função real: o sensor altera o comportamento do robô, bloqueando o avanço mesmo com o comando ativo |
| Mensagens no monitor serial | Depuração das distâncias lidas durante os testes |

## 4. Velocidade fixa (sem PWM)

**Decisão:** manter `ENA`/`ENB` sempre em `HIGH` (velocidade máxima fixa).
**Motivo:** simplicidade e prazo. O MVP ([`MVP.md`](MVP.md)) exigia apenas
avançar, recuar e virar de forma estável. O controle de velocidade por PWM
foi identificado durante o desenvolvimento e adicionado como **Could Have**
na versão 2 do [`MoSCoW.md`](MoSCoW.md), ficando como melhoria futura.

## 5. Priorização (MoSCoW)

Na priorização inicial, chassi, motores, ponte H, bateria e movimento
básico eram Must; Wi-Fi e sensor ultrassônico eram Should; carenagem, LEDs e
roda boba eram Could. Na entrega final, o controle remoto (por Bluetooth)
passou a ser tratado como obrigatório, o sensor e a parada automática foram
entregues e a roda boba foi incorporada. Comparação completa em
[`MoSCoW.md`](MoSCoW.md).

## 6. Itens não implementados nesta versão (Won't Have)

RFID, display OLED, GPS, câmera, buzzer, Wi-Fi e modo autônomo completo foram
conscientemente deixados de fora do escopo, para manter o foco nos requisitos
obrigatórios do Check Point dentro do tempo disponível.

## 7. Chassi `[PREENCHER]`

> Registrar aqui qual chassi foi usado na versão final. A planilha de custos
> prevê um **kit chassi 2WD comercial**, e a pasta `cad/` tem um **chassi
> próprio** modelado no Fusion 360. Escolher o caso que se aplica:
>
> - **Chassi próprio impresso:** material, impressora, tempo de impressão,
>   ajustes de tolerância e o que mudou entre versões.
> - **Kit comercial:** por que foi escolhido (ex.: prazo, rigidez) e quais
>   adaptações foram feitas (furos novos, suportes impressos, posição do
>   sensor, fixação da bateria etc.). O modelo em `cad/` passa a ser
>   documentado como estudo de layout/proposta inicial.

## 8. Carenagem `[PREENCHER]`

> Material escolhido (a planilha prevê papelão/papel cartão reaproveitado),
> como foi fixada, abertura para o sensor, acesso à chave da bateria e
> eventuais versões/ajustes.
