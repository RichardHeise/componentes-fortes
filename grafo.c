#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------
grafo le_grafo(void) {

    return agread(stdin, NULL); 
}
//------------------------------------------------------------------------------
void destroi_grafo(grafo g) {
  
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

grafo decompoe(grafo g) {

    if (!agisdirected(g)) return;

    int tam = agnnodes(g);

    vertice pilha[tam];
    int j = 0;
    vertice visitados[tam];
    int k = 0;
    vertice pos[tam];
    pos[k++] = agfstnode(g);

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
                            pos[k++] = ver;
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

    for (int i = 0; i < j; i++) {
        printf("%s ", agnameof(visitados[i]));
    }


    return g;
}

