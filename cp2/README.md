# 🤖 Carrinho-Robô — Check Point 2: TinyML

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

Evoluir o carrinho-robô do CP1 com uma aplicação de **TinyML**: usar o
sensor ultrassônico do próprio carrinho para coletar dados reais, treinar um
modelo de classificação no **Edge Impulse**, embarcar o modelo no **ESP32-S3**
e fazer com que o resultado da classificação **controle o comportamento do
carrinho**.

```
COLETAR → TREINAR → TESTAR → EMBARCAR → INFERIR → AGIR

HC-SR04 → coleta (ESP32) → CSV → Edge Impulse → treinamento → teste
        → Arduino library → ESP32-S3 → run_classifier() → motores / semáforo de LEDs
```

## 🔄 Evolução CP1 → CP2

| | CP1 | CP2 |
| :--- | :--- | :--- |
| Microcontrolador | Arduino Uno ([`../src/codigo.ino`](../src/codigo.ino)) | ESP32-S3-DevKitC-1 |
| Controle | Remoto, via Bluetooth | **Autônomo**, decidido pelo modelo TinyML |
| Papel do sensor | Trava de segurança (`if distância < 20 cm`) | Entrada do modelo de classificação |
| Decisão | Regra fixa no código | Classe produzida pelo modelo (`longe` / `medio` / `perto`) |

O código e a documentação do CP1 foram mantidos sem alterações nas pastas
originais do repositório. Tudo do CP2 está nesta pasta `cp2/`.

## 📁 Organização da pasta `cp2/`

| Pasta / arquivo | Conteúdo |
| :--- | :--- |
| [`hardware/`](hardware/README.md) | Pinagem do ESP32-S3, ligações, divisor de tensão e alimentação |
| [`testes/`](testes/TESTES.md) | Sketch de teste do hardware e registro dos testes |
| [`coleta/`](coleta/) | Sketch de coleta (`coleta_dataset.ino`) e script que grava o CSV (`coletar.py`) |
| [`dados/`](dados/) | CSVs coletados pelo grupo (`coleta_*.csv` e `dataset.csv`) |
| [`edge-impulse/`](edge-impulse/README.md) | Processo no Edge Impulse, prints e biblioteca exportada |
| [`src/tinyml_carrinho/`](src/tinyml_carrinho/tinyml_carrinho.ino) | **Código final do carrinho** com o modelo embarcado |
| [`evidencias/`](evidencias/) | Vídeos da demonstração do carrinho com TinyML |

---

## 1. Dataset e coleta de dados

### Sensor utilizado

**HC-SR04 (ultrassônico)**, o mesmo sensor do CP1, montado na frente do
carrinho e lido pelo ESP32-S3 (TRIG no GPIO 15, ECHO no GPIO 16 via divisor
de tensão — ver [`hardware/`](hardware/README.md)).

### Dados coletados

Cada amostra é a **distância até o obstáculo, em cm**, já tratada pelo
ESP32 antes de ser gravada:

1. leitura do HC-SR04 (`tempo / 58`);
2. **limitação de faixa**: leituras abaixo de 2 cm, acima de 200 cm ou sem eco
   viram 2 / 200 cm;
3. **média móvel** das últimas 5 leituras, para reduzir o ruído do sensor.

Uma amostra é gerada a cada 100 ms.

### Classes definidas

| Classe | Situação | Faixa na coleta |
| :--- | :--- | :--- |
| `perto` | obstáculo muito próximo | menos de 20 cm |
| `medio` | obstáculo se aproximando | entre 20 e 50 cm |
| `longe` | caminho livre | mais de 50 cm, incluindo sem obstáculo |

### Como a coleta foi realizada

1. O sketch [`coleta/coleta_dataset/coleta_dataset.ino`](coleta/coleta_dataset/coleta_dataset.ino)
   foi gravado no ESP32-S3 do carrinho. Ele imprime no Serial linhas no
   formato `distancia,label` e recebe pelo Serial qual classe está sendo
   gravada (`1` = perto, `2` = medio, `3` = longe, `x` = pausa).
2. O script [`coleta/coletar.py`](coleta/coletar.py), rodando no computador
   conectado ao ESP32, envia o comando da classe, grava as linhas recebidas
   em `dados/coleta_<classe>.csv` e para ao atingir a quantidade pedida.
3. Com o carrinho parado no chão, um obstáculo (caixa/caderno com a face reta
   voltada para o sensor) foi movido lentamente dentro da faixa de cada
   classe, usando uma folha A4 no chão como referência de distância.
4. Para a classe `longe` também foram coletadas leituras com o carrinho
   apontado para um espaço aberto, sem obstáculo.
5. Leituras fora da faixa da classe (momentos em que o obstáculo saiu da
   posição e o sensor enxergou o fundo) foram descartadas, para que nenhuma
   amostra ficasse com o rótulo errado.
6. Por fim, `python coletar.py juntar` uniu os três arquivos em
   `dados/dataset.csv`.

### Arquivos CSV

