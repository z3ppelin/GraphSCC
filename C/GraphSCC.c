/**
 * Calculate Strongly Connected Components of a Directed Graph.
 * in O(n + m), n = number of vertices, m = number of edges.
 *
 * @author      Bogdan Constantinescu <bog_con@yahoo.com>
 * @since       2013.08.03
 * @version     1.0
 * @link        GitHub    https://github.com/z3ppelin/GraphSCC
 * @licence     The MIT License (http://opensource.org/licenses/MIT); see LICENCE.txt
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "GraphSCC.h"

// global variables, keep them so and don't use parameterized functions cause
// for large input memory overflow problems will appear.
int sccCount = 0;
int *visited, *finishingTimes, *finishingTimesOrdered, *sccComponentsCount;
Graph graph, graphRev;

/**
 * Main function. Reads graph, calculates SCCs and prints the result.
 * @param     argc       Command line arguments no.
 * @param     argv       Command line arguments.
 * @return    int        Success/error code. (0 is a success, anything else is error).
 */
int main(int argc, char** argv) {
	printf("------ Begin Graph Strongly Connected Components ------\n");
	clock_t start, end;
	char error[128];
	int i, t = 0;

	/* read directed graph */
	start = clock();
	if (argc == 1) {
		err("Err. The input file must be given as an argument.\n");
	}
	if (!readGraphFromFile(argv[1], error)) {
		err(error);
	}
	/* initialize other vars */
	visited = (int*) malloc(sizeof(int) * graph.n);
	finishingTimes = (int*) malloc(sizeof(int) * graph.n);
	finishingTimesOrdered = (int*) malloc(sizeof(int) * graph.n);
	sccComponentsCount = (int*) malloc(sizeof(int) * graph.n);
	for (i = 0; i < graph.n; i++) {
		visited[i] = 0;
		finishingTimes[i] = -1;
		finishingTimesOrdered[i] = -1;
		sccComponentsCount[i] = 0;
	}
	end = clock();
	printf("Elapsed: %f seconds with initializations, reading graph.\n",
			(double) (end - start) / CLOCKS_PER_SEC);

	/* print read graphs */
	//printf("The read graph:\n");
	//printGraph(graph);
	//printf("\n");
	//printf("The reversed graph:\n");
	//printGraph(graphRev);
	//printf("\n");
	
	/* start Kosaraju's algorithm */
	start = clock();
	/* calculate finishing times of the reversed graph */
	DFSLoop1(&t);
	/* free memory ocupied by reveresed graph */
	freeGraph(&graphRev);
	/* reset visited */
	for (i = 0; i < graph.n; i++) {
		visited[i] = 0;
	}
	/* calculate how many SCCs are, and their components number */
	DFSLoop2();
	end = clock();

	printf("Found %d SCC(s).\n", sccCount);
	printf("Elapsed: %f seconds with the two DFSs.\n",
			(double) (end - start) / CLOCKS_PER_SEC);

	/* free left allocated memory */
	freeGraph(&graph);
	free(visited);
	free(finishingTimes);
	free(finishingTimesOrdered);
	free(sccComponentsCount);

	printf("------- End Graph Strongly Connected Components -------\n");
	return EXIT_SUCCESS;
}

/**
 * Depth first search routine for the first DFS loop.
 * @param      vertex      The new visited vertex.
 * @param      t           Pointer to current finishing time.
 */
void DFS1(const int vertex, int* t) {
	Node* node = graphRev.edges[vertex];
	visited[vertex] = 1;
	while (NULL != node) {
		if (!visited[node->vertex]) {
			DFS1(node->vertex, t);
		}
		node = node->next;
	}
	finishingTimesOrdered[*t] = vertex;
	finishingTimes[vertex] = *t;
	*t = *t + 1;
}

/**
 * Calculates finishing time for each vertex. It operates on the graph reversed.
 * @param      t           Pointer to current finishing time.
 */
void DFSLoop1(int* t) {
	int i;
	for (i = graph.n - 1; i >= 0; i--) {
		if (!visited[i]) {
			DFS1(i, t);
		}
	}
}

/**
 * Depth first search routine for second DFS loop.
 * @param      vertex            The new visited vertex.
 * @param      startVertex      The start vertex, nodes with same start vertex are in the same SCC.
 */
