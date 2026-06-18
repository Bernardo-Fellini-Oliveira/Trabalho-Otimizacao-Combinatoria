#include "ADMAG.hpp"
#include <fstream>
#include <string>
#include <iostream>

uint32_t random_seed;

// argumentos:
// [0] - nome do arquivo do programa (padrão)
// [1] - Nome do arquivo de saída
// [2] - valor k para o k-torneio (inteiro positivo)
// [3] - limite populacional (inteiro positivo)
// [4] - max da população atual para a próxima população (inteiro não-negativo)
// [5] - max dos filhos para a próxima população (inteiro não-negativo)
// [6] - determina se o algoritmo rodará por n iterações ou por uma certa duração de tempo em minutos (0|1)
// [7] - mode 0 -> Máximo de iterações do algoritmo ; mode 1 -> tempo em minutos para execução (inteiro positivo)
// [8] - OPICIONAL : valor de seed para garantir convergência em múltiplos experimentos (inteiro não-negativo)
// [9] - OPICIONAL : nome do arquivo de entrada

int main(int argc, char **argv)
{

    bool error = false;

    // Checar se todos os argumentos foram inseridos
    if (argc < 8)
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

        // Checar validade do valor de k do k-torneio
        int16_t k;

        try {
            k = std::stoi(argv[2]);
            if (k < 1)
            {
                printf("O valor de k para o k-torneio deve ser maior que 0\n");
                error = true;
            }
            static_cast<uint16_t>(k);
        } catch(std::out_of_range& e) {
            printf("K passado não foi inteiro\n");
            error = true;
        }



        // #########################################################################################################################################################

        // Checar validade do limite populacional
        int32_t limite_populacional;

        try {
            limite_populacional = std::stoi(argv[3]);
            if (limite_populacional < 2)
            {
                printf("O limite populacional deve ser maior que 1 para poder simular reprodução entre soluções\n");
                error = true;
            }
        } catch(std::out_of_range& e) {
            printf("limite populacional passado não foi inteiro\n");
            error = true;
        }


        static_cast<uint32_t>(limite_populacional);

        // #########################################################################################################################################################

        int32_t max_atuais;

        try {
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
                static_cast<uint32_t>(max_atuais);
            }
        } catch(std::out_of_range& e) {
            printf("Total de sobreviventes da população atual passado não foi inteiro\n");
            error = true;
        }

        // #########################################################################################################################################################

        int32_t max_novos;

        try {
            max_novos = std::stoi(argv[5]);
            if (max_novos < 0)
            {
                printf("O total de sobreviventes dos filhos não pode ser negativo\n");
                error = true;
            }
            else if (max_novos > limite_populacional)
            {
                printf("O total de sobreviventes dos filhos não pode ser maior que o limite populacional\n");
                error = true;
            }
            else if (max_atuais + max_novos != limite_populacional)
            {
                // Podemos testar isso agora que temos 'max_atuais' e 'max_novos'
                printf("O total de sobreviventes da população atual e de sobreviventes dos filhos deve ser igual ao limite populacional\n");
                error = true;
            }
            else
            {
                static_cast<uint32_t>(max_novos);
            }
        } catch(std::out_of_range& e) {
            printf("Total de sobreviventes dos filhos passado não foi inteiro\n");
            error = true;
        }

        // #########################################################################################################################################################

        int modo_temporal;

        try {
            modo_temporal = std::stoi(argv[6]);
            if (modo_temporal != 0 && modo_temporal != 1)
            {
                printf("Valor de modo inválido, deve ser 0 ou 1\n");
                error = true;
            }
            else
            {
                static_cast<bool>(modo_temporal);
            }
        } catch(std::out_of_range& e) {
            printf("Modo temporal passado não foi inteiro\n");
            error = true;
        }

        // #########################################################################################################################################################

        int32_t condicao_de_parada;

        try {
            condicao_de_parada = std::stoi(argv[7]);
            if (condicao_de_parada < 1)
            {
                printf("Condição de parada deve ser um valor positivo, independente do modo\n");
                error = true;
            }
            else
            {
                static_cast<uint32_t>(condicao_de_parada);
            }
        } catch(std::out_of_range& e) {
            printf("Condição de parada passsada não foi inteira\n");
            error = true;
        }

        // #########################################################################################################################################################

        int32_t seed = -1;
        if(argc > 8) {
            try {
                seed = std::stoi(argv[8]);
                if(seed < 0) {
                    printf("Seed passada deve ser um valor não-negativo\n");
                    error = true;
                } else {
                    static_cast<uint32_t>(seed);
                }
            } catch(std::out_of_range& e) {
                printf("Seed passada não foi inteira\n");
                error = true;
            }
        }

        // #########################################################################################################################################################

        // Lidando com arquivos e execução principal
        if(!error) {
            try{
                std::string out_file_name = argv[1];

                std::ofstream out_file;
                out_file.exceptions(std::ifstream::failbit | std::ofstream::badbit);

                std::ifstream in_file;
                in_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

                if(argc == 10) {
                    // entrada por arquivo
                    std::string in_file_name = argv[9];
                    std::string line;

                    in_file.open(in_file_name);
                    std::getline(in_file, line);
                    tarefas = std::stoi(line);
                } else {
                    // entrada por stdin
                    printf("Insira uma instância no formato do problema: \n");
                    std::cin >> tarefas;
                }

                tam_particao = static_cast<uint8_t>(floor((tarefas / 3)));
                max_mutacoes = static_cast<uint8_t>(floor((tarefas / 2)));
                std::vector<uint8_t> solucao_inicial(tarefas);

                if(argc == 10) {
                    std::string line;
                    while(std::getline(in_file, line)) {
                        solucao_inicial.push_back(static_cast<uint8_t>(std::stoi(line)));
                    }
                    in_file.close();
                } else {
                    for(int i = 0; i < tarefas; ++i) {
                        std::cin >> solucao_inicial[i];
                    }
                }

                // TODO: definir lógica de executar por max_iter ou por tempo
                const candidato melhor_solucao = ADMAG(k,
                                                    limite_populacional,
                                                    max_atuais,
                                                    max_novos,
                                                    modo_temporal,
                                                    condicao_de_parada,
                                                    solucao_inicial,
                                                    seed);

                out_file.open(out_file_name);
                const char* resultado_permutacao = "A melhor permutação das tarefas do problema foi: \n";
                const char* resultado_inicializacao = "O melhor tempo de inicialização correspondente para cada uma das tarefas foi: \n";
                const char* resultado_makespan = "O makespan correspondente dessa melhor solução encontrada foi: \n";

                for(int i = 0; i < 2; ++i) {
                    if (i == 0) {
                        printf(resultado_permutacao);
                        out_file << resultado_permutacao;
                    } else {
                        std::cout << std::endl;
                        out_file << std::endl;
                        printf(resultado_inicializacao);
                        out_file << resultado_inicializacao;
                    }
                    for(int j = 0; j < tarefas; ++j) {
                        if (i == 0) {
                            std::cout << melhor_solucao.processamentos[j] << " ";
                            out_file << melhor_solucao.processamentos[j] << " ";
                        } else {
                            std::cout << melhor_solucao.inicios[j] << " ";
                            out_file << melhor_solucao.inicios[j] << " ";
                        }
                    }
                }
                printf(resultado_makespan);
                std::cout << melhor_solucao.makespan;
                out_file << resultado_makespan;
                out_file << melhor_solucao.makespan;
                out_file.close();
            } catch(std::ios_base::failure& e) {
                printf("Erro de arquivo. Certifique-se de que os nomes passados como argumentos estão certos\n");
                error = true;
            }
        }
    }

    if (error)
    {
        printf("Os argumentos são:\n nome do arquivo com a instância do problema, valor k para o k-torneio, limite populacional, max da população atual para a próxima população, max dos filhos para a próxima população, modo de execução, número de iterações (mode = 0) ou tempo de execução em minutos (mode = 1), e um nome de arquivo de entrada opicional\n");
        return 1;
    }

    return 0;
}