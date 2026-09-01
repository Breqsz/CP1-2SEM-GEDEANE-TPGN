# Dependências — Carrinho-Robô

**Projeto:** CP1-2SEM-GEDEANE-TPGN

Árvore de dependências entre as etapas do projeto, para definir a ordem
correta de execução (uma etapa não pode começar antes que a anterior esteja
pronta).

```
Medir componentes
      ↓
Desenho 3D do chassi
      ↓
Teste de encaixes
      ↓
Corte/Impressão do chassi
      ↓
Montagem mecânica (motores + rodas)
      ↓
Montagem eletrônica (ESP32 + Ponte H + bateria)
      ↓
Teste de motores
      ↓
Software básico (avançar/recuar/virar)
      ↓
MVP concluído
      ↓
Itens adicionais (sensor ultrassônico, Wi-Fi, carenagem final, LEDs)
```

Essa ordem evita retrabalho: por exemplo, não faz sentido programar o
software de movimento (etapa 8) antes de a montagem eletrônica (etapa 6)
estar pronta, e não faz sentido desenhar a carenagem final antes de o
chassi básico já ter sido testado e validado.
