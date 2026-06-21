#pragma once
#include <utility>
#include <random>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <algorithm>

#include "candidato.hpp"

// Variáveis globais definidas na inicialização do programa baseado no tamanho de uma solução
extern unsigned int tam_particao;
extern unsigned int max_mutacoes;
extern unsigned int total_pais;

candidato criarCandidatoAleatorio(const std::vector<unsigned int>& p, std::mt19937& gen);

vetorDeCandidatos selecionarPais(const vetorDeCandidatos& populacao_atual, const unsigned int k, const unsigned int limite_populacional, std::mt19937& gen);

vetorDeCandidatos gerarFilhos(const vetorDeCandidatos& pais, const unsigned int limite_populacional, std::mt19937& gen);

void causarMutacoes(std::vector<unsigned int>& genes, std::mt19937& gen);

vetorDeCandidatos determinarNovaPopulacao(const vetorDeCandidatos& populacao_atual, const vetorDeCandidatos& novos_candidatos, const unsigned int max_atuais, const unsigned int max_novos);

// Função principal do algoritmo
candidato ADMAG(const unsigned int k, const unsigned int limite_populacional, const unsigned int max_atuais, const unsigned int max_novos, const bool modo_temporal, const unsigned int condicao_de_parada, const std::vector<unsigned int>& solucao_inicial, const int seed);