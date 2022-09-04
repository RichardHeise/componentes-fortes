#ifndef _GRAFO_H
#define _GRAFO_H

#include <graphviz/cgraph.h>

//------------------------------------------------------------------------------
typedef Agraph_t *grafo;
typedef Agnode_t *vertice;

// -----------------------------------------------------------------------------
// devolve o grafo lido da entrada padrão (formato dot)

grafo le_grafo(void);

// -----------------------------------------------------------------------------
// desaloca g

void destroi_grafo(grafo g);

// -----------------------------------------------------------------------------
// escreve g na saída padrão em formato dot
// devolve g

grafo escreve_grafo(grafo g);

//------------------------------------------------------------------------------
// acrescenta à lista de subgrafos de g cada um de seus componentes fortes
// vide agsubg(), agfstsubg(), agnxtsubg()
//
// não faz nada se g não for grafo direcionado
//
// devolve g

grafo decompoe(grafo g);


#endif
