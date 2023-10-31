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
	bool is_visitado;
} ContentStruct;

typedef struct coluna {
	ContentStruct* Content;
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
	ContentStruct* Content;
	struct no* Prox;
} NoStruct;

typedef struct elemento {
	int tamanho;
	NoStruct* NoInicial;
	struct elemento* Prox;
} ElementoStruct;

typedef struct lista {
	int tamanho;
	ElementoStruct* ElementoInicial;
} ListaStruct;

/** Variáveis global */
MatrizAdjacenteStruct Matriz;
ListaStruct Lista;

/** INTERFACES DE FUNÇÕES QUE SERÃO IMPLEMENTADO ABAIXO DO MAIN() */
int obterQuantidadeColuna(char*);

// Alocação/remoção de memoria
void alocarMemoria(char*);
void alocarLinhaMatriz();
void alocarColunaMatriz();
void alocarLista();
void liberarMemoria();

// Operacao na lista
void adicionarNo(ElementoStruct*, ContentStruct);
bool temContent(ElementoStruct*, ContentStruct);
void printLista();

// Manipulacao de arquivos e preenchimento da matriz
void tokenizar(char*, int, void (*func_ptr)(char*, int, int));
void atribuirToken(char*, int, int);
void referenciarDiaInferiorcomDiaSuperior();
void preencherMatriz(char*, int);
void readFile(char*);
void printMatriz();

// Obteção do grafo SCIE

// Funções Extras
template<typename T> void throwExceptionMemoryAlloc(T);

// Função Principal
int main(void) 
{
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

/*** IMPLMENTAÇÕES ***/
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
	while(token != NULL) 
	{
		token = strtok(NULL, delimitador);
		posicao++;
	}
	
	return posicao;
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
	Lista.tamanho = 0;

	alocarLinhaMatriz();
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

	if(pos_coluna <= pos_linha) 
	{
		LinhaStruct* linha = Matriz.Linha[pos_linha];
		ColunaStruct* coluna = linha->Coluna[pos_coluna];

		coluna->Content = (ContentStruct*) malloc(sizeof(ContentStruct));
		throwExceptionMemoryAlloc<ContentStruct*>(coluna->Content);

		strcpy(coluna->Content->content, token);
	} 
}

void referenciarDiaInferiorcomDiaSuperior() 
{
	for(int i = 0; i < Matriz.dimensao; i++) 
	{
		for(int j = i+1; j < Matriz.dimensao; j++) 
		{
			LinhaStruct* linhaOrigem = Matriz.Linha[j];
			ColunaStruct* colunaOrigem = linhaOrigem->Coluna[i];

			LinhaStruct* linhaDestino = Matriz.Linha[i];
			ColunaStruct* colunaDestino = linhaDestino->Coluna[j];

			colunaDestino->Content = colunaOrigem->Content;
		}
	}
}

void preencherMatriz(char* buffer, int pos_linha) 
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
		
		preencherMatriz(buffer, qtdLinha); 
		
		qtdLinha++;
	}

	referenciarDiaInferiorcomDiaSuperior();
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
			printf("(%d, %d)=%s",i, j, Matriz.Linha[i]->Coluna[j]->Content->content);
			printf("%s", (j != Matriz.dimensao - 1 ? " ": ""));
		}
		printf("\n");
	}
	printf("==================================================\n");
}

void liberarMemoria() 
{
	for(int pos = 0; pos < Matriz.dimensao; pos++) 
	{
		LinhaStruct* linha = Matriz.Linha[pos];
		free(linha->Coluna);
	}
	
	free(Matriz.Linha);
}

template<typename T> void throwExceptionMemoryAlloc(T ptr) 
{
	if(ptr == NULL) 
	{
		throw ERROR_ALOCACAO_MEMORIA_CODE;
	}
}

/** MANIPULAÇÃO DA LISTA */
void alocarLista() 
{
	Lista.tamanho++;

	
	ElementoStruct* novoElemento = (ElementoStruct*) malloc(sizeof(ElementoStruct));
	novoElemento->NoInicial = NULL;
	novoElemento->tamanho = 0;

	ElementoStruct* aux = Lista.ElementoInicial;
	ElementoStruct* auxAnterior = NULL;

	if(aux == NULL) 
	{
		Lista.ElementoInicial = novoElemento;
		return;
	}

	while(aux != NULL)
	{
		auxAnterior = aux;
		aux = aux->Prox;
	}

	auxAnterior->Prox = novoElemento;

}

void adicionarNo(ElementoStruct* elemento, ContentStruct content)
{
	elemento->tamanho++;

	NoStruct* novoNo = (NoStruct*) malloc(sizeof(NoStruct));
	novoNo->Content = &content;
	novoNo->Prox = NULL;

	NoStruct* aux = elemento->NoInicial;
	NoStruct* auxAnterior = NULL;

	if(aux == NULL)
	{
		elemento->NoInicial = novoNo;
		return;
	}

	while(aux != NULL) 
	{
		auxAnterior = aux;
		aux = aux->Prox;
	}

	auxAnterior->Prox = novoNo;
}

bool temContent(ElementoStruct* elemento, ContentStruct content)
{
	NoStruct* no = elemento->NoInicial;

	while(no != NULL) 
	{
		bool is_contents_iguais = no->Content == &content;
		if(is_contents_iguais) 
		{
			return true;
		}

		no = no->Prox;
	}

	return false;
}

void printLista() 
{
	if(Lista.ElementoInicial == NULL)
	{
		printf("<< LISTA VAZIA >>\n");
		return;
	}

	printf("==================================================\n");
	printf("  LISTAGEM DOS %d CONJUNTOS QUE NAO SE TOCAM    \n", Lista.tamanho);
	printf("==================================================\n");

	ElementoStruct* elemento = Lista.ElementoInicial;
	while(elemento != NULL) 
	{
		NoStruct* no = elemento->NoInicial;
		printf("QTD [%d]: ", elemento->tamanho);
		while(no != NULL)
		{
			printf("%s ", no->Content->content);
			no = no->Prox;
		}
		printf("\n");

		elemento = elemento->Prox;
	}

	printf("==================================================\n");
}

void scie(ContentStruct content) 
{
	
}

void calcular(ContentStruct content) 
{

}