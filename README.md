# Trabalho-Otimizacao-Combinatoria
Projeto Final da Disciplina de Otimização Combinatória do Curso de Ciência da Computação da Universidade Federal do Rio Grande do Sul - UFRGS. Envolve desenvolver uma aplicação CLI que implementa o uso de uma meta-heurística para buscar um candidato para a melhor solução de um problema de programação inteira

# Compilação
1. Clone esse repositório localmente.
2. Certifique-se de que tenha o compilador g++ instalado.
3. Compile com mingw32-make (assumindo Windows). Isso criará o arquivo "heuristica.exe".
4. Caso mude o código fonte, use o comando mingw32-make clean antes de compilar novamente para evitar conflitos.

# Execução
Para executar o programa:
1. Certifique-se de conhecer os argumentos necessários:

// [1] - Nome do arquivo de saída
// [2] - valor k para o k-torneio (inteiro positivo)
// [3] - limite populacional (inteiro positivo)
// [4] - max da população atual para a próxima população (inteiro não-negativo)
// [5] - max dos filhos para a próxima população (inteiro não-negativo)
// [6] - determina se o algoritmo rodará por n iterações ou por uma certa duração de tempo em minutos (0|1)
// [7] - mode 0 -> Máximo de iterações do algoritmo ; mode 1 -> tempo em minutos para execução (inteiro positivo)
// [8] - OPICIONAL : valor de seed para garantir convergência em múltiplos experimentos (inteiro não-negativo)
// [9] - OPICIONAL : nome do arquivo de entrada

2. Execute com "./heuristica.exe argumentos"
3. Se não inserir arquivo de entrada, você precisará inserir manualmente na entrada padrão: primeiro o tamanho da instância, e depois a lista de tempos de processamento de cada tarefa, separadas por espaço

# Solver
Para usar o solver: 
1. Abra o notebook ADM_Solver no google colabs com um ambiente de execução "Julia"
2. Faça upload do arquivo cujo
3. Troque o comentário em "filename = # Insira aqui o nome do arquivo da instância" com o arquivo de instância .dat da sua preferência
4. Execute as células