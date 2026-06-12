#include "candidato.hpp"

// Função auxiliar para ordenação de candidatos baseado no seu valor de makespan (quanto menor o valor, melhor)
bool compararCandidatos(const candidato& a, const candidato& b) {
    return a.makespan < b.makespan;
}