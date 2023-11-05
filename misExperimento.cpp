#include <iostream>
#include <vector>
#include <set>
#define TAM_BUFFER_READ 1000

using namespace std;

class Graph {
	public:
		vector<vector<string>> adjacency_matrix;
		vector<int> independent_set;

		void createAdjacenteMatrix(int tam) {
			vector<string> line;
			line.assign(tam, "0");
			this->adjacency_matrix.assign(tam, line);
		}

		void addVertex(int src, int dst, string value) {
			this->adjacency_matrix[src-1][dst-1] = value;
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
			cout << "============================" << endl;
		}

		Graph copy() {
			Graph new_graph;
			new_graph.createAdjacenteMatrix(this->adjacency_matrix.size());

			for(int i = 0; i < this->adjacency_matrix.size(); i++) {
				for(int j = 0; j < this->adjacency_matrix.size(); j++) {
					new_graph.addVertex(i+1, j+1, this->adjacency_matrix[i][j]);
				}
			}

			return new_graph;
		}

		bool isEmpty() {
			bool is_empty = true;
			for(int line = 0; line < this->adjacency_matrix.size(); line++) {
				is_empty = is_empty && this->isVertexIguinor(line);
			}
			return is_empty;
		}

		int min_pos() {
			int pos_min_vertex;
			int min_degree;

			for(int line = 0; line < this->adjacency_matrix.size(); line++) {

				if(this->isVertexIguinor(line)) continue;
				
				int tot_degree = 0;
				for(auto column: this->adjacency_matrix[line]) {
					if(column == "1") tot_degree++;
				}
				
				bool is_min = line == 0 || tot_degree < min_degree;
				if(is_min) {
					min_degree = tot_degree;
					pos_min_vertex = line;
				}
			}

			return pos_min_vertex;
		}

		void addNeighborDelete(int min_degree) {
			for(int i = 0; i < this->adjacency_matrix.size(); i++) {
				
				if(this->adjacency_matrix[min_degree][i] == "1" && i != min_degree) {
					this->addVertexDelete(i);
				}
			}
		}

		void addVertexDegreeMinDelete(int min_degree)
		{
			this->addVertexDelete(min_degree);
		}

		Graph mis() {
			Graph graph = this->copy();
			while(!graph.isEmpty()) {
				int vertex_min_degree_position = graph.min_pos();
				graph.independent_set.push_back(vertex_min_degree_position);
				graph.addNeighborDelete(vertex_min_degree_position);
				graph.addVertexDegreeMinDelete(vertex_min_degree_position);
			}
			return graph;
		}

		void showResult() {
			cout << "CONJUNTO MAXIMAL: [" << this->independent_set.size() << "]" << endl;
			cout << "{ ";
			for(auto i: this->independent_set) {
				cout << i << " ";
			}
			cout << "}" << endl << endl;
		}
		
		private:
			set<int> vertexDeletedSet;
			
			void addVertexDelete(int vertex)
			{
				vertexDeletedSet.insert(vertex);
			}

			bool isVertexIguinor(int vertex_search){
				for(auto element: vertexDeletedSet) {
					if(vertex_search == element) return true;
				}
				return false;
			}
};

class FileHandler {
	public:
	Graph graph;

	FileHandler(Graph graph) {
		this->graph = graph;
	}

	int getColumnTotal(char* buffer) 
	{
		const char delimiter[2] = " ";
		char* token;
		char newBuffer[TAM_BUFFER_READ];
		
		strcpy(newBuffer, buffer);
		
		/** Obter o primeiro token */
		token = strtok(newBuffer, delimiter);
		
		/** Obter o segundo token */
		int pos = 0;
		while(token != NULL) 
		{
			token = strtok(NULL, delimiter);
			pos++;
		}
		
		return pos;
	}

	void tokenize(char* buffer, int pos_line)
	{
		const char delimter[2] = " ";
		char* token;
		
		/** Obter o primeiro token */
		token = strtok(buffer, delimter);
		
		/** Obter os restantes dos tokens */
		int pos_column = 0;
		while(token != NULL) 
		{
			int last_posicional = strlen(token) - 1;
			bool is_new_break_scape = token[last_posicional] == '\n';
			if(is_new_break_scape) 
			{
				token[last_posicional] = '\0';
			}

			this->graph.addVertex(pos_line+1, pos_column+1, token);
			pos_column++;
			token = strtok(NULL, delimter);
		}
	}

	Graph read(string file_source) 
	{
		FILE* fptr = fopen(file_source.c_str(), "r");
		
		if(fptr == NULL) {
			cout << "<< ERROR! Arquivo nao encontrado! >> " << endl;
			exit(100);
		}

		char buffer[TAM_BUFFER_READ];
		int totalLine = 0;
		while(fgets(buffer, TAM_BUFFER_READ, fptr)) 
		{
			bool is_first_buffer_catch = totalLine == 0;
			if(is_first_buffer_catch) 
			{
				int totalColumn = this->getColumnTotal(buffer);
				cout << totalColumn << endl;
				this->graph.createAdjacenteMatrix(totalColumn);
			}

			this->tokenize(buffer, totalLine);

			totalLine++;
		}

		return this->graph;
	}
};

int main() {
	Graph graph;
	FileHandler file(graph);

	graph = file.read("tmp/teste01.txt");
	graph.print();
	graph.mis().showResult();

	return 0;
}