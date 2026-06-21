#include "ADMAG.hpp"
#include <fstream>
#include <string>
#include <iostream>

// Pré-setagem de varáveis globais externas para o compilador não reclamar
unsigned int tarefas = 0;
unsigned int tam_particao = 0;
unsigned int max_mutacoes = 0;
unsigned int total_pais = 0;

// argumentos:
// [0] - nome do arquivo do programa (padrão)
// [1] - Nome do arquivo de saída
// [2] - valor k para o k-torneio (inteiro positivo)
// [3] - limite populacional (inteiro positivo)
// [4] - max da população atual para a próxima população (inteiro não-negativo) (filhos é o inverso desse valor)
// [5] - determina se o algoritmo rodará por n iterações ou por uma certa duração de tempo em minutos (0|1)
// [6] - mode 0 -> Máximo de iterações do algoritmo ; mode 1 -> tempo em minutos para execução (inteiro positivo)
// [7] - OPICIONAL : valor de seed para garantir convergência em múltiplos experimentos (inteiro não-negativo)
// [8] - OPICIONAL : nome do arquivo de entrada

int main(int argc, char **argv)
{

    bool error = false;

    // Checar se todos os argumentos foram inseridos
    if (argc < 7)
    {
        printf("Quantidade insuficiente de argumentos.\n");
        error = true;
    }

    if (argc > 9)
    {
        printf("Quantidade excessiva de argumentos.\n");
        error = true;
    }

    // #########################################################################################################################################################

    if (!error)
    {

        // Checar validade do valor de k do k-torneio
        int k;

        try
        {
            k = std::stoi(argv[2]);
            if (k < 1)
            {
                printf("O valor de k para o k-torneio deve ser maior que 0\n");
                error = true;
            }
            else
            {
                k = static_cast<unsigned int>(k);
            }
        }
        catch (std::out_of_range &e)
        {
            printf("K passado não foi inteiro\n");
            error = true;
        }

        // #########################################################################################################################################################

        // Checar validade do limite populacional
        int limite_populacional;

        try
        {
            limite_populacional = std::stoi(argv[3]);
            if (limite_populacional < 1)
            {
                printf("O limite populacional deve ser um valor positivo\n");
                error = true;
            }
            else
            {
                limite_populacional = static_cast<unsigned int>(limite_populacional);
            }
        }
        catch (std::out_of_range &e)
        {
            printf("limite populacional passado não foi inteiro\n");
            error = true;
        }

        // #########################################################################################################################################################

        int max_atuais;
        int max_novos;

        try
        {
            max_atuais = std::stoi(argv[4]);
            if (max_atuais < 0)
            {
                printf("O total de sobreviventes da população atual não pode ser negativo\n");
                error = true;
            }
            else if (max_atuais > limite_populacional)
            {
                printf("O total de sobreviventes da população atual não pode ser maior que o limite populacional\n");
                error = true;
            }
            else
            {
                max_atuais = static_cast<unsigned int>(max_atuais);
                max_novos = static_cast<unsigned int>(limite_populacional-max_atuais);
            }
        }
        catch (std::out_of_range &e)
        {
            printf("Total de sobreviventes da população atual passado não foi inteiro\n");
            error = true;
        }


        // #########################################################################################################################################################

        int modo_temporal;

        try
        {
            modo_temporal = std::stoi(argv[5]);
            if (modo_temporal != 0 && modo_temporal != 1)
            {
                printf("Valor de modo inválido, deve ser 0 ou 1\n");
                error = true;
            }
            else
            {
                modo_temporal = static_cast<bool>(modo_temporal);
            }
        }
        catch (std::out_of_range &e)
        {
            printf("Modo temporal passado não foi inteiro\n");
            error = true;
        }

        // #########################################################################################################################################################

        int condicao_de_parada;

        try
        {
            condicao_de_parada = std::stoi(argv[6]);
            if (condicao_de_parada < 1)
            {
                printf("Condição de parada deve ser um valor positivo, independente do modo\n");
                error = true;
            }
            else
            {
                condicao_de_parada = static_cast<unsigned int>(condicao_de_parada);
            }
        }
        catch (std::out_of_range &e)
        {
            printf("Condição de parada passsada não foi inteira\n");
            error = true;
        }

        // #########################################################################################################################################################

        int seed = -1;
        if (argc > 7)
        {
            try
            {
                seed = std::stoi(argv[7]);
                if (seed < 0)
                {
                    printf("Seed passada deve ser um valor não-negativo\n");
                    error = true;
                }
                else
                {
                    seed = static_cast<unsigned int>(seed);
                }
            }
            catch (std::out_of_range &e)
            {
                printf("Seed passada não foi inteira\n");
                error = true;
            }
        }

        // #########################################################################################################################################################

        // Lidando com arquivos e execução principal
        if (!error)
        {
            std::string out_file_name = argv[1];

            std::ofstream out_file;

            std::ifstream in_file;

            if (argc == 9)
            {
                // entrada por arquivo
                std::string in_file_name = argv[8];
                std::string line;

                in_file.open(in_file_name);

                if (!in_file.is_open())
                {
                    std::cerr << "Erro ao abrir arquivo\n";
                }
                if (!std::getline(in_file, line))
                {
                    std::cerr << "Erro ao ler primeira linha\n";
                }
                tarefas = std::stoi(line);
            }
            else
            {
                // entrada por stdin
                printf("Insira o tamanho da instância do problema e então insira a instância: \n");
                std::cin >> tarefas;
            }

            // Setando variáveis globais
            tam_particao = static_cast<unsigned int>(floor((tarefas / 3)));
            max_mutacoes = static_cast<unsigned int>(floor(sqrt(tarefas)));
            total_pais = std::max((unsigned int)1, static_cast<unsigned int>(floor(std::log2(limite_populacional))));

            std::vector<unsigned int> solucao_inicial(tarefas);

            if (argc == 9)
            {
                std::string line;
                unsigned int i = 0;
                while (std::getline(in_file, line))
                {
                    solucao_inicial[i] = static_cast<unsigned int>(std::stoi(line));
                    ++i;
                }
                in_file.close();
            }
            else
            {
                for (unsigned int i = 0; i < tarefas; ++i)
                {
                    std::cin >> solucao_inicial[i];
                }
            }

            // Execução do algoritmo principal
            const candidato melhor_solucao = ADMAG(k,
                                                   limite_populacional,
                                                   max_atuais,
                                                   max_novos,
                                                   modo_temporal,
                                                   condicao_de_parada,
                                                   solucao_inicial,
                                                   seed);

            out_file.open(out_file_name);
            const char *msg_permutacao = "A melhor permutacao das tarefas do problema foi: \n";
            const char *msg_inicializacao = "O melhor tempo de inicializacao correspondente para cada uma das tarefas foi: \n";
            const char *msg_makespan = "\nO makespan correspondente dessa melhor solucao encontrada foi: \n";

            for (int i = 0; i < 2; ++i)
            {
                if (i == 0)
                {
                    printf(msg_permutacao);
                    out_file << msg_permutacao;
                }
                else
                {
                    std::cout << std::endl;
                    out_file << std::endl;
                    printf(msg_inicializacao);
                    out_file << msg_inicializacao;
                }
                for (unsigned int j = 0; j < tarefas; ++j)
                {
                    if (i == 0)
                    {
                        std::cout << static_cast<int>(melhor_solucao.processamentos[j]) << " ";
                        out_file << static_cast<int>(melhor_solucao.processamentos[j]) << " ";
                    }
                    else
                    {
                        std::cout << static_cast<int>(melhor_solucao.inicios[j])  << " ";
                        out_file << static_cast<int>(melhor_solucao.inicios[j]) << " ";
                    }
                }
            }
            printf(msg_makespan);
            std::cout << melhor_solucao.makespan;
            out_file << msg_makespan;
            out_file << melhor_solucao.makespan;
            out_file.close();
        }
    }

    if (error)
    {
        printf("Os argumentos são:\n nome do arquivo com a instância do problema, valor k para o k-torneio, limite populacional, max da população atual para a próxima população, max dos filhos para a próxima população, modo de execução, número de iterações (mode = 0) ou tempo de execução em minutos (mode = 1), e um nome de arquivo de entrada opicional\n");
        return 1;
    }

    return 0;
}