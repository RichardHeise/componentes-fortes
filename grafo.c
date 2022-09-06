#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

// Estrutura de vértice para decomposição
typedef struct verticePP_s {
    vertice nodo;
    int estado;
    int componente;
} verticePP;

//------------------------------------------------------------------------------
grafo le_grafo(void) {

    return agread(stdin, NULL); 
}
//------------------------------------------------------------------------------
int destroi_grafo(grafo g) {
  
    agclose(g);
    return 1;
}
//------------------------------------------------------------------------------
grafo escreve_grafo(grafo g) {
  
    return agwrite(g, stdout);
}

//------------------------------------------------------------------------------

static int buscaVetor(vertice vetor[], vertice elem, int tam) {
    for (int i = 0; i < tam; i++) {
        if (vetor[i] == elem) {
            return 1;
        }
    }

    return 0;
}

//------------------------------------------------------------------------------

static int decomp(grafo g, verticePP* vertices, verticePP* r, int c, int tam) {

    // Caso base, se já processamos, retornamos
    if (r->estado == 2) {
        return 0;
    }

    // Processando
    r->estado = 1;

    // DFS
    for (int i = 0; i < tam; i++) {
        if ( agedge(g, r->nodo, vertices[i].nodo, NULL, FALSE) ) {
            
            // Se o estado é não processado
            if (vertices[i].estado == 0) {
                decomp(g, vertices, &vertices[i], c, tam);
            } 
        }
    }

    // r pertence a este componente c
    r->componente = c;

    // r foi processado
    r->estado = 2;

    return 1;
    
}

//------------------------------------------------------------------------------

static void pos_ordem_reversa(grafo g, vertice ver, vertice* visitados, int* tam_v, vertice* pos, int* tam_pos) {

    // Caso base, se já visitamos não fazemos nada
    if (buscaVetor(visitados, ver, *tam_v)) return;

    // Estamos visitando ver
    visitados[*tam_v] = ver;
    *tam_v += 1;

    // DFS
    for (vertice atual = agfstnode(g); atual; atual = agnxtnode(g, atual)) {
        if ( agedge(g, ver, atual, NULL, FALSE) ) {
            pos_ordem_reversa(g, atual, visitados, tam_v, pos, tam_pos);
        }
    }

    // Salva a pós-ordem em pos[]
    pos[*tam_pos] = ver;
    *tam_pos += 1;
}

//------------------------------------------------------------------------------

grafo decompoe(grafo g) {

    // Se o grafo não é direcionado não fazemos nada
    if (!agisdirected(g)) return g;

    // Quantidade de vértices
    int tam = agnnodes(g);

    // Vetor de visitados
    int tam_vis = 0;
    vertice visitados[tam];
    memset(visitados, 0, tam);

    // vetor de pós-ordem
    int tam_pos = 0;
    vertice pos[tam];
    memset(pos, 0, tam);

    // Varremos o grafo fazendo a pós-ordem reversa
    for (vertice atual = agfstnode(g); atual; atual = agnxtnode(g, atual)) {
        pos_ordem_reversa(g, atual, visitados, &tam_vis, pos, &tam_pos);
    }

    // Vetor de vértices com estado e componente
    int tam_vpp = 0;
    verticePP* vertices = malloc(sizeof(verticePP) * tam);

    // Inicialização da estrutura verticePP
    for (int i = 0; i < tam_pos; i++) {
        vertices[tam_vpp].componente = vertices[tam_vpp].estado = 0;
        vertices[tam_vpp++].nodo = pos[i];
    }

    // Não temos componentes fortes ainda
    int n_componentes = 0;

    // Para cada vértice
    for (int i = 0; i < tam_vpp; i++) {

        // fazemos a decomposição em componentes
        if ( decomp(g, vertices, &vertices[i], n_componentes, tam_vpp) ) {

            // Se um componente novo foi criado, precisamos de outro valor para 
            // identificar nosso novo componente
            n_componentes++;
        }
    }

    // Declaração de subgrafos de g
    grafo* subgrafos = malloc(n_componentes * sizeof(grafo));

    // Associação de subgrafos com o grafo g
    for (int i = 0; i < n_componentes; i++){
        subgrafos[i] = agsubg(g, NULL, TRUE);
    }

    // Construindo os subgrafos de g
    if (n_componentes == 1) {

        // Caso g seja trivial
        subgrafos[0] = g;
    } else {
        
        // Caso g não seja trivial
        for (int i = 0; i < tam_vpp; i++) {
            for (int j = 0; j < tam_vpp; j++) {

                // Se existe aresta entre os vértices e são do mesmo componente
                if ( agedge(g, vertices[i].nodo, vertices[j].nodo, NULL, FALSE) && vertices[i].componente == vertices[j].componente) {

                    // Então o subgrafo é alterado de acordo
                    agedge(subgrafos[vertices[i].componente], agnode(g, agnameof(vertices[i].nodo), FALSE), agnode(g, agnameof(vertices[j].nodo), FALSE), NULL, TRUE);
                }
            }
        }
    }

    return g;
}
