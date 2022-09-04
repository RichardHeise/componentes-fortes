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

static void decompAux(grafo g, verticePP* vertices, verticePP* r, int c, int tam) {

    if (r->estado == 2) {
        return;
    }

    r->estado = 1;

    for (int i = 0; i < tam; i++) {
        if ( agedge(g, vertices[i].nodo, r->nodo , NULL, FALSE) ) {

            if (vertices[i].estado == 0) {
                decompAux(g, vertices, &vertices[i], c, tam);
            } 
        }
    }

    r->componente = c;
    r->estado = 2;
    
}

//------------------------------------------------------------------------------

grafo decompoe(grafo g) {

    if (!agisdirected(g)) return;

    int tam = agnnodes(g);

    vertice pilha[tam];

    int j = 0;
    vertice visitados[tam];
    int k;
    vertice pos[tam];

    for (vertice atual = agfstnode(g); atual; atual = agnxtnode(g, atual)) {

        int i = 0;
        
        memset(pos, 0, sizeof(vertice));
        k = 0;
        
        memset(pilha, 0, sizeof(vertice));
        pilha[i] = atual;
        i++;

        while (i) {

            i--;
            vertice topo = pilha[i];

            if ( !buscaVetor(visitados, topo, j) ) {

                for (vertice ver = agfstnode(g); ver; ver = agnxtnode(g, ver)) {
                    if ( agedge(g, topo, ver, NULL, FALSE) ) {
                        
                        if ( !buscaVetor(visitados, ver, j) ) {
                            pilha[i] = ver; 
                            i++;
                        } 
                    }
                }

                pos[k++] = topo;
                visitados[j] = topo;
                j++;
            }
        }

    }

    printf("pos-order?:\n");
    for (int i = 0; i < tam; i++) {
        printf("%s ", agnameof(pos[i]));
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
            c += 1;
            decompAux(g, vertices, &vertices[i], c, tam);
        }
    }

    for (int i = 0; i < tam; i++) {
        printf("vértice: %s, estado: %d, componente: %d\n", agnameof(vertices[i].nodo), vertices[i].estado, vertices[i].componente);
    }

    return g;
}

