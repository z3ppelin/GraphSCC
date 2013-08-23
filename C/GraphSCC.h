/**
 * Header file for Graph Strongly Connected Components.
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.08.03
 * @version     1.0
 * @link        GitHub    https://github.com/z3ppelin/GraphSCC
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */

#ifndef _GARPH_SCC_H_
#define _GARPH_SCC_H_

struct Node {
	int vertex;
	struct Node* next;
	int startingVertex;
};
typedef struct Node Node;

struct Graph {
	int n;
	int m;
	struct Node** edges;
};
typedef struct Graph Graph;

/* function prototypes */
void DFS1(const int, int*);
void DFSLoop1(int*);
void DFS2(const int, const int);
void DFSLoop2();
void err(const char*);
int readGraphFromFile(char*, char*);
void printGraph(const Graph);
void freeGraph(Graph*);

#endif /* _GARPH_SCC_H_ */
