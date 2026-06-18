#include "ADMAG.hpp"
#include <fstream>
#include <string>
#include <iostream>

// argumentos:
// [0] - nome do arquivo do programa (padrão)
// [1] - Nome do arquivo de saída
// [2] - número de soluções iniciais (inteiro positivo > 2)
// [3] - valor k para o k-torneio (inteiro positivo)
// [4] - limite populacional (inteiro positivo)
// [5] - max da população atual para a próxima população (inteiro não-negativo)
// [6] - max dos filhos para a próxima população (inteiro não-negativo)
// [7] - determina se o algoritmo rodará por n iterações ou por uma certa duração de tempo em minutos (0|1)
// [8] - mode 0 -> Máximo de iterações do algoritmo ; mode 1 -> tempo em minutos para execução (inteiro positivo)

int main(int argc, char **argv)
{

    bool error = false;

    // Checar se todos os argumentos foram inseridos
    if (argc < 10)
    {
        printf("Quantidade insuficiente de argumentos.\n");
        error = true;
    }

    if (argc > 10)
    {
        printf("Quantidade excessiva de argumentos.\n");
        error = true;
    }

    // #########################################################################################################################################################

    if (!error)
    {
        // Checar validade do tamanho da população inicial
        const int32_t tam_pop_inicial = std::stoi(argv[2]);

        if (tam_pop_inicial < 2)
        {
            printf("População inicial deve ter ao menos 2 indivíduos\n");
            error = true;
        }
        else
        {
            // Os casts depois das seções de erro são para evitar erros causados pela entrada de um número negativo em um uint (underflow)
            static_cast<uint32_t>(tam_pop_inicial);
        }

        // #########################################################################################################################################################

        // Checar validade do valor de k do k-torneio
        const int16_t k = std::stoi(argv[3]);

        if (k < 1)
        {
            printf("O valor de k para o k-torneio deve ser maior que 0\n");
            error = true;
        }

        static_cast<uint16_t>(k);

        // #########################################################################################################################################################

        // Checar validade do limite populacional
        const int32_t limite_populacional = std::stoi(argv[4]);

        if (limite_populacional < 1)
        {
            printf("O limite populacional deve ser maior que 0\n");
            error = true;
        }

        if (tam_pop_inicial > limite_populacional)
        {
            printf("A população inicial não pode ser maior do que o limite populacional\n");
            error = true;
        }

        static_cast<uint32_t>(limite_populacional);

        // #########################################################################################################################################################

        const int32_t max_atual = std::stoi(argv[5]);

        if (max_atual < 0)
        {
            printf("O total de sobreviventes da população atual não pode ser negativo\n");
            error = true;
        }
        else if (max_atual > limite_populacional)
        {
            printf("O total de sobreviventes da população atual não pode ser maior que o limite populacional\n");
            error = true;
        }
        else
        {
            static_cast<uint32_t>(max_atual);
        }

        // #########################################################################################################################################################

        const int32_t max_filhos = std::stoi(argv[6]);

        if (max_filhos < 0)
        {
            printf("O total de sobreviventes dos filhos não pode ser negativo\n");
            error = true;
        }
        else if (max_filhos > limite_populacional)
        {
            printf("O total de sobreviventes dos filhos não pode ser maior que o limite populacional\n");
            error = true;
        }
        else if (max_atual + max_filhos != limite_populacional)
        {
            // Podemos testar isso agora que temos 'max_atual' e 'max_filhos'
            printf("O total de sobreviventes da população atual e de sobreviventes dos filhos deve ser igual ao limite populacional\n");
            error = true;
        }
        else
        {
            static_cast<uint32_t>(max_atual);
        }

        // #########################################################################################################################################################

        const int modo = std::stoi(argv[7]);

        if (modo != 0 && modo != 1)
        {
            printf("Valor de modo inválido, deve ser 0 ou 1\n");
            error = true;
        }
        else
        {
            static_cast<bool>(modo);
        }

        // #########################################################################################################################################################

        const int32_t condicao_de_parada = std::stoi(argv[8]);

        if (condicao_de_parada < 1)
        {
            printf("Condição de parada deve ser um valor positivo, independente do modo\n");
            error = true;
        }
        else
        {
            static_cast<uint32_t>(condicao_de_parada);
        }

        // #########################################################################################################################################################

        if(!error) {
            std::string out_file_name = argv[1];

            std::ofstream out_file;
            out_file.exceptions(std::ofstream::badbit);

            printf("Insira uma instância no formato do problema: \n");
            std::cin >> tarefas;
            tam_particao = static_cast<uint8_t>(floor((tarefas / 2)));
            max_mutacoes = static_cast<uint8_t>(tarefas / 10);
            std::vector<uint8_t> primeira_permutacao(tarefas);
            for(int i = 0; i < tarefas; ++i) {
                std::cin >> primeira_permutacao[i];
            }

            // TODO: definir lógica de executar por max_iter ou por tempo
            const candidato melhor_solucao = ADMAG(tam_pop_inicial);

            for(int i = 0; i < 2; ++i) {
                if (i == 0) {
                    printf("A permutação final das tarefas do problema foi: \n");
                } else {
                    std::cout << std::endl;
                    printf("O tempo de inicialização correspondente para cada uma das tarefas foi: \n");
                }
                for(int j = 0; j < tarefas; ++j) {
                    if (i == 0) {
                        std::cout << melhor_solucao.processamentos[j] << " ";
                    } else {
                        std::cout << melhor_solucao.inicios[j] << " ";
                    }
                }
            }
            printf("O makespan correspondente dessa solução foi: \n");
            std::cout << melhor_solucao.makespan;
        }
    }

    if (error)
    {
        printf("Os argumentos são:\n nome do arquivo com a instância do problema, número de soluções iniciais, valor k para o k-torneio, limite populacional, max da população atual para a próxima população, max dos filhos para a próxima população, modo de execução, número de iterações (mode = 0) ou tempo de execução em minutos (mode = 1)\n");
        return 1;
    }

    return 0;
}