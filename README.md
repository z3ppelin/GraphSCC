Kosaraju's algorithm for finding strongly-connected components
===================

About
------------
An implementation of Kosaraju's algorithm for finding strongly-connected 
components (SCCs) of a graph in linear time.  While Kosaraju's algorithm is
not the fastest known algorithm for computing SCCs (both Tarjan's algorithm
and Gabow's algorithm are faster by roughly a factor of two), it is a
remarkably elegant algorithm with asymptotic behavior identical to the other
algorithms.

Kosaraju's algorithm relies on several important properties of graphs.
First, any graph can be decomposed into a directed acyclic graph (DAG) of
SCCs.  This yields an interesting observation.  Suppose that we start in a
node in an SCC that's a sink node in this DAG and perform a depth-first
search.  Because the SCC is a sink in the DAG, there are no outgoing edges
from it, and so every node we encounter we must be in the same SCC.  If we
then remove all of these nodes and repeat, we can repeatedly find sink SCCs,
run depth-first searches in them, and then recover all the nodes in the SCC.

The challenge is how we can find which SCCs are sinks in the DAG.  For this,
Kosaraju's algorithm uses a clever trick.  Suppose that we run a full depth-
first search on the nodes in the graph, recording each node on a stack when
we finish expanding it.  Then we know that the topmost node of this stack
must be in a source SCC, since if it weren't, then some node in an SCC above
it would be have been expanded before it.  More generally, if two nodes u
and v are on the stack with u deeper than v, then the SCC containing v is
no lower than the SCC containing u.  This means that if we run this DFS in
the graph and then explore the nodes in the order in which they come back
off the stack, we'll explore the DAG's sources one after the other.

Of course, this is backwards - we want to explore the sinks, not the
sources!  But this isn't a problem.  If we consider the reverse graph of
the original graph (formed by directing each arc the opposite of its normal
direction), then we don't change the SCCs.  You can see this by realizing
that two nodes in an SCC have two paths between them - one forward and one
backward - and so in the reverse graph both of these paths will still exist,
albeit swapped with one another.  However, reversing the edges does change
the DAG of SCCs in the graph by inverting all of the arcs in it.  This means
that in the reverse graph source SCCs become sink SCCs and vice-versa.
The net result of this is that if we run a full DFS in the reverse graph,
then run a depth-first search from each node in the graph in the reverse
order in which they are visited, we will end up recovering all the SCCs.

Running code examples
------------
**C** implementation:

You should compile the source file first.

    cd C/
    gcc -o GraphSCC GraphSCC.c (Linux)
    ./GraphSCC ../in/inputBig.txt (Linux)
    
    compile with Visual Studio | MinGW | DevC++ (Windows)
    GraphSCC.exe ../in/inputBig.txt (Windows)

For the input files in *in/* folder the expected results are: *inputBig.txt*: 371762, *inputSmall.txt*: 6, *inputTiny.txt*: 3