#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Graph {
	public:
		vector<vector<string>> adjacency_matrix;

		Graph(int tam) {
			vector<string> line;
			line.assign(tam, "0");
			this->adjacency_matrix.assign(tam, line);
		}

		void addVertex(int src, int dst, string value) {
			this->adjacency_matrix[src-1][dst-1] = value;
		}

		/**
		 * 
		 * Removo o grafico que as posições está marcado para ser deletado
		*/
		void remove() {
			for(int line = this->adjacency_matrix.size() - 1; line >= 0; line--) {
				auto it = this->vertexDeletedSet.find(line);

				if(this->isVertexDeletedTrack(line))
				{
					this->adjacency_matrix.erase(this->adjacency_matrix.begin() + line);
					cout << "Deleted " << endl;
				}
			}

			this->vertexDeletedSet.clear();
		}

		void print() {
			cout <<  "=== MATRIZ ADJACENCIA ====" << endl;
			for(int line = 0; line < this->adjacency_matrix.size(); line++) {
				cout << "pos " << line + 1 << ": ";
				for(auto column: this->adjacency_matrix[line]) {
					cout << "\t" << column << " ";
				}
				cout << endl;
			}
		}

		Graph copy() {
			Graph new_graph(this->adjacency_matrix.size());

			for(int i = 0; i < this->adjacency_matrix.size(); i++) {
				for(int j = 0; j < this->adjacency_matrix.size(); j++) {
					new_graph.addVertex(i+1, j+1, this->adjacency_matrix[i][j]);
				}
			}

			return new_graph;
		}

		bool isEmpty(Graph* graph) {
			bool is_empty = true;

			for(auto i: graph->adjacency_matrix) {
				for(auto j: i) {
					is_empty = is_empty && j == "0";
				}
			}

			return is_empty;
		}

		int min_pos(Graph* graph) {
			int pos_min_vertex;
			int min_degree;

			for(int line = 0; line < graph->adjacency_matrix.size(); line++) {
				int tot_degree = 0;
				for(auto column: graph->adjacency_matrix[line]) {
					if(column == "1") tot_degree++;
				}

				if(line == 0 || tot_degree < min_degree) {
					min_degree = tot_degree;
					pos_min_vertex = line;
				}
			}

			return pos_min_vertex;
		}

		void addNeighborDelete(Graph* graph, int min_degree) {
			for(int i = 0; i < graph->adjacency_matrix.size(); i++) {
				if(graph->adjacency_matrix[min_degree][i] == "1") {
					graph->addVertexDelete(i);
				}
			}
		}

		void addVertexDegreeMinDelete(Graph* graph, int min_degree)
		{
			graph->addVertexDelete(min_degree);
		}

		vector<int> mis() {
			vector<int> independent_set;
			Graph graph = this->copy();

			while(!isEmpty(&graph)) {
				int vertex_min_degree_position = min_pos(&graph);

				independent_set.push_back(vertex_min_degree_position);

				graph.addNeighborDelete(&graph, vertex_min_degree_position);
				graph.addVertexDegreeMinDelete(&graph, vertex_min_degree_position);
				graph.remove();
			}

			graph.print();

			return independent_set;
		}
		
		private:
			set<int> vertexDeletedSet;
			
			void addVertexDelete(int vertex)
			{
				vertexDeletedSet.insert(vertex);
			}

			bool isVertexDeletedTrack(int vertex_search){
				for(auto element: vertexDeletedSet) {
					if(vertex_search == element) return true;
				}
				return false;
			}

};


/** HEADER */
void teste1(Graph* graph);

int main() {
	Graph graph(6);

	teste1(&graph);

	vector<int> i = graph.mis();

	for(auto t: i) {
		printf("%d\n", t + 1);
	}

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
	graph->addVertex(6, 3, "1");
	graph->addVertex(6, 4, "1");
	graph->addVertex(6, 5, "1");
	graph->addVertex(6, 6, "0");
}