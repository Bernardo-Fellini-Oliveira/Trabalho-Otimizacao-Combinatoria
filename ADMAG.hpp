#pragma once

#include <vector>
#include <algorithm>
#include <utility>
#include <cstdint>
#include <random>
#include <set>

#include <candidato.hpp>



bool compararCandidatos(const candidato& a, const candidato& b);

candidato criarCandidatoAleatorio(const std::vector<uint8_t>& p);

std::set<candidato> selecionarPais(const std::set<candidato> populacao_atual);

// Função principal do algoritmo
candidato ADMAG(int n_candidatos_iniciais);

