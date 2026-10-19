# Testes — CP2

## 1. Comunicação com o ESP32-S3

| Teste | Resultado |
| :--- | :--- |
| Placa reconhecida pelo computador | ✅ após instalar o driver CP210x (porta COM3) |
| Gravação do sketch | ✅ placa configurada como ESP32S3 Dev Module Octal (WROOM2), pois a memória flash é octal |

## 2. Leitura do sensor

Sketch: [`teste_hardware/teste_hardware.ino`](teste_hardware/teste_hardware.ino)
e [`../coleta/coleta_dataset/coleta_dataset.ino`](../coleta/coleta_dataset/coleta_dataset.ino).

| Teste | Resultado |
| :--- | :--- |
| Distância muda conforme o obstáculo se move | ✅ leituras entre ~4 cm e ~160 cm acompanhando o movimento |
| Sem obstáculo | ✅ leitura limitada a 200 cm |

## 3. Coleta de dados

| Problema encontrado | Causa | Solução |
| :--- | :--- | :--- |
| Muitas leituras de ~85–120 cm durante a coleta de `perto` | O obstáculo saía da frente do sensor no vai e vem e o sensor media a parede do fundo | Uso de obstáculo largo e reto, folha A4 como referência de distância e descarte automático, no `coletar.py`, das leituras fora da faixa da classe |

## 4. Modelo no Edge Impulse

| Teste | Resultado |
| :--- | :--- |
| Treino com 20 ciclos | 86,4% — `medio` confundido com `longe` |
| Treino com 100 ciclos | ✅ 100% na validação |
| Model testing (int8) | ✅ 100% nas 72 amostras de teste |

## 5. Inferência no ESP32

Sketch: [`../src/tinyml_carrinho/tinyml_carrinho.ino`](../src/tinyml_carrinho/tinyml_carrinho.ino).

| Teste | Resultado |
| :--- | :--- |
| Modelo compila e roda no ESP32-S3 | ✅ 10% da flash, 7% da RAM |
| Saída no Serial com distância, probabilidades e classe | ✅ ex.: `dist=123.2cm \| longe=1.00 medio=0.00 perto=0.00 \| classe=longe (1.00)` |
| Tempo de inferência | ✅ menos de 1 ms |

## 6. Ação no carrinho

| Teste | Resultado |
| :--- | :--- |
| Rodas suspensas: longe → normal, medio → devagar, perto → para, ré e gira | ✅ |
| Velocidade alta demais no primeiro teste | Ajustada: normal 200 → 160, devagar 140 → 125, manobra 190 → 170; ré 600 ms e giro 550 ms |
| Carrinho no chão em direção a uma parede | _a registrar_ |
| Funcionamento só na bateria | _a registrar_ |
