#pragma once
#include <vector>
#include <algorithm>
#include <functional>

#define setDeCandidatos std::set<candidato, bool(*)(const candidato& a, const candidato& b)>
#define vetorDeCandidatos std::vector<candidato>

// Total de tarefas do arquivo passado em .main
extern unsigned int tarefas;

// Representação de uma possível solução
struct candidato {
    std::vector<unsigned int> processamentos; // Tempos de processamentos das tarefas, define a ordem de análise para tempos de início
    std::vector<unsigned int> inicios; // Tempos de início de cada tarefa. O tempo de início no índice i se refere ao tempo de início da tarefa cujo tempo de processamentos está no índice i (inicializado com 0s)
    unsigned int makespan; // Duração total de execução das tarefas

    // Construtor do candidato
    // Calcula os valores dos tempos de início das tarefas e estima o makespan final da permutação de forma gulosa (processmaneto deve ser pré-definido)
    // Complexidade O(n^2)
    candidato(std::vector<unsigned int> p) : processamentos(p), inicios(tarefas), makespan(0) {
        inicios[0] = 0;
        makespan = processamentos[0];
        for(unsigned int i = 1; i < tarefas; ++i) {
            unsigned int inicio_mais_limitante = 0;
            for(unsigned int j = 0; j < i; ++j) {
                unsigned int min_p = std::min(processamentos[i], processamentos[j]);
                inicio_mais_limitante = std::max(inicio_mais_limitante, inicios[j] + min_p);
            }
            inicios[i] = inicio_mais_limitante;
            makespan = std::max(makespan, inicio_mais_limitante + processamentos[i]);
        }
    }
};

// Função auxiliar para ordenação de candidatos baseado no seu valor de makespan (quanto menor o valor, melhor)
bool compararCandidatos(const candidato& a, const candidato& b);