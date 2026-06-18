#include "ADMAG.hpp"

// Cria um novo candidato  com uma permutação aleatória de tarefas
candidato criarCandidatoAleatorio(const std::vector<uint8_t>& p, std::mt19937& gen) {
    std::vector<uint8_t> new_p(p);
    // mistura os valores do vetor aleatoriamente
    std::shuffle(new_p.begin(), new_p.end(), gen);
    candidato novo_candidato(new_p);
    return novo_candidato;
}

// Função que seleciona os pais que irão reproduzir para gerar os novos candidatos da próxima população
// Usa algoritmo de k-torneio (seleciona k candidatos n vezes [n = limite populacional] e seleciona o melhor deles para reprodução a cada rodada)
// implementação sem reposição
setDeCandidatos selecionarPais(const setDeCandidatos& populacao_atual, const float_t k, const uint32_t limite_populacional, std::mt19937& gen) {
    std::uniform_int_distribution<int32_t> dist(0, limite_populacional);
    setDeCandidatos pais(compararCandidatos);
    auto it = populacao_atual.begin();
    while(pais.size() < total_pais) {
        uint32_t idx_do_selecionado = dist(gen);
        std::advance(it, idx_do_selecionado);
        candidato reprodutor = *it;
        for(int j = 0; j < k-1; ++j) {
            it = populacao_atual.begin();
            idx_do_selecionado = dist(gen);
            std::advance(it, idx_do_selecionado);
            reprodutor = std::max(reprodutor, *it, compararCandidatos);
        }
        pais.insert(reprodutor);
    }
    return pais;
}

// Gera os filhos dessa geração a partir dos pais selecionados
// A cada iteração, são gerados (2^total_pais)-1 filhos
setDeCandidatos gerarFilhos(const setDeCandidatos& pais, std::mt19937& gen) {
    std::uniform_int_distribution<uint16_t> dist(0, tarefas-tam_particao);
    setDeCandidatos filhos;
    for(auto it1 = pais.cbegin(); it1 != std::prev(pais.cend()); ++it1) {
        const candidato& pai1 = *it1;
        for(auto it2 = std::next(it1); it2 != pais.cend(); ++it2) {
            const candidato& pai2 = *it2;
            const uint16_t inicio_particao = dist(gen);
            std::unordered_set<uint8_t> genes_copiados;
            std::vector<uint8_t> genes_do_filho(tarefas);
            for(uint16_t i = inicio_particao; i < tam_particao; ++i) {
                const uint8_t gene_copiado = pai1.processamentos[i];
                genes_do_filho[i] = gene_copiado;
                genes_copiados.insert(gene_copiado);
            }
            uint16_t i = tam_particao;
            for(uint16_t j = 0; j < tarefas; ++j) {
                if(i >= tarefas) {
                    i = 0;
                }
                const uint8_t gene_a_inserir = pai2.processamentos[j];
                if(genes_copiados.find(gene_a_inserir) == genes_copiados.end()) {
                    genes_do_filho[i] = gene_a_inserir;
                    ++i;
                }
            }
            causarMutacoes(genes_do_filho, gen);
            candidato filho(genes_do_filho);
            filhos.insert(filho);
        }
    }
    return filhos;
}

// Causa mutações aleatórias nos genes dos filhos, alterando o vetor diretamente
void causarMutacoes(std::vector<uint8_t>& genes, std::mt19937& gen) {
    std::uniform_int_distribution<uint16_t> dist_total_mut(0, max_mutacoes);
    const uint8_t total_mutacoes = dist_total_mut(gen);
    std::uniform_int_distribution<uint16_t> dist_idxs_mutados(0, tarefas-1);
    for(uint8_t i = 0; i < total_mutacoes; ++i) {
        uint16_t idx1 = dist_idxs_mutados(gen);
        uint16_t idx2 = dist_idxs_mutados(gen);
        while(idx1 == idx2) {
            idx1 = dist_idxs_mutados(gen);
            idx2 = dist_idxs_mutados(gen);
        }
        std::swap(genes[idx1], genes[idx2]);
    }
}

