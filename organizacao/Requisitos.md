# Requisitos — Carrinho-Robô

**Projeto:** CP1-2SEM-GEDEANE-TPGN
**Equipe:** Rodrigo Brasileiro, Guilherme Rocha, Nikolas Rodrigues, Thiago Jardim

Este documento reúne os requisitos do carrinho-robô, definidos a partir da
proposta da disciplina e refinados ao longo das aulas. A priorização de cada
requisito está em [`MoSCoW.md`](MoSCoW.md) e as mudanças de escopo estão
justificadas em [`Decisoes.md`](Decisoes.md).

## 1. Requisitos funcionais (RF)

| ID | Requisito | Prioridade | Status final | Onde é atendido |
| :--- | :--- | :---: | :---: | :--- |
| RF01 | O carrinho deve se mover para frente | Must | ✅ Atendido | `moveForward()` em [`../src/codigo.ino`](../src/codigo.ino) |
| RF02 | O carrinho deve se mover para trás | Must | ✅ Atendido | `moveBackward()` |
| RF03 | O carrinho deve virar para a esquerda e para a direita | Must | ✅ Atendido | `turnLeft()` / `turnRight()` |
| RF04 | O carrinho deve parar quando nenhum comando for enviado | Must | ✅ Atendido | `stopMotors()` no `else` do `loop()` |
| RF05 | O carrinho deve ser controlado remotamente, sem fio, pelo celular | Must | ✅ Atendido | Bluetooth + app Dabble (Gamepad) |
| RF06 | O carrinho deve medir a distância até obstáculos à frente | Should | ✅ Atendido | `medirDistancia()` com HC-SR04 |
| RF07 | O carrinho deve bloquear o avanço ao detectar obstáculo a ≤ 20 cm | Should | ✅ Atendido | Trava `DISTANCIA_SEGURANCA` no `loop()` |
| RF08 | Controle de velocidade dos motores (PWM) | Could | ❌ Não implementado | Melhoria futura |
| RF09 | LEDs indicadores de status | Could | ❌ Não implementado | Melhoria futura |
| RF10 | Desvio automático de obstáculos (modo autônomo) | Won't | ❌ Fora do escopo | — |

## 2. Requisitos não funcionais (RNF)

| ID | Requisito | Status final |
| :--- | :--- | :---: |
| RNF01 | Alimentação autônoma por bateria, sem cabo durante o uso | ✅ 2x 18650 em série com chave ON/OFF |
| RNF02 | Todos os componentes devem caber e ficar fixos no chassi (dimensões em [`../hardware/README.md`](../hardware/README.md)) | `[CONFIRMAR]` |
| RNF03 | O carrinho deve permanecer estável em movimento e em curvas (roda boba de apoio) | `[CONFIRMAR após teste]` |
| RNF04 | O comando do app deve refletir no carrinho sem atraso perceptível | `[CONFIRMAR após teste]` |
| RNF05 | O carrinho deve ter carenagem que proteja a eletrônica | `[CONFIRMAR]` |
| RNF06 | Custo total compatível com um projeto acadêmico (≈ R$ 235, ver [`Planilha_Custos_Carrinho.xlsx`](Planilha_Custos_Carrinho.xlsx)) | ✅ |
| RNF07 | Código-fonte organizado, comentado e versionado no GitHub | ✅ |

## 3. Restrições do projeto

- Microcontrolador da família Arduino/ESP32, programado na Arduino IDE;
- Tração por 2 motores DC com caixa de redução, acionados por ponte H;
- Pelo menos um sensor integrado com função real no comportamento do robô;
- Comunicação sem fio para controle remoto;
- Prazo: entrega no Check Point 1 do 2º semestre.

## 4. Critérios de aceitação

O projeto é considerado concluído quando:

1. Todos os requisitos **Must** (RF01–RF05, RNF01) estiverem atendidos;
2. O sensor ultrassônico interromper o avanço antes de uma colisão
   (RF06–RF07), comprovado em vídeo;
3. Os testes registrados em [`../testes/TESTES.md`](../testes/TESTES.md)
   tiverem resultado positivo na integração final.
