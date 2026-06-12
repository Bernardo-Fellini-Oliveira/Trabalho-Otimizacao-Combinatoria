#pragma once

#include <vector>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <random>
#include <set>

// Total de tarefas
extern const uint16_t tarefas;

// Representação de uma possível solução
struct candidato {
    std::vector<uint8_t> processamento; // Tempo de processamento das tarefas, define a ordem de análise para tempos de início
    std::vector<uint32_t> inicios; // Tempos de início de cada tarefa. O tempo de início no índice i se refere ao tempo de início da tarefa cujo tempo de processamento está no índice i (inicializado com 0s)
    uint32_t makespan; // Duração total de execução das tarefas

    // Construtor do candidato
    // Calcula os valores dos tempos de início das tarefas e o makespan final da permutação (processmaneto deve ser pré-definido)
    candidato(std::vector<uint8_t> p) : processamento(p), inicios(tarefas), makespan(0) {
        for(uint16_t i = 1; i < tarefas; ++i) {
            uint32_t tempo_mais_limitante = 0;
            for(uint16_t j = 0; j < i; ++j) {
                uint8_t min_p = std::min(processamento[i], processamento[j]);
                tempo_mais_limitante = std::max(tempo_mais_limitante, inicios[j] + min_p);
            }
            inicios[i] = tempo_mais_limitante;
            makespan = std::max(makespan, tempo_mais_limitante + processamento[i]);
        }
    }
};