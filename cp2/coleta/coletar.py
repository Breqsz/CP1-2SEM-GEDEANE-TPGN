"""
CP2 - Grava a coleta do HC-SR04 direto em CSV.

Uso (no Windows, com o Serial Monitor da Arduino IDE FECHADO):
    python coletar.py COM5 perto 300     -> grava 300 amostras em ../dados/coleta_perto.csv
    python coletar.py COM5 medio 300
    python coletar.py COM5 longe 300
    python coletar.py juntar             -> junta tudo em ../dados/dataset.csv

Requer: pip install pyserial
"""
import re
import sys
import time
from pathlib import Path

PASTA_DADOS = Path(__file__).resolve().parent.parent / "dados"
CLASSES = {"perto": "1", "medio": "2", "longe": "3"}
FAIXAS = {"perto": (0, 20), "medio": (20, 50), "longe": (50, 201)}  # cm
CABECALHO = "distancia,label"
LINHA_VALIDA = re.compile(r"^\d+(\.\d+)?,(perto|medio|longe)$")


def coletar(porta, classe, quantidade):
    import serial

    arquivo = PASTA_DADOS / f"coleta_{classe}.csv"
    PASTA_DADOS.mkdir(parents=True, exist_ok=True)
    novo = not arquivo.exists()

    with serial.Serial(porta, 115200, timeout=1) as esp:
        print("Conectando (a placa reinicia ao abrir a porta)...")
        time.sleep(2.5)
        esp.reset_input_buffer()

        input(f"Posicione o obstáculo para a classe '{classe}' e aperte ENTER...")
        esp.write(CLASSES[classe].encode())

        # Só grava leituras dentro da faixa da classe: fora dela, o obstáculo
        # saiu da posição (ou o sensor viu o fundo) e o rótulo estaria errado.
        gravadas = 0
        descartadas = 0
        minimo, maximo = FAIXAS[classe]
        with arquivo.open("a", encoding="utf-8", newline="\n") as f:
            if novo:
                f.write(CABECALHO + "\n")
            while gravadas < quantidade:
                linha = esp.readline().decode(errors="ignore").strip()
                if LINHA_VALIDA.match(linha):
                    distancia = float(linha.split(",")[0])
                    if minimo <= distancia < maximo:
                        f.write(linha + "\n")
                        gravadas += 1
                        print(f"[{gravadas}/{quantidade}] {linha}")
                    else:
                        descartadas += 1
                        print(f"[descartada] {linha}  <-- fora da faixa")
                elif linha:
                    print(linha)

        esp.write(b"x")

    print(f"\nPronto: {gravadas} amostras adicionadas em {arquivo}")
    print(f"Descartadas (fora de {minimo}–{maximo} cm): {descartadas}")


def juntar():
    linhas = []
    contagem = {}
    for classe in CLASSES:
        arquivo = PASTA_DADOS / f"coleta_{classe}.csv"
        if not arquivo.exists():
            print(f"Aviso: {arquivo.name} não encontrado")
            continue
        dados = [l.strip() for l in arquivo.read_text(encoding="utf-8").splitlines()
                 if LINHA_VALIDA.match(l.strip())]
        linhas += dados
        contagem[classe] = len(dados)

    destino = PASTA_DADOS / "dataset.csv"
    destino.write_text(CABECALHO + "\n" + "\n".join(linhas) + "\n", encoding="utf-8")

    print(f"dataset.csv gerado em {destino}")
    for classe, n in contagem.items():
        print(f"  {classe}: {n}")
    print(f"  total: {len(linhas)}")


if __name__ == "__main__":
    args = sys.argv[1:]
    if args == ["juntar"]:
        juntar()
    elif len(args) == 3 and args[1] in CLASSES and args[2].isdigit():
        coletar(args[0], args[1], int(args[2]))
    else:
        print(__doc__)
