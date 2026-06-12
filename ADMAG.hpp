#pragma once

#include <vector>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <random>
#include <set>
#include <unordered_set>
#include <cmath>

#include <candidato.hpp>

// Variáveis globais definidas na inicialização do programa baseado no tamanho de uma solução
extern const uint16_t tam_particao;
extern const uint8_t max_mutacoes;

candidato criarCandidatoAleatorio(const std::vector<uint8_t>& p, std::mt19937& gen);

setDeCandidatos selecionarPais(const setDeCandidatos& populacao_atual, const float_t p, std::mt19937& gen);

setDeCandidatos gerarFilhos(const setDeCandidatos& pais, std::mt19937& gen);

void causarMutacoes(std::vector<uint8_t>& genes, std::mt19937& gen);

setDeCandidatos determinarNovaPopulacao(const setDeCandidatos& populacao_atual, const setDeCandidatos& novos_candidatos, const uint16_t max_atuais, const uint16_t max_novos);

// Função principal do algoritmo
candidato ADMAG(const uint32_t tam_pop_inicial, const std::vector<uint8_t>& p_inicial, const uint32_t max_iter);

