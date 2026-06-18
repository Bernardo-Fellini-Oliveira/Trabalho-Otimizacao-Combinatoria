#pragma once
#include <utility>
#include <random>
#include <set>
#include <unordered_set>
#include <cmath>
#include <chrono>

#include "candidato.hpp"

// Variáveis globais definidas na inicialização do programa baseado no tamanho de uma solução
extern uint16_t tam_particao;
extern uint8_t max_mutacoes;
extern uint32_t total_pais;

candidato criarCandidatoAleatorio(const std::vector<uint8_t>& p, std::mt19937& gen);

setDeCandidatos selecionarPais(const setDeCandidatos& populacao_atual, const float_t k, const uint32_t limite_populacional, std::mt19937& gen);

setDeCandidatos gerarFilhos(const setDeCandidatos& pais, std::mt19937& gen);

void causarMutacoes(std::vector<uint8_t>& genes, std::mt19937& gen);

setDeCandidatos determinarNovaPopulacao(const setDeCandidatos& populacao_atual, const setDeCandidatos& novos_candidatos, const uint16_t max_atuais, const uint16_t max_novos);

// Função principal do algoritmo
candidato ADMAG(const uint16_t k, const uint32_t limite_populacional, const uint16_t max_atuais, const uint16_t max_novos, const bool modo_temporal, const uint32_t condicao_de_parada, const std::vector<uint8_t>& solucao_inicial, const int32_t seed);