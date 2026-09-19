# 🤖 Carrinho-Robô — Check Point 1

**Disciplina:** Project-based Maker Lab — Profª Dra. Gedeane G.S. Kenshima
**Turma:** 2º Semestre — FIAP

## Integrantes

| Nome | RM |
| :--- | :---: |
| Rodrigo Brasileiro | RM98952 |
| Guilherme Rocha | RM97974 |
| Nikolas Rodrigues | RM551566 |
| Thiago Jardim | RM551624 |



## 🎯 Objetivo

Projetar, fabricar e programar um **carrinho-robô** capaz de se locomover de
forma controlada, integrando conhecimentos de projeto mecânico, fabricação
digital, eletrônica e programação embarcada trabalhados ao longo da
disciplina.

O robô utiliza um **ESP32** como microcontrolador central, aciona dois
motores DC por meio de uma **ponte H (L298N)**, é alimentado por bateria e
pode ser controlado remotamente via **Bluetooth** (aplicativo Dabble),
contando ainda com um **sensor ultrassônico** para detecção de obstáculos.

## ✨ Principais funcionalidades

- 🚗 Movimentação para frente, para trás e realização de curvas (esquerda/direita);
- 📱 Controle remoto sem fio via **Bluetooth**, usando o app **Dabble** (módulo Gamepad) no celular;
- 📡 **Sensor ultrassônico (HC-SR04)** integrado ao software, que interrompe automaticamente o avanço do carrinho ao detectar um obstáculo a menos de 20 cm, evitando colisões;
- 🔋 Alimentação autônoma por bateria;
- 🖨️ Chassi próprio, modelado em CAD (Fusion 360) e fabricado pela equipe.

## 🖼️ Carrinho finalizado

![Carrinho](.evidencias/carrinho-finalizado.jpg)

## 📁 Organização do repositório

```
├── README.md                 → este arquivo (visão geral do projeto)
├── cad/                       → arquivos-fonte da modelagem 3D do chassi
├── hardware/                  → documentação de eletrônica, conexões e fotos da montagem
├── src/                        → código-fonte do robô (ESP32) e explicação do funcionamento
├── organizacao/               → planejamento: backlog, MoSCoW, MVP, kanban, dependências, custos, decisões
├── testes/                     → registro de testes realizados, problemas e soluções
└── evidencias/                 → fotos finais, vídeos de funcionamento e instruções de uso
```

| Pasta | Conteúdo |
| :--- | :--- |
| [`cad/`](cad/) | Modelagem 3D do chassi (Fusion 360) |
| [`hardware/`](hardware/README.md) | Lista de componentes, esquema de ligações e fotos da montagem eletrônica |
| [`src/`](src/README.md) | Código-fonte (`codigo.ino`) e explicação do funcionamento do programa |
| [`organizacao/`](organizacao/) | Requisitos, MoSCoW, MVP, backlog, kanban, dependências, planilha de custos e decisões de projeto |
| [`testes/`](testes/TESTES.md) | Testes realizados, problemas encontrados e correções |
| [`evidencias/`](evidencias/EVIDENCIAS.md) | Fotos finais, vídeos de demonstração e instruções de uso |

## 🚀 Como usar

Ver instruções detalhadas em [`evidencias/EVIDENCIAS.md`](evidencias/EVIDENCIAS.md).
