Testes e Resultados — Carrinho-Robô

Este documento registra os testes realizados durante o desenvolvimento, os
problemas encontrados e as correções aplicadas. Cada teste tem um
procedimento, para que possa ser repetido.

1. Problemas encontrados no firmware (v1 → v2)

Identificados na revisão do código v1
(../src/versoes/codigo_v1.ino) e corrigidos
na v2 (../src/codigo.ino):

#	Problema	Sintoma	Causa	Correção (v2)
P1	ENB no GPIO 6	ESP32 trava/reinicia ao usar o pino; motor direito não é habilitado	No ESP32, os GPIO 6–11 são ligados à memória flash interna	ENB movido para o GPIO 23
P2	ENA/ENB nunca em HIGH	Motores não giram mesmo com IN1–IN4 corretos	Pinos configurados como saída, mas não ativados	digitalWrite(ENA/ENB, HIGH) no setup()
P3	Motor não fica ligado com o botão pressionado	Carrinho dá um "tranco" e não segue andando	Callback onPressed reage à transição do botão	Loop reescrito com polling (isUpPressed() etc.)
P4	Faltava configuração da biblioteca Dabble	Módulo Gamepad sem configuração explícita	Ausência de CUSTOM_SETTINGS / INCLUDE_GAMEPAD_MODULE	#defines adicionados antes do #include
P5	Sem proteção contra colisão	Carrinho bate em obstáculos	Nenhum sensor no v1	HC-SR04 + trava de 20 cm
2. Registro de testes
#	Data	Teste	Procedimento	Resultado esperado	Resultado obtido	Problema / correção
T1	18/09/2026 (Aula 16)	Motores com L298N em bancada	Alimentar o L298N e alternar os níveis de IN1–IN4	Cada motor gira nos dois sentidos	Os dois motores giraram nos dois sentidos corretamente	Nenhum problema identificado
T2	18/09/2026	Pareamento Bluetooth	Ligar o ESP32, abrir o Dabble e conectar a "My Bluetooth Car"	Conexão estabelecida	Conexão estabelecida em aproximadamente 4 s	Nenhum problema identificado
T3	18/09/2026	Comandos do Gamepad (v1)	Pressionar cada seta no app	Carrinho executa cada comando	Motor não permanecia ligado com o botão pressionado	Ver P1–P4 → firmware v2
T4	18/09/2026	Comandos do Gamepad (v2)	Segurar cada seta por ~3 s e soltar	Movimento contínuo enquanto pressionado; para ao soltar	Movimentos permaneceram ativos durante o pressionamento e cessaram ao soltar	Nenhum problema identificado
T5	18/09/2026	Sentido das curvas	Pressionar ← e →	Gira para o lado indicado	As curvas ocorreram para os lados correspondentes aos comandos	Nenhuma inversão observada
T6	18/09/2026	Leitura do HC-SR04	Monitor serial a 115200; objeto a 10, 20, 30 e 50 cm medidos com régua	Distância lida próxima da real (±2 cm)	Leituras permaneceram dentro de ±2 cm em relação às distâncias medidas	Nenhuma correção necessária
T7	18/09/2026	Parada automática	Segurar ↑ em direção a uma parede	Carrinho para a ~20 cm, sem tocar a parede	Carrinho parou a aproximadamente 19 cm da parede em 5 de 5 tentativas	Nenhum problema identificado
T8	18/09/2026	Ré e curvas perto do obstáculo	Com obstáculo a < 20 cm, pressionar ↓, ← e →	Carrinho consegue sair (só o avanço é bloqueado)	Ré e curvas permaneceram disponíveis enquanto o avanço foi bloqueado	Nenhum problema identificado
T9	18/09/2026	Encaixe no chassi	Montar todos os componentes	Tudo fixo, sem folga excessiva	Componentes ficaram firmes, sem folgas que interferissem no funcionamento	Ajustado o posicionamento do suporte do sensor
T10	18/09/2026	Estabilidade (com carenagem)	Andar em linha reta e fazer curvas seguidas	Não tomba nem arrasta a carenagem	Não houve tombamento nem contato da carenagem com o piso durante o percurso	Nenhum problema identificado
T11	18/09/2026	Autonomia da bateria	Uso contínuo a partir de carga plena, cronometrar	≥ 20 min	Aproximadamente 27 min de funcionamento contínuo	Nenhuma correção necessária
T12	18/09/2026	Alcance do Bluetooth	Afastar-se com o celular até perder o controle	≥ 5 m	Controle permaneceu estável até aproximadamente 8 m em área aberta	Pequenas oscilações observadas acima de 8 m
T13	18/09/2026	Integração final	Percurso completo: frente, ré, curvas e parada no obstáculo	Robô funcional de ponta a ponta	Percurso completo executado corretamente, incluindo a parada diante do obstáculo	Nenhum problema crítico identificado
Medições do sensor (T6)
Distância real (cm)	Leitura 1	Leitura 2	Leitura 3	Média	Erro
10	10,4	9,8	10,2	10,1	+0,1 cm
20	20,6	19,7	20,2	20,2	+0,2 cm
30	30,8	29,6	30,3	30,2	+0,2 cm
50	50,7	49,5	50,4	50,2	+0,2 cm
3. Limitações conhecidas

Velocidade fixa: sem PWM, o carrinho anda sempre na velocidade máxima,
o que reduz a precisão em curvas.

Tempo de reação do sensor: a leitura ocorre uma vez por ciclo do
loop() (~30–60 ms). Em velocidade máxima, o carrinho percorre alguns
centímetros antes de parar; por isso a trava é de 20 cm e não menor.

Ângulo do sensor: o HC-SR04 detecta objetos em um cone estreito à
frente; obstáculos baixos, finos ou inclinados podem não ser detectados.

Sem leitura (-1): se o sensor não recebe eco, o avanço é permitido.
Isso evita que o carrinho fique travado em espaço aberto, mas significa
que uma falha do sensor não bloqueia o movimento.

Só a frente é protegida: a ré e as curvas não usam o sensor.

4. Resultado final

Ao final dos testes, o carrinho-robô avançou, recuou e fez curvas pelo
comando Bluetooth do app Dabble, apresentando pequeno atraso na resposta aos
comandos. O alcance do Bluetooth foi de aproximadamente 8 m em área aberta.
O sensor ultrassônico interrompeu o avanço em 5 de 5 tentativas, com parada a
aproximadamente 19 cm do obstáculo. A bateria garantiu cerca de 27 min de uso
contínuo. As principais limitações são a velocidade fixa e o cone de detecção
do sensor, descritas acima.