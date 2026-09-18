# MVP — Carrinho-Robô

**Projeto:** CP1-2SEM-GEDEANE-TPGN
**Disciplina:** Project-based Maker Lab — Profª Dra. Gedeane G.S. Kenshima
**Equipe:** Rodrigo Brasileiro, Guilherme Rocha, Nikolas Rodrigues, Thiago Jardim

## O que é o nosso MVP

Com base nos componentes levantados na tabela dimensional do projeto (motor
esquerdo, motor direito, Arduino/ESP32, Ponte H, bateria e sensor), o MVP do
carrinho-robô foi definido como:

> Um chassi com duas rodas motorizadas, controlado por um ESP32 através de uma
> ponte H, alimentado por bateria, capaz de **avançar, recuar e virar** por
> meio de um controle básico (código pré-programado ou comandos simples).

Isso segue a lógica apresentada na Aula 17: o MVP não precisa ter todos os
sensores e recursos planejados, precisa **funcionar no essencial**. Um
carrinho que anda e vira já resolve o problema principal do projeto
(demonstrar locomoção controlada), mesmo sem sensores, LEDs ou modo autônomo.

> **Nota de evolução:** na definição original do MVP, o controle previsto era
> por serial/Wi-Fi. Na implementação, o controle passou a ser por
> **Bluetooth** (app Dabble). Ver [`Decisoes.md`](Decisoes.md), Decisão 2.

## Quando estará minimamente funcional

O carrinho é considerado **minimamente funcional (MVP)** quando:

- [x] Chassi montado (motores + rodas fixados);
- [x] Motores ligados à Ponte H e a Ponte H ao ESP32;
- [x] Bateria alimentando o circuito de forma estável;
- [x] O ESP32 move o carrinho para frente, para trás e faz curvas.

Esse ponto corresponde às etapas "Teste de motores" + "Software básico" da
árvore de [`Dependencias.md`](Dependencias.md).

## Quando estará concluído (Produto Final)

Além do MVP, o produto final precisa ter:

- [x] Sensor ultrassônico integrado para detecção de obstáculos;
- [x] Forma de controle definida (Bluetooth via app Dabble);
- [x] Testes finais de estabilidade, autonomia de bateria e acabamento
      (ver [`../testes/TESTES.md`](../testes/TESTES.md)).

## Resultado

| Etapa | Situação |
| :--- | :--- |
| MVP (andar e virar com ESP32 + ponte H + bateria) | ✅ Atingido |
| Controle remoto sem fio | ✅ Bluetooth (Dabble) |
| Sensor com função real | ✅ Parada automática a ≤ 20 cm |
| Produto final com carenagem | ✅ Atingido |