// Pega os melhores indivíduos de cada conjunto (atual e novo) 
setDeCandidatos determinarNovaPopulacao(const setDeCandidatos& populacao_atual, const setDeCandidatos& novos_candidatos, const uint16_t max_atuais, const uint16_t max_novos) {
    uint16_t atuais_escolhidos = 0;
    uint16_t novos_escolhidos = 0;

    auto it_atuais = populacao_atual.begin();
    auto it_novos = novos_candidatos.begin();
    setDeCandidatos nova_populacao;

    while((atuais_escolhidos < max_atuais && it_atuais != populacao_atual.end()) && (novos_escolhidos < max_novos && it_novos != novos_candidatos.end())) {
        if((*it_atuais).makespan > (*it_novos).makespan) {
            nova_populacao.insert(*it_atuais);
            ++it_atuais;
            ++atuais_escolhidos;
        } else {
            nova_populacao.insert(*it_novos);
            ++it_novos;
            ++novos_escolhidos; 
        }
    }

    while(atuais_escolhidos < max_atuais && it_atuais != populacao_atual.end()) {
        nova_populacao.insert(*it_atuais);
        ++it_atuais;
        ++atuais_escolhidos;
    }

    while(novos_escolhidos < max_novos && it_novos != novos_candidatos.end()) {
        nova_populacao.insert(*it_novos);
        ++it_novos;
        ++novos_escolhidos; 
    }

    return nova_populacao;
}

// Função principal do algoritmo
candidato ADMAG(const uint16_t k, const uint32_t limite_populacional, const uint16_t max_atuais, const uint16_t max_novos, const bool modo_temporal, const uint32_t condicao_de_parada, const std::vector<uint8_t>& solucao_inicial, const int32_t seed) {
    // boilerplate necessário para geração de valores aleatórios
    std::random_device rd;
    std::mt19937 gen( (seed == -1) ? rd() : static_cast<uint32_t>(seed) );

    // Geração da população inicial
    setDeCandidatos populacao(compararCandidatos);
    populacao.emplace(solucao_inicial);
    for(uint32_t i = 1; i < limite_populacional; ++i) {
        populacao.insert(std::move(criarCandidatoAleatorio(solucao_inicial, gen)));
    }
    candidato melhor_solucao = *populacao.begin(); // O primeiro elemento do set é a melhor solução candidata na população atual
    
    //#############################################################################################################################################################################

    // Loop Principal, roda por "condicao_de_parada" iterações se não estamos no modo temporal, ou por "condicao_de_parada" minutos se estivermos
    if(modo_temporal) {
        auto limite_temporal = std::chrono::steady_clock::now() + std::chrono::minutes(condicao_de_parada);
        while(std::chrono::steady_clock::now() < limite_temporal) {
            // seleciona pais
            setDeCandidatos pais = selecionarPais(populacao, k, limite_populacional, gen);
            // gera filhos
            setDeCandidatos filhos = gerarFilhos(pais, gen);
            // determina nova população
            populacao = determinarNovaPopulacao(populacao, filhos, max_atuais, max_novos);
            // Verifica se a melhor solução da nova população é melhor que a já encontrada e a salva se for
            melhor_solucao = std::max<candidato, bool(*)(const candidato& a, const candidato& b)>(melhor_solucao, *populacao.begin(), compararCandidatos); 
        }
    } else {
        for(uint32_t i = 0; i < condicao_de_parada; ++i) {
            // seleciona pais
            setDeCandidatos pais = selecionarPais(populacao, k, limite_populacional, gen);
            // gera filhos
            setDeCandidatos filhos = gerarFilhos(pais, gen);
            // determina nova população
            populacao = determinarNovaPopulacao(populacao, filhos, max_atuais, max_novos);
            // Verifica se a melhor solução da nova população é melhor que a já encontrada e a salva se for
            melhor_solucao = std::max<candidato, bool(*)(const candidato& a, const candidato& b)>(melhor_solucao, *populacao.begin(), compararCandidatos);
        }
    }

    return melhor_solucao;
}