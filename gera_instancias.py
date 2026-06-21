import random

random.seed(789245)

total_instancias = 100
tamanhos_instancias = [50, 100, 1000]
min_valor = 1
max_valor = 100

results = {50   : [],
           100  : [],
           1000 : []}

for tamanho in tamanhos_instancias:
    for _ in range(total_instancias):
        instancia = []
        for _ in range(tamanho):
            instancia.append(random.randint(min_valor, max_valor))
        results[tamanho].append(instancia)

for tamanho, instancias in results.items():
    for i, instancia in enumerate(instancias):
        with open(f"testes/{tamanho}/instancias/inst_{i+1}.txt", "w") as f:
            f.write(f"{tamanho}\n")
            for val in instancia:
                f.write(f"{val}\n")
