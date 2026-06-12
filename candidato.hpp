#pragma once

#include <vector>
#include <algorithm>
#include <cstdint>

#define setDeCandidatos std::set<candidato, bool(*)(const candidato& a, const candidato& b)>

// Total de tarefas do arquivo passado em .main
extern const uint16_t tarefas;

// Representação de uma possível solução
struct candidato {
    std::vector<uint8_t> processamentos; // Tempos de processamentos das tarefas, define a ordem de análise para tempos de início
    std::vector<uint32_t> inicios; // Tempos de início de cada tarefa. O tempo de início no índice i se refere ao tempo de início da tarefa cujo tempo de processamentos está no índice i (inicializado com 0s)
    uint32_t makespan; // Duração total de execução das tarefas

    // Construtor do candidato
    // Calcula os valores dos tempos de início das tarefas e o makespan final da permutação (processmaneto deve ser pré-definido)
    // Complexidade O(n^2)
    candidato(std::vector<uint8_t> p) : processamentos(p), inicios(tarefas), makespan(0) {
        for(uint16_t i = 1; i < tarefas; ++i) {
            uint32_t inicio_mais_limitante = 0;
            for(uint16_t j = 0; j < i; ++j) {
                uint8_t min_p = std::min(processamentos[i], processamentos[j]);
                inicio_mais_limitante = std::max(inicio_mais_limitante, inicios[j] + min_p);
            }
            inicios[i] = inicio_mais_limitante;
            makespan = std::max(makespan, inicio_mais_limitante + processamentos[i]);
        }
    }
};

// Função auxiliar para ordenação de candidatos baseado no seu valor de makespan (quanto menor o valor, melhor)
bool compararCandidatos(const candidato& a, const candidato& b);