| Arquivo | Conteúdo |
| :--- | :--- |
| [`dados/coleta_perto.csv`](dados/coleta_perto.csv) | Sessão de coleta da classe `perto` |
| [`dados/coleta_medio.csv`](dados/coleta_medio.csv) | Sessão de coleta da classe `medio` |
| [`dados/coleta_longe.csv`](dados/coleta_longe.csv) | Duas sessões da classe `longe` (obstáculo e espaço aberto) |
| [`dados/dataset.csv`](dados/dataset.csv) | Arquivo final enviado ao Edge Impulse |

### Quantidade de amostras

| Classe | Amostras no CSV | Faixa medida |
| :--- | :---: | :--- |
| perto | 158 | 4,5 – 19,8 cm |
| medio | 160 | 25,4 – 49,9 cm |
| longe | 220 | 57,2 – 200 cm |
| **Total** | **538** | |

Ao importar, o Edge Impulse descarta amostras idênticas (valores repetidos
quando o obstáculo ficou parado). Ficaram **363 amostras únicas**: 291 para
treino e 72 para teste.

---

## 2. Edge Impulse

Projeto público: **https://studio.edgeimpulse.com/public/1142742/live**

Resumo (o passo a passo completo, com todos os prints, está em
[`edge-impulse/README.md`](edge-impulse/README.md)):

| Etapa | Configuração |
| :--- | :--- |
| Importação | Upload do `dataset.csv` pelo CSV Wizard (não é série temporal; label = coluna `label`; valor = coluna `distancia`) |
| Divisão dos dados | Automática, 80% treino (291) / 20% teste (72) |
| Impulse | Input: `distancia` → Processing: **Raw data** → Learning: **Classification** → 3 classes |
| Processamento | Raw data (a distância em cm entra direto no modelo) |
| Modelo | Rede neural densa: entrada (1) → Dense 20 → Dense 10 → saída (3), learning rate 0,005 |
| Treino 1 | 20 ciclos: **86,4%** de acurácia — 38,1% de `medio` confundido com `longe` |
| Treino 2 | 100 ciclos: **100%** de acurácia, loss 0,02 |
| Teste (dados não vistos) | **100%** em float32 e em **int8 (quantizado)** |
| Deployment | **Arduino library**, Quantized (int8), EON Compiler — 1 ms, 1,4 KB de RAM, 14,3 KB de flash |

---

## 3. Integração com o ESP32

### Biblioteca do Edge Impulse

A biblioteca exportada está em
[`edge-impulse/biblioteca/ei-tinyml-carrinho-tpgn-arduino-1.0.1.zip`](edge-impulse/biblioteca/ei-tinyml-carrinho-tpgn-arduino-1.0.1.zip).
Para compilar: **Sketch → Incluir biblioteca → Adicionar biblioteca .ZIP**.

### Código final

[`src/tinyml_carrinho/tinyml_carrinho.ino`](src/tinyml_carrinho/tinyml_carrinho.ino)

A cada 100 ms o programa:

1. lê o HC-SR04 e aplica **o mesmo tratamento da coleta** (limitação de faixa
   e média de 5 leituras);
2. coloca a distância no buffer de entrada e executa o modelo com
   `run_classifier()` — **a inferência roda no ESP32-S3**;
3. escolhe a classe de maior probabilidade (aceita se a confiança for de
   pelo menos 60%);
4. **age de acordo com a classe**:

| Classe do modelo | Motores | Semáforo de LEDs | LED RGB da placa |
| :--- | :--- | :--- | :--- |
| `longe` | frente, velocidade normal | 🟢 verde | verde |
| `medio` | frente, **devagar** | 🟡 amarelo | amarelo |
| `perto` | **para, dá ré e gira à direita** | 🔴 vermelho | vermelho |

O movimento é decidido **somente pela classe produzida pelo modelo** — não
há regra de distância no código de controle.

5. imprime no Serial a entrada, a saída do modelo e a ação:

```
dist=42.0cm | longe=0.01 medio=0.97 perto=0.02 | classe=medio (0.97) | tempo=0ms | acao=FRENTE DEVAGAR
```

### Como usar

1. Ligar as baterias: o modelo começa a classificar imediatamente (semáforo
   aceso) e o LED RGB da placa pisca em azul durante **10 segundos**.
2. Após os 10 segundos os motores ligam sozinhos e o carrinho passa a andar
   conforme a classe do modelo.
3. O botão **BOOT** da placa (ou `G` pelo Serial) liga/desliga os motores a
   qualquer momento — apertado durante a contagem, cancela a partida.
4. Com os motores desligados o modelo continua rodando, o que permite
   observar as classes nos LEDs e no Serial Monitor (115200).

Configuração da Arduino IDE: placa **ESP32S3 Dev Module Octal (WROOM2)**,
**USB CDC On Boot: Disabled**, cabo na porta **UART**.

---

## 4. Testes e demonstração

Registro dos testes: [`testes/TESTES.md`](testes/TESTES.md).

Vídeos da demonstração (clique para assistir no GitHub):

- [▶️ Demonstração 1](evidencias/cp2-demonstracao-1.mp4) — 23 s
- [▶️ Demonstração 2](evidencias/cp2-demonstracao-2.mp4) — 1 min 9 s
