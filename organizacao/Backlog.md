# Backlog — Carrinho-Robô

**Projeto:** CP1-2SEM-GEDEANE-TPGN
**Equipe:** Rodrigo Brasileiro, Guilherme Rocha, Nikolas Rodrigues, Thiago Jardim

Lista de tarefas necessárias para sair do protótipo até o produto final,
com responsável definido para cada uma.

## Registro inicial (01/09/2026)

> Estado do backlog no momento em que foi criado. Mantido sem alterações
> para registrar a evolução; o estado final está na seção seguinte.

| # | Tarefa | Categoria (MoSCoW) | Responsável | Status |
|---|---|---|---|---|
| 1 | Medir componentes (motores, ESP32, Ponte H, bateria, sensor) | Must | Rodrigo Brasileiro | Concluído |
| 2 | Desenho 3D do chassi (`cad/Carro.f3z`) | Must | Guilherme Rocha | Concluído |
| 3 | Teste de encaixes das peças no chassi | Must | Nikolas Rodrigues | Em andamento |
| 4 | Corte/impressão do chassi | Must | Thiago Jardim | Em andamento |
| 5 | Montagem mecânica (motores + rodas + chassi) | Must | Rodrigo Brasileiro | A fazer |
| 6 | Montagem eletrônica (ESP32 + Ponte H + motores + bateria) | Must | Nikolas Rodrigues | A fazer |
| 7 | Teste dos motores (girando nos dois sentidos) | Must | Thiago Jardim | A fazer |
| 8 | Software básico: avançar, recuar, virar | Must | Guilherme Rocha | A fazer |
| 9 | Integração final do MVP e testes de bancada | Must | Equipe toda | A fazer |
| 10 | Sensor ultrassônico — leitura de distância | Should | Nikolas Rodrigues | Backlog |
| 11 | Parada automática ao detectar obstáculo | Should | Guilherme Rocha | Backlog |
| 12 | Controle via Wi-Fi (app/página simples) | Should | Rodrigo Brasileiro | Backlog |
| 13 | Carenagem final (papelão, MDF ou 3D) | Could | Thiago Jardim | Backlog |
| 14 | LEDs indicadores de status | Could | Guilherme Rocha | Backlog |
| 15 | Roda boba para estabilidade | Could | Nikolas Rodrigues | Backlog |
| 16 | Documentação final do projeto (README) | Must | Thiago Jardim | Backlog |


## Estado final (entrega)

| # | Tarefa | Responsável | Status final | Observação |
|---|---|---|---|---|
| 1 | Medir componentes | Rodrigo Brasileiro | ✅ Concluído | Tabela em [`../hardware/README.md`](../hardware/README.md) |
| 2 | Desenho 3D do chassi | Guilherme Rocha | ✅ Concluído | [`../cad/`](../cad/README.md) |
| 3 | Teste de encaixes | Nikolas Rodrigues | ✅ Concluído | `[CONFIRMAR]` |
| 4 | Corte/impressão do chassi | Thiago Jardim | ✅ Concluído | `[CONFIRMAR: chassi próprio ou kit comercial]` |
| 5 | Montagem mecânica | Rodrigo Brasileiro | ✅ Concluído | |
| 6 | Montagem eletrônica | Nikolas Rodrigues | ✅ Concluído | |
| 7 | Teste dos motores | Thiago Jardim | ✅ Concluído | Bancada da Aula 16 |
| 8 | Software básico: avançar, recuar, virar | Guilherme Rocha | ✅ Concluído | Firmware v1 → v2 |
| 9 | Integração final do MVP | Equipe toda | ✅ Concluído | |
| 10 | Sensor ultrassônico — leitura de distância | Nikolas Rodrigues | ✅ Concluído | `medirDistancia()` |
| 11 | Parada automática ao detectar obstáculo | Guilherme Rocha | ✅ Concluído | Trava de 20 cm |
| 12 | ~~Controle via Wi-Fi~~ → Controle via Bluetooth (Dabble) | Rodrigo Brasileiro | ✅ Concluído (substituído) | Ver Decisão 2 em [`Decisoes.md`](Decisoes.md) |
| 13 | Carenagem final | Thiago Jardim | `[CONFIRMAR]` | |
| 14 | LEDs indicadores de status | Guilherme Rocha | ❌ Não realizado | Movido para melhorias futuras |
| 15 | Roda boba para estabilidade | Nikolas Rodrigues | ✅ Concluído | |
| 16 | Documentação final (README) | Thiago Jardim | ✅ Concluído | |

> Itens **Won't Have** (RFID, OLED, GPS, câmera, buzzer, modo autônomo) não
> entraram no backlog de execução. Ver [`MoSCoW.md`](MoSCoW.md).
