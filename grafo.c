#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

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

    if (r->estado == 2) {
        return 0;
    }

    r->estado = 1;

    for (int i = 0; i < tam; i++) {
        if ( agedge(g, r->nodo, vertices[i].nodo, NULL, FALSE) ) {

            if (vertices[i].estado == 0) {
                decomp(g, vertices, &vertices[i], c, tam);
            } 
        }
    }

    r->componente = c;
    r->estado = 2;

    return 1;
    
}

//------------------------------------------------------------------------------

static void pos_ordem_reversa(grafo g, vertice ver, vertice* visitados, int* tam_v, vertice* pos, int* tam_pos) {

    if (buscaVetor(visitados, ver, *tam_v)) return;

    visitados[*tam_v] = ver;
    *tam_v += 1;

    for (vertice atual = agfstnode(g); atual; atual = agnxtnode(g, atual)) {
        if ( agedge(g, ver, atual, NULL, FALSE) ) {
            pos_ordem_reversa(g, atual, visitados, tam_v, pos, tam_pos);
        }
    }
    pos[*tam_pos] = ver;
    *tam_pos += 1;
}

//------------------------------------------------------------------------------

vertice *reverte_ordem(vertice *pos, int tam_pos) {

    vertice* reverso = malloc(sizeof(vertice) * tam_pos);

    for (int i = 0; i < tam_pos; i++)
        reverso[i] = pos[tam_pos-1 - i];

    return reverso;
}

//------------------------------------------------------------------------------

grafo decompoe(grafo g) {

    if (!agisdirected(g)) return;

    int tam = agnnodes(g);

    int tam_vis = 0;
    vertice visitados[tam];
    memset(visitados, 0, tam);

    int tam_pos = 0;
    vertice pos[tam];
    memset(pos, 0, tam);

    for (vertice atual = agfstnode(g); atual; atual = agnxtnode(g, atual)) {
        pos_ordem_reversa(g, atual, visitados, &tam_vis, pos, &tam_pos);
    }

    vertice* revertido = reverte_ordem(pos, tam_pos);
    verticePP* vertices = malloc(sizeof(verticePP) * tam);

    int tam_vpp = 0;
    for (int i = 0; i < tam_pos; i++) {
        vertices[tam_vpp].componente = vertices[tam_vpp].estado = 0;
        vertices[tam_vpp++].nodo = pos[i];
    }

    int n_componentes = 0;
    for (int i = 0; i < tam_vpp; i++) {
        if ( decomp(g, vertices, &vertices[i], n_componentes, tam_vpp) ) {
            n_componentes++;
        }
    }

    grafo* subgrafos = malloc(n_componentes * sizeof(grafo));

    for (int i = 0; i < n_componentes; i++){
        subgrafos[i] = agsubg(g, NULL, TRUE);
    }

    if (n_componentes == 1) {
        subgrafos[0] = g;
    } else {
        for (int i = 0; i < tam_vpp; i++) {
            for (int j = 0; j < tam_vpp; j++) {
                if ( agedge(g, vertices[i].nodo, vertices[j].nodo, NULL, FALSE) && vertices[i].componente == vertices[j].componente) {
                    agedge(subgrafos[vertices[i].componente], agnode(g, agnameof(vertices[i].nodo), FALSE), agnode(g, agnameof(vertices[j].nodo), FALSE), NULL, TRUE);
                }
            }
        }

    }

    return g;
}
