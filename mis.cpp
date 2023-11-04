#include <iostream>
#include <vector>

using namespace std;

class Graph {
    public:
    vector<vector<string>> adjacency_matrix;

    Graph(int tam)
    {
        vector<string> line;
        line.assign(tam, "0");
        this->adjacency_matrix.assign(tam, line);
    }

    void addVertex(int src, int dst, string value) 
    {
       this->adjacency_matrix[src-1][dst-1] = value;
    }

    void removeVertex(int src, int dst) 
    {
        this->adjacency_matrix[src][dst] = "0";
    }

    void print()
    {
        cout <<  "=== MATRIZ ADJACENCIA ====" << endl;
        for(auto line: adjacency_matrix) 
        {
            for(auto column: line) 
            {
                cout << "\t" << column << " ";
            }
            cout << endl;
        }
    }

};


/** HEADER */
void teste1(Graph* graph);

int main()
{
    Graph graph(6);

    teste1(&graph);

    graph.print();

    return 0;
}

void teste1(Graph* graph) {
    graph->addVertex(1, 1, "0");
    graph->addVertex(1, 2, "1");
    graph->addVertex(1, 3, "1");
    graph->addVertex(1, 4, "0");
    graph->addVertex(1, 5, "1");
    graph->addVertex(1, 6, "0");

    graph->addVertex(2, 1, "1");
    graph->addVertex(2, 2, "0");
    graph->addVertex(2, 3, "0");
    graph->addVertex(2, 4, "0");
    graph->addVertex(2, 5, "1");
    graph->addVertex(2, 6, "0");

    graph->addVertex(3, 1, "1");
    graph->addVertex(3, 2, "0");
    graph->addVertex(3, 3, "0");
    graph->addVertex(3, 4, "0");
    graph->addVertex(3, 5, "1");
    graph->addVertex(3, 6, "1");

    graph->addVertex(4, 1, "0");
    graph->addVertex(4, 2, "0");
    graph->addVertex(4, 3, "0");
    graph->addVertex(4, 4, "0");
    graph->addVertex(4, 5, "0");
    graph->addVertex(4, 6, "1");

    graph->addVertex(5, 1, "1");
    graph->addVertex(5, 2, "1");
    graph->addVertex(5, 3, "1");
    graph->addVertex(5, 4, "0");
    graph->addVertex(5, 5, "0");
    graph->addVertex(5, 6, "1");

    graph->addVertex(6, 1, "0");
    graph->addVertex(6, 2, "0");
    graph->addVertex(6, 3, "0");
    graph->addVertex(6, 4, "1");
    graph->addVertex(6, 5, "1");
    graph->addVertex(6, 6, "0");
}