#include "ADMAG.hpp"
#include "candidato.hpp"

// Cria um novo candidato  com uma permutação aleatória de tarefas
candidato criarCandidatoAleatorio(const std::vector<uint8_t>& p) {
    // boilerplate necessário para geração de valores aleatórios
    std::random_device rd;
    std::mt19937 g(rd);

    std::vector<uint8_t> new_p(p);
    std::shuffle(new_p.begin(), new_p.end(), g);
    candidato novo_candidato(p);
    return novo_candidato;
}

std::set<candidato> selecionarPais(const std::set<candidato> populacao_atual, const float_t p) {

}

// Função principal do algoritmo
candidato ADMAG(const uint32_t tam_pop_inicial, const std::vector<uint8_t>& p_inicial) {
    std::set<candidato, bool(*)(const candidato& a, const candidato& b)> populacao(compararCandidatos);
    for(uint32_t i = 0; i < tam_pop_inicial; ++i) {
        // std::move evita guardar cópias extras do objeto
        populacao.insert(std::move(criarCandidatoAleatorio(p_inicial)));
    }

}