void DFS2(const int vertex, const int startVertex) {
	Node* node = graph.edges[vertex];
	visited[finishingTimes[vertex]] = 1;
	sccComponentsCount[finishingTimesOrdered[startVertex]] += 1;
	while (NULL != node) {
		if (!visited[finishingTimes[node->vertex]]) {
			node->startingVertex = finishingTimesOrdered[startVertex];
			DFS2(node->vertex, startVertex);
		}
		node = node->next;
	}
}

/**
 * Calculates number of SCCs and counts for each the number of vertices it has.
 * It operates on the original graph but with vertices changed with their finishing time.
 */
void DFSLoop2() {
	int i;
	for (i = graph.n - 1; i >= 0; i--) {
		if (!visited[i]) {
			sccCount++;
			DFS2(finishingTimesOrdered[i], i);
		}
	}
}

/**
 * Reads graph from file.
 * @param      file      The file where to read graph from.
 * @param      err       An error message, if any occcurred during reading.
 * @return               1 if everything went fine, 0 otherwise.
 */
int readGraphFromFile(char* file, char* err) {
	FILE *inputFile = fopen(file, "rt");
	int n, m = 0, i, tailVertex, headVertex;
	Node** edges, **edgesRev;
	char buffer[10000];
	Node *node, *nodeRev;

	if (NULL == inputFile) {
		strcpy(err, "Err. Could not open file.\n");
		return 0;
	}
	if (NULL == fgets(buffer, sizeof buffer, inputFile)
			|| 1 != sscanf(buffer, "%d", &n)) {
		fclose(inputFile);
		strcpy(err, "Err. Could not read number of vertices.\n");
		return 0;
	}
	edges = (Node**) malloc(sizeof(Node*) * n);
	edgesRev = (Node**) malloc(sizeof(Node*) * n);
	for (i = 0; i < n; i++) {
		edges[i] = edgesRev[i] = NULL;
	}

	while (!feof(inputFile)) {
		if (fscanf(inputFile, "%d", &tailVertex) != 1 || tailVertex < 1
				|| tailVertex > n) {
			break;
		}
		tailVertex--;
		if (fscanf(inputFile, "%d", &headVertex) != 1 || headVertex < 1
				|| headVertex > n) {
			break;
		}
		headVertex--;

		node = (Node*) malloc(sizeof(Node));
		node->vertex = headVertex;
		if (NULL == edges[tailVertex]) {
			node->next = NULL;
		} else {
			node->next = edges[tailVertex];
		}
		edges[tailVertex] = node;

		nodeRev = (Node*) malloc(sizeof(Node));
		nodeRev->vertex = tailVertex;
		if (NULL == edgesRev[headVertex]) {
			nodeRev->next = NULL;
		} else {
			nodeRev->next = edgesRev[headVertex];
		}
		edgesRev[headVertex] = nodeRev;

		m++;
	}
	fclose(inputFile);

	graph.n = graphRev.n = n;
	graph.m = graphRev.m = m;
	graph.edges = edges;
	graphRev.edges = edgesRev;
	return 1;
}

/**
 * Prints graph.
 * @param   graph   The graph to print.
 */
void printGraph(const Graph graph) {
	printf("Graph has %d vertices and %d arc(s).\n", graph.n, graph.m);
	Node* node;
	int i;
	for (i = 0; i < graph.n; i++) {
		printf("Vertex %d has arc(s) with: ", i + 1);
		node = graph.edges[i];
		if (NULL == node) {
			printf("nobody");
		} else {
			while (NULL != node) {
				printf("%d ", node->vertex + 1);
				node = node->next;
			}
		}
		printf("\n");
	}
}

/**
 * Prints error and exits program.
 * @param    msg    The error to print.
 */
void err(const char* msg) {
	printf(msg);
	printf("------- End Graph Strongly Connected Components -------\n");
	exit(EXIT_FAILURE);
}

/**
 * Frees memory occupied by pointer members of a graph.
 * @param   graph    The graph to free pointer members.
 */
void freeGraph(Graph* graph) {
	Node *node1, *node2;
	int i;
	for (i = 0; i < graph->n; i++) {
		node1 = graph->edges[i];
		while (NULL != node1) {
			node2 = node1;
			node1 = node1->next;
			free(node2);
		}
	}
	free(graph->edges);
}
