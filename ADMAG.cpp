#include "ADMAG.hpp"
#include "candidato.hpp"

// Cria um novo candidato  com uma permutação aleatória de tarefas
candidato criarCandidatoAleatorio(const std::vector<uint8_t>& p, std::mt19937& gen) {
    std::vector<uint8_t> new_p(p);
    // mistura os valores do vetor aleatoriamente
    std::shuffle(new_p.begin(), new_p.end(), gen);
    candidato novo_candidato(new_p);
    return novo_candidato;
}

// Função que seleciona os pais que irão reproduzir para gerar os novos candidatos da próxima população
setDeCandidatos selecionarPais(const setDeCandidatos& populacao_atual, const float_t p, std::mt19937& gen) {
    setDeCandidatos pais(compararCandidatos);
    std::uniform_real_distribution<float_t> dist(0.0f, 1.0f);
    const float_t inverse_p = 1.0f-p;
    while(pais.size() < 2) {
        float_t modulador_do_p = 1.0f;
        for(auto& reprodutor : populacao_atual) {
            const float_t prob = p*modulador_do_p;
            if(dist(gen) < prob) {
                pais.insert(reprodutor);
            }
            modulador_do_p *= inverse_p;
        } 
    }
    return pais;
}

// Gera os filhos dessa geração a partir dos pais selecionados
// Para k pais selecionados, serão gerados (2^k)-1 filhos
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
        uint8_t idx1 = dist_idxs_mutados(gen);
        uint8_t idx2 = dist_idxs_mutados(gen);
        while(idx1 == idx2) {
            idx1 = dist_idxs_mutados(gen);
            idx2 = dist_idxs_mutados(gen);
        }
        std::swap(genes[idx1], genes[idx2]);
    }
}

setDeCandidatos determinarNovaPopulacao(const setDeCandidatos& populacao_atual, const setDeCandidatos& novos_candidatos, const uint16_t max_atuais, const uint16_t max_novos) {
    uint16_t atuais_escolhidos = 0;
    uint16_t novos_escolhidos = 0;

    auto it_atuais = populacao_atual.begin();
    auto it_novos = novos_candidatos.begin();
    setDeCandidatos nova_populacao;

    while((atuais_escolhidos < max_atuais || it_atuais != populacao_atual.end()) && (novos_escolhidos < max_novos || it_novos != novos_candidatos.end())) {
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

    while(atuais_escolhidos < max_atuais || it_atuais != populacao_atual.end()) {
        nova_populacao.insert(*it_atuais);
        ++it_atuais;
        ++atuais_escolhidos;
    }

    while(novos_escolhidos < max_novos || it_novos != novos_candidatos.end()) {
        nova_populacao.insert(*it_novos);
        ++it_novos;
        ++novos_escolhidos; 
    }

    return nova_populacao;
}

// Função principal do algoritmo
candidato ADMAG(const uint32_t tam_pop_inicial, const std::vector<uint8_t>& pop_inicial, const uint16_t p, const uint16_t max_atuais, const uint16_t max_novos, const uint32_t max_iter) {
    const uint8_t max_mutacoes = static_cast<uint8_t>(tarefas/10);
    // boilerplate necessário para geração de valores aleatórios
    std::random_device rd;
    std::mt19937 gen(rd);

    // Geração da população inicial
    //#############################################################################################################################################################################
    setDeCandidatos populacao(compararCandidatos);
    for(uint32_t i = 0; i < tam_pop_inicial; ++i) {
        populacao.insert(std::move(criarCandidatoAleatorio(pop_inicial, gen)));
    }
    candidato melhor_solucao = *populacao.begin(); // O primeiro elemento do set é a melhor solução candidata na população atual
    //#############################################################################################################################################################################

    // Loop Principal, roda por max_iter iterações
    for(uint32_t i = 0; i < max_iter; ++i) {
        // seleciona pais
        setDeCandidatos pais = selecionarPais(populacao, p, gen);
        // gera filhos
        setDeCandidatos filhos = gerarFilhos(pais, gen);
        // determina nova população
        populacao = determinarNovaPopulacao(populacao, filhos, max_atuais, max_novos);
        // Verifica se a melhor solução da nova população é melhor que a já encontrada
        melhor_solucao = std::max<candidato, bool(*)(const candidato& a, const candidato& b)>(melhor_solucao, *populacao.begin(), compararCandidatos);
    }

    return melhor_solucao;
}