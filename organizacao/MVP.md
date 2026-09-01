# MVP — Carrinho-Robô

**Projeto:** CP1-2SEM-GEDEANE-TPGN
**Disciplina:** Project-based Maker Lab — Profª Dra. Gedeane G.S. Kenshima
**Equipe:** Rodrigo Brasileiro, Guilherme Rocha, Nikolas Rodrigues, Thiago Jardim

## O que é o nosso MVP

Com base nos componentes já levantados na tabela dimensional do projeto (motor
esquerdo, motor direito, Arduino/ESP32, Ponte H, bateria e sensor), o MVP do
carrinho-robô é definido como:

> Um chassi com duas rodas motorizadas, controlado por um ESP32 através de uma
> ponte H, alimentado por bateria, capaz de **avançar, recuar e virar** por
> meio de um controle básico (código pré-programado ou comandos simples via
> serial/Wi-Fi).

Isso segue diretamente a lógica apresentada na Aula 17: o MVP não precisa ter
todos os sensores e recursos planejados — precisa **funcionar no essencial**.
Um carrinho que anda e vira já resolve o "problema principal" do projeto
(demonstrar locomoção autônoma controlada), mesmo sem Bluetooth, RFID, LEDs,
buzzer ou modo autônomo.

## Quando estará minimamente funcional

O carrinho será considerado **minimamente funcional (MVP)** quando:

- [ ] Chassi estiver montado (motores + rodas fixados);
- [ ] Motores estiverem ligados à Ponte H e a Ponte H ao ESP32;
- [ ] Bateria estiver alimentando o circuito de forma estável;
- [ ] O ESP32 conseguir mover o carrinho para frente, para trás e virar
      (mesmo que via código fixo, sem interface de controle externa).

Esse ponto corresponde à etapa "Teste de motores" + "Software básico" na
árvore de dependências do projeto.

## Quando estará concluído (Produto Final)

O carrinho será considerado **concluído** quando, além do MVP, tiver:

- Carenagem definitiva (papelão, MDF ou impressão 3D — conforme decidido
  com base no protótipo em `cad/Carro.f3z`);
- Sensor ultrassônico integrado para detecção de obstáculos;
- Forma de controle definida (Wi-Fi/app ou modo autônomo básico);
- Testes finais de estabilidade, autonomia de bateria e acabamento.

