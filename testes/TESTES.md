# Testes e Resultados — Carrinho-Robô

Este documento registra os testes realizados durante o desenvolvimento, os
problemas encontrados e as correções aplicadas. Preencher/atualizar conforme
os testes forem executados (bancada, chassi, integração final).

## Como preencher esta tabela

Para cada teste: data, o que foi testado, resultado esperado, resultado
obtido e, se houve problema, qual foi a causa e a correção aplicada.

| Data | Teste | Resultado esperado | Resultado obtido | Problema encontrado | Correção aplicada |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `[PREENCHER]` | Teste de bancada dos motores com L298N (Aula 16) — ver [`../hardware/foto-montagem-circuito-motores.jpg`](../hardware/foto-montagem-circuito-motores.jpg) | Motores giram em ambos os sentidos ao inverter IN1-IN4 | `[PREENCHER]` | `[PREENCHER]` | `[PREENCHER]` |
| `[PREENCHER]` | Controle via Bluetooth (v1 — callbacks `onPressed`) | Botões do Dabble acionam frente/trás/curvas | `[PREENCHER]` | Motor não permanecia ligado com o botão pressionado continuamente | Reescrita do loop para modelo de polling (`isXPressed()`) na v2 — ver [`../organizacao/Decisoes.md`](../organizacao/Decisoes.md) |
| `[PREENCHER]` | Leitura do sensor ultrassônico HC-SR04 isolado | Retorna distância coerente em cm | `[PREENCHER]` | `[PREENCHER]` | `[PREENCHER]` |
| `[PREENCHER]` | Parada automática por obstáculo (< 20 cm) durante avanço | Carrinho para antes de colidir | `[PREENCHER]` | `[PREENCHER]` | `[PREENCHER]` |
| `[PREENCHER]` | Encaixe das peças no chassi impresso | Todos os componentes encaixam sem folga excessiva | `[PREENCHER]` | `[PREENCHER]` | `[PREENCHER]` |
| `[PREENCHER]` | Autonomia da bateria em uso contínuo | `[PREENCHER]` | `[PREENCHER]` | `[PREENCHER]` | `[PREENCHER]` |
| `[PREENCHER]` | Estabilidade do carrinho em movimento (com carenagem) | Não tomba nem derrapa em curvas | `[PREENCHER]` | `[PREENCHER]` | `[PREENCHER]` |
| `[PREENCHER]` | Integração final (chassi + eletrônica + software + carenagem) | Robô controlável e funcional de ponta a ponta | `[PREENCHER]` | `[PREENCHER]` | `[PREENCHER]` |

## Resultado final

`[PREENCHER]`: descrever, em um parágrafo, o estado final do carrinho após
todos os testes (o que funciona plenamente, o que tem limitações conhecidas
e o que ficou de fora do escopo desta entrega — ver
[`../organizacao/MoSCoW.md`](../organizacao/MoSCoW.md), seção *Won't Have*).
