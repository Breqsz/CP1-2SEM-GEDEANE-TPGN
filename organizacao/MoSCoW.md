# MoSCoW — Priorização de Funcionalidades

Este documento tem duas versões: a **priorização inicial**, feita no início
do planejamento, e a **priorização revisada**, que reflete o que foi
efetivamente entregue. As duas foram mantidas para registrar a evolução do
projeto. A justificativa de cada mudança está em [`Decisoes.md`](Decisoes.md).

---

## Versão 1 — Priorização inicial (planejamento)

### Must Have — o carrinho-robô DEVE ter obrigatoriamente

- Chassi montado com 2 motores e rodas fixadas
- ESP32/Arduino controlando a Ponte H
- Ponte H acionando os motores corretamente
- Alimentação estável via bateria
- Movimento básico: avançar, recuar e virar

### Should Have — o carrinho-robô PODE ter

- Controle remoto via Wi-Fi
- Sensor ultrassônico para detecção de obstáculos
- Parada automática ao detectar obstáculo próximo

### Could Have — seria INTERESSANTE ter

- Carenagem final impressa em 3D
- LEDs indicadores de status
- Roda boba para maior estabilidade

### Won't Have (por enquanto) — o que NÃO TERÁ nesta versão

- Bluetooth
- RFID
- Display OLED
- GPS
- Câmera
- Buzzer
- Modo autônomo completo de navegação

---

## Versão 2 — Priorização revisada (entrega final)

| Item | Prioridade inicial | Prioridade final | Situação | Motivo da mudança |
| :--- | :---: | :---: | :---: | :--- |
| Chassi + 2 motores + rodas | Must | Must | ✅ Entregue | — |
| ESP32 controlando a ponte H | Must | Must | ✅ Entregue | — |
| Ponte H L298N | Must | Must | ✅ Entregue | — |
| Alimentação por bateria | Must | Must | ✅ Entregue | — |
| Avançar, recuar e virar | Must | Must | ✅ Entregue | — |
| Controle remoto sem fio | Should (Wi-Fi) | **Must (Bluetooth)** | ✅ Entregue | Controle remoto passou a ser requisito obrigatório; Bluetooth foi mais simples que Wi-Fi (Decisão 2) |
| Controle via Wi-Fi | Should | **Won't** | ❌ Substituído | Trocado por Bluetooth (Decisão 2) |
| Bluetooth | Won't | **Must** | ✅ Entregue | Ver Decisão 2 |
| Sensor ultrassônico | Should | Should | ✅ Entregue | — |
| Parada automática por obstáculo | Should | Should | ✅ Entregue | — |
| Roda boba (caster) | Could | Could | ✅ Entregue | Necessária para apoiar o chassi de 2 rodas |
| Carenagem final | Could | Could | `[CONFIRMAR]` | `[PREENCHER material usado]` |
| LEDs indicadores | Could | Could | ❌ Não entregue | Priorizado o sensor |
| Controle de velocidade (PWM) | — | **Could** (novo) | ❌ Não entregue | Identificado durante o desenvolvimento (Decisão 4) |
| RFID, OLED, GPS, câmera, buzzer, modo autônomo | Won't | Won't | ❌ Fora do escopo | — |
