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
			this->adjacency_matrix[src][dst] = value;
		}

		void print() {
			cout <<  "=== MATRIZ ADJACENCIA ====" << endl;
			for(int line = 0; line < this->adjacency_matrix.size(); line++) {
				cout << " V" << line + 1 << ": ";
				for(auto column: this->adjacency_matrix[line]) {
					cout << "[ " << column << " ] ";
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
					new_graph.addVertex(i, j, this->adjacency_matrix[i][j]);
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

		int minimusVertexPosition() {
			int pos_min_vertex;
			int minimus_degree;

			for(int line = 0; line < this->adjacency_matrix.size(); line++) {

				if(this->isVertexIguinor(line)) continue;

				int tot_degree = 0;
				for(auto column: this->adjacency_matrix[line]) {
					if(column == "1") tot_degree++;
				}

				bool is_min = line == 0 || tot_degree < minimus_degree;
				if(is_min) {
					minimus_degree = tot_degree;
					pos_min_vertex = line;
				}
			}

			return pos_min_vertex;
		}

		void addNeighborStatusDeleted(int minimus_vertex_degree_position) {
			for(int vertexNeighborPos = 0; vertexNeighborPos < this->adjacency_matrix.size(); vertexNeighborPos++) {

				if(
					this->adjacency_matrix[minimus_vertex_degree_position][vertexNeighborPos] == "1" && 
					vertexNeighborPos != minimus_vertex_degree_position) 
				{
					this->addVertexDeleted(vertexNeighborPos);
				}
			}
		}

		void addVertexDegreeMinStatusDeleted(int minimus_vertex_degree_position) {
			this->addVertexDeleted(minimus_vertex_degree_position);
		}

		Graph mis() {
			Graph graph = this->copy();
			while(!graph.isEmpty()) {
				int vertex_min_degree_position = graph.minimusVertexPosition();
				graph.independent_set.push_back(vertex_min_degree_position);
				graph.addNeighborStatusDeleted(vertex_min_degree_position);
				graph.addVertexDegreeMinStatusDeleted(vertex_min_degree_position);
			}
			return graph;
		}

		void showResult(bool is_began_with_zero_vertex = false) {
			cout << "SUBCONJUNTO INTERNAMENTE ESTAVEL: TAM |" << this->independent_set.size() << "| = ";
			cout << "{ ";
		
			for(int pos = 0; pos < this->independent_set.size(); pos++) {
				auto vertex = this->independent_set[pos];

				cout << "V" << ( is_began_with_zero_vertex ? vertex : vertex + 1 );
				cout << ( pos < this->independent_set.size() - 1 ? ", " : "" );
			}
			cout << " }" << endl << endl;
		}

	private:
		set<int> vertexDeletedSet;

		void addVertexDeleted(int vertex) {
			vertexDeletedSet.insert(vertex);
		}

		bool isVertexIguinor(int vertex_search) {
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

		int getColumnTotal(char* buffer) {
			const char delimiter[2] = " ";
			char* token;
			char newBuffer[TAM_BUFFER_READ];

			strcpy(newBuffer, buffer);

			/** Obter o primeiro token */
			token = strtok(newBuffer, delimiter);

			/** Obter o segundo token */
			int pos = 0;
			while(token != NULL) {
				token = strtok(NULL, delimiter);
				pos++;
			}

			return pos;
		}

		void tokenize(char* buffer, int pos_line) {
			const char delimter[2] = " ";
			char* token;
			char newBuffer[TAM_BUFFER_READ];

			strcpy(newBuffer, buffer);

			/** Obter o primeiro token */
			token = strtok(newBuffer, delimter);

			/** Obter os restantes dos tokens */
			int pos_column = 0;
			while(token != NULL) {
				int last_posicional = strlen(token) - 1;
				bool is_new_break_scape = token[last_posicional] == '\n';
				if(is_new_break_scape) {
					token[last_posicional] = '\0';
				}

				this->graph.addVertex(pos_line, pos_column, token);
				pos_column++;
				token = strtok(NULL, delimter);
			}
		}

		Graph read(string file_source) {
			FILE* fptr = fopen(file_source.c_str(), "r");

			if(fptr == NULL) {
				cout << "<< ERROR! Arquivo nao encontrado! >> " << endl;
				exit(100);
			}

			char buffer[TAM_BUFFER_READ];
			int totalLine = 0;
			while(fgets(buffer, TAM_BUFFER_READ, fptr)) {

				bool is_first_buffer_catch = totalLine == 0;

				if(is_first_buffer_catch) {
					int totalColumn = this->getColumnTotal(buffer);
					this->graph.createAdjacenteMatrix(totalColumn);
				}
				
				this->tokenize(buffer, totalLine);

				totalLine++;
			}

			return this->graph;
		}
};

int main() {
	string input;
	do {
		cout << "Informe o camanho do arquivo [sair: termina de executar]: ";
		cin >> input;
		cout << endl << endl;

		if(strcmp(input.c_str(), "sair") == 0) break;

		try {
			Graph graph;
			FileHandler file(graph);

			graph = file.read(input);
			graph.print();
			graph.mis().showResult();
		} catch(exception &e) {
			cout << "Exception: " << e.what() << endl;
		}
		
		system("pause");
		system("cls");
	} 
	while(strcmp(input.c_str(), "sair") != 0);

	cout << "Bye Bye!" << endl;

	return 0;
}