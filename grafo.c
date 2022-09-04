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
static void decompAux(grafo g, verticePP* vertices, verticePP r, int c, int tam ) {
    r.estado = 1;
    for (int i = 0; i < tam; i++) {
        if ( agedge(g, r.nodo, vertices[i].nodo, NULL, FALSE)) {
            if (!vertices[i].estado) {
                decompAux(g, vertices, vertices[i], c, tam);
            }
        }
    }
    r.componente = c;
    r.estado = 2;
}

grafo decompoe(grafo g) {

    if (!agisdirected(g)) return;

    int tam = agnnodes(g);

    vertice pilha[tam];
    int j = 0;
    vertice visitados[tam];

    for (vertice atual = agfstnode(g); atual; atual = agnxtnode(g, atual)) {

        int i = 0;

        memset(pilha, 0, sizeof(vertice));
        pilha[i] = atual;
        i++;
        while (i) {

            i--;
            vertice topo = pilha[i];

            if ( !buscaVetor(visitados, topo, j) ) {
                for (vertice ver = agfstnode(g); ver; ver = agnxtnode(g, ver)) {
                    if ( agedge(g, topo, ver, NULL, FALSE)) {
                        
                        if ( !buscaVetor(visitados, ver, j) ) {
                            pilha[i] = ver; 
                            i++;
                        } 
                    }
                }

                visitados[j] = topo;
                j++;

            }
        }
    }

    printf("pos-order?:\n");
    for (int i = 0; i < j; i++) {
        printf("%s ", agnameof(visitados[i]));
    }
    printf("\n");

    verticePP* vertices = malloc(sizeof(verticePP) * tam);

    for (int i = 0; i < tam; i++) {
        vertices[i].nodo = visitados[i];
        vertices[i].componente = vertices[i].estado = 0; 
    }
    int c = 0;

    for (int i = 0; i < tam; i++) {
        if (!vertices[i].estado) {
            decompAux(g, vertices, vertices[i], ++c, tam);
        }
    }


    return g;
}

