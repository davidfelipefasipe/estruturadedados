import time


def bubble_sort(lista):
    v = lista[:]
    comparacoes = trocas = 0
    n = len(v)
    for i in range(n - 1):
        houve_troca = False
        for j in range(n - 1 - i):
            comparacoes += 1
            if v[j] > v[j + 1]:
                v[j], v[j + 1] = v[j + 1], v[j]
                trocas += 1
                houve_troca = True
        if not houve_troca:
            break
    return v, comparacoes, trocas


def selection_sort(lista):
    v = lista[:]
    comparacoes = trocas = 0
    n = len(v)
    for i in range(n - 1):
        m = i
        for j in range(i + 1, n):
            comparacoes += 1
            if v[j] < v[m]:
                m = j
        if m != i:
            v[i], v[m] = v[m], v[i]
            trocas += 1
    return v, comparacoes, trocas


def quick_sort(lista):
    v = lista[:]
    contagem = {"comparacoes": 0, "trocas": 0}

    def particiona(ini, fim):
        pivo = v[fim]
        i = ini - 1
        for j in range(ini, fim):
            contagem["comparacoes"] += 1
            if v[j] <= pivo:
                i += 1
                if i != j:
                    v[i], v[j] = v[j], v[i]
                    contagem["trocas"] += 1
        if i + 1 != fim:
            v[i + 1], v[fim] = v[fim], v[i + 1]
            contagem["trocas"] += 1
        return i + 1

    def quick(ini, fim):
        if ini < fim:
            p = particiona(ini, fim)
            quick(ini, p - 1)
            quick(p + 1, fim)

    quick(0, len(v) - 1)
    return v, contagem["comparacoes"], contagem["trocas"]


def medir(funcao, lista, repeticoes=1000):
    """Executa a função várias vezes e devolve o tempo médio em microssegundos."""
    inicio = time.perf_counter()
    for _ in range(repeticoes):
        resultado = funcao(lista)
    fim = time.perf_counter()
    tempo_us = (fim - inicio) / repeticoes * 1_000_000
    return resultado, tempo_us


def main():
    entrada = input("Digite a lista separada por espaços (Enter para usar 5 3 8 1 4): ").strip()
    lista = [int(x) for x in entrada.split()] if entrada else [5, 3, 8, 1, 4]

    print(f"\nLista original: {lista}\n")
    print(f"{'Método':<16}{'Resultado':<26}{'Comparações':>12}{'Trocas':>8}{'Tempo (µs)':>12}")
    print("-" * 74)

    metodos = [
        ("Bubble Sort", bubble_sort),
        ("Selection Sort", selection_sort),
        ("Quick Sort", quick_sort),
    ]
    for nome, funcao in metodos:
        (ordenada, comp, trocas), tempo = medir(funcao, lista)
        print(f"{nome:<16}{str(ordenada):<26}{comp:>12}{trocas:>8}{tempo:>12.2f}")

    print("\nTempo médio de 1000 execuções; a lista original não é alterada.")


if __name__ == "__main__":
    main()