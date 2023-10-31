# include <stdio.h>
# include <stdlib.h>
# include <locale>
# include <string>
# include <list>
# define TAM_BUFFER_READ 1000
# define ERROR_FILE_NAO_EXISTE_CODE 404
# define ERROR_ALOCACAO_MEMORIA_CODE 200

/** Definições de estrutura de dados */
typedef struct content {
	char content[10];
} ContentStruct;

typedef struct coluna {
	ContentStruct Content;
} ColunaStruct;

typedef struct linha {
	ColunaStruct** Coluna;
} LinhaStruct;

typedef struct matrizAdjacente {
	int dimensao;
	bool is_valorado;
	LinhaStruct** Linha;
} MatrizAdjacenteStruct;

typedef struct no {
	ContentStruct Content;
	struct no* Prox;
} NoStruct;

typedef struct elemento {
	int tamanho;
	NoStruct* NoInicial;
} ElementoStruct;

typedef struct lista {
	int tamanho;
	ElementoStruct** Elemento;
} ListaStruct;

/** Variável global */
MatrizAdjacenteStruct Matriz;
ListaStruct Lista;

/** INTERFACES DE FUNÇÕES QUE SERÃO IMPLEMENTADO ABAIXO DO MAIN() */
void atualizarDimensaoMatriz(char*);
int obterQuantidadeColuna(char*);

// Alocação/remoção de memoria
void alocarMemoria(char*);
void alocarLinhaMatriz();
void alocarColunaMatriz();
void alocarLista();
void liberarMemoria();

// Operacao na lista
void adicionarNo(int, ContentStruct);
bool isNoAdicionado(int, ContentStruct);
void printLista();

// Manipulacao de arquivos e preenchimento da matriz
void tokenizar(char*, int, void (*func_ptr)(char*, int, int));
void atribuirToken(char*, int, int);
void preecherMatriz(char*, int);
void readFile(char*);
void printMatriz();

// Obteção do grafo SCIE

template<typename T> void throwExceptionMemoryAlloc(T);

int main(void) {
	setlocale(LC_ALL, "Portuguese");
	
	try 
	{
		char fileTmp[] = "./tmp/teste01.txt";
		readFile(fileTmp);
		
		printMatriz();
		printLista();
		
		liberarMemoria();
	} 
	catch(int e) 
	{
		printf("<< Houve problema. Code: %d\n", e);
	}

	return 0;
}

int obterQuantidadeColuna(char* buffer) 
{
	const char delimitador[2] = " ";
	char* token;
	char novoBuffer[TAM_BUFFER_READ];
	
	strcpy(novoBuffer, buffer);
	
	/** Obter o primeiro token */
	token = strtok(novoBuffer, delimitador);
	
	/** Obter o segundo token */
	int posicao = 0;
	while(token != NULL) {
		
		token = strtok(NULL, delimitador);
		posicao++;
	}
	
	return posicao;
}

void atualizarDimensaoMatriz(char* token) 
{
	Matriz.dimensao++;
}

void alocarMemoria(char* buffer) 
{
	/** 
	 * Pega o buffer e calcula quanto 0s e 1s existem dentro do arquivo e 
	 * chama a funcao para atualizar a dimensao da matriz. 
	 * Lembre de callback em javascript.
	 */
	int tamanho = obterQuantidadeColuna(buffer);
	Matriz.dimensao = tamanho;
	Lista.tamanho = tamanho;

	alocarLinhaMatriz();
	alocarLista();
}

void alocarLinhaMatriz() 
{
	// Alocando a linha da matriz
	LinhaStruct** linhas =
		(LinhaStruct**) malloc(sizeof(LinhaStruct*) * Matriz.dimensao);
	throwExceptionMemoryAlloc<LinhaStruct**>(linhas);
	
	// Alocar o objeto/struct para utilizar as propriedades, similar objetos em POO
	for(int posicao = 0; posicao < Matriz.dimensao; posicao++) 
	{
		linhas[posicao] = (LinhaStruct*) malloc(sizeof(LinhaStruct));
		throwExceptionMemoryAlloc<LinhaStruct*>(linhas[posicao]);
	}
	
	// Fazendo um bind
	Matriz.Linha = linhas;
	
	alocarColunaMatriz();
}

void alocarColunaMatriz() 
{
	for(int posicaoLinha = 0; posicaoLinha < Matriz.dimensao; posicaoLinha++)
	{
		for(int posicaoColuna = 0; posicaoColuna < Matriz.dimensao; posicaoColuna++) 
		{
			
			/*! Mesma explicao da funcao "alocarLinhaMatriz()" !*/
			ColunaStruct** colunas = 
				(ColunaStruct**) malloc(sizeof(ColunaStruct*) * Matriz.dimensao);
			throwExceptionMemoryAlloc<ColunaStruct**>(colunas);
				
			for(int posicao = 0; posicao < Matriz.dimensao; posicao++) {
				colunas[posicao] = (ColunaStruct*) malloc(sizeof(ColunaStruct));
				throwExceptionMemoryAlloc<ColunaStruct*>(colunas[posicao]);
			}

			Matriz.Linha[posicaoLinha][posicaoColuna].Coluna = colunas;
		}
	}
}

void alocarLista() 
{
	Lista.Elemento = (ElementoStruct**) malloc(sizeof(ElementoStruct*) * Lista.tamanho);
	throwExceptionMemoryAlloc<ElementoStruct**>(Lista.Elemento);

	for(int i = 0; i < Lista.tamanho; i++) 
	{
		Lista.Elemento[i] = (ElementoStruct*) malloc(sizeof(ElementoStruct));
		throwExceptionMemoryAlloc<ElementoStruct*>(Lista.Elemento[i]);

		Lista.Elemento[i]->NoInicial = NULL;
		Lista.Elemento[i]->tamanho = 0;
	}
}

void tokenizar(
	char* buffer, 
	int pos_linha, 
	void (*func_ptr)(char* token, int posicao_linha, int posicao_coluna))
{
	const char delimitador[2] = " ";
	char* token;
	
	/** Obter o primeiro token */
	token = strtok(buffer, delimitador);
	
	/** Obter o segundo token */
	int posicao = 0;

	while(token != NULL) 
	{
		
		int ultima_posicao = strlen(token) - 1;
		bool is_escape_quebra_linha = token[ultima_posicao] == '\n';

		if(is_escape_quebra_linha) 
		{
			token[ultima_posicao] = '\0';
		}
		
		/**
		* CHAMADA DE CALLBACK: em javascript por exemplo, tem muito esse conceito de callback.
		* O objetivo de usar callback é separação de responsabilidade de código.
		* Essa função, a única responsabilidade dela é gerar tokens e cuspir para outro local, atribuir numa matriz/vetor
		* fica a cargo de outra função. Tem como objetivo deixar o código mais limpo.
		**/
		(*func_ptr)(token, pos_linha, posicao);
		
		posicao++;
		
		token = strtok(NULL, delimitador);
	}
}

void atribuirToken(char* token, int pos_linha, int pos_coluna) 
{
	LinhaStruct* linha = Matriz.Linha[pos_linha];
	ColunaStruct* coluna = linha->Coluna[pos_coluna];
	
	strcpy(coluna->Content.content, token);
}

void preecherMatriz(char* buffer, int pos_linha) 
{
	// a funcao "tokenizar" vai gerar os tokens e cuspir para a funcao "atribuirToken"
	tokenizar(buffer, pos_linha, &atribuirToken);
}

void lancarExceptionSeArquivoNaoExiste(FILE* fptr) 
{
	if(fptr == NULL) 
	{
		throw ERROR_FILE_NAO_EXISTE_CODE;
	}
}

void readFile(char* file_source) 
{
	FILE* fptr = fopen(file_source, "r");
	char buffer[TAM_BUFFER_READ];
	
	lancarExceptionSeArquivoNaoExiste(fptr);
	
	int qtdLinha = 0;
	while(fgets(buffer, TAM_BUFFER_READ, fptr)) 
	{
		/* EXPLICÃO DO: "if(qtdLinha == 0)"
		* Quando for a primeira vez, o algoritmo irá pegar o buffer da primeira linha
		* lido no arquivo e irá calcular quanto de espaço ixj a matriz irá precisar 
		* para armazenar os tokens capturados do arquivo.
		**/
		if(qtdLinha == 0) 
		{
			alocarMemoria(buffer);
		}
		
		preecherMatriz(buffer, qtdLinha); 
		
		qtdLinha++;
	}
}

void printMatriz() 
{
	printf("==================================================\n");
	printf("       MATRIZ ADJACENCIA: { %d X %d }             \n", Matriz.dimensao, Matriz.dimensao);
	printf("==================================================\n");

	for(int i = 0; i < Matriz.dimensao; i++) 
	{
		printf("\t\t");
		for(int j = 0; j < Matriz.dimensao; j++) 
		{
			printf("[%2s ] ", Matriz.Linha[i]->Coluna[j]->Content.content);
		}
		printf("\n");
	}
	printf("==================================================\n");
}

void liberarMemoria() {
	for(int pos = 0; pos < Matriz.dimensao; pos++) {
		LinhaStruct* linha = Matriz.Linha[pos];
		free(linha->Coluna);
	}
	
	free(Matriz.Linha);
}

template<typename T> void throwExceptionMemoryAlloc(T ptr) 
{
	if(ptr == NULL) {
		throw ERROR_ALOCACAO_MEMORIA_CODE;
	}
}

/** MANIPULAÇÃO DA LISTA */
void adicionarNo(int posicaoElemento, ContentStruct content)
{
	ElementoStruct* elemento = Lista.Elemento[posicaoElemento];
	
	NoStruct* noAnt = NULL;
	NoStruct* aux = elemento->NoInicial;

	NoStruct* no = (NoStruct*) malloc(sizeof(NoStruct));
	no->Content = content;
	no->Prox = NULL;
	elemento->tamanho++;

	if(aux == NULL) 
	{
		elemento->NoInicial = no;
		return;
	}

	while(aux != NULL) 
	{
		noAnt = aux;
		aux = aux->Prox;
	};

	noAnt->Prox = no;
}

bool isNoAdicionado(int posicaoElemento, ContentStruct content)
{
	ElementoStruct* elemento = Lista.Elemento[posicaoElemento];

	NoStruct* aux = elemento->NoInicial;

	while(aux != NULL) {
		bool is_content_iguais = strcmp(aux->Content.content, content.content) == 0;
		if(is_content_iguais) 
		{
			return true;
		}
		aux = aux->Prox;
	}

	return false;
}

void printLista() 
{
	printf("==================================================\n");
	printf("  LISTAGEM DE CONJUNTOS QUE NAO SE TOCAM    \n");
	printf("==================================================\n");

	for(int posicaoElemento = 0; posicaoElemento < Lista.tamanho; posicaoElemento++) {
		ElementoStruct* elemento = Lista.Elemento[posicaoElemento];
		NoStruct* aux = elemento->NoInicial;

		printf("QTD[%d]: ", elemento->tamanho);
		while(aux != NULL) {
			printf("%s ", aux->Content.content);
		}

		printf("\n");
	}

	printf("==================================================\n");
}