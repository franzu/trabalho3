#include <stdio.h>
#include <stdlib.h>

typedef struct elemento{//definimos a estrutura basica de cada elemento da matriz
	int coluna;
	int linha;
	int valor;
	struct elemento *direita;//com ponteiros para os proximos elementos 
	struct elemento *baixo;
}elemento;

typedef struct matriz_esparsa{//definimos uma "cabeça" para a matriz que contem:
  elemento **linhas;//vetor para as linhas
  elemento **colunas;//vetor para as colunas
  int nlin;
  int ncol;
}matriz;

elemento *novo(int x,int y,int valor){//função para criar e alocar um novo elemento
	elemento *novo;
	novo=(elemento*)malloc(sizeof(elemento));
	novo->coluna=x;
	novo->linha=y;
	novo->valor=valor;
	novo->baixo=novo->direita=NULL;
	return (novo);
}

matriz *criamatriz(int m, int n){// função que cria a matriz
  matriz *raiz=(matriz*)malloc(sizeof(matriz));
  if (raiz!=NULL){
    int i;
    raiz->ncol=n;//estabelece os valores da struct matriz_esparsa
    raiz->nlin=m;
    raiz->colunas=(elemento**)malloc(sizeof(elemento*)*n);
    raiz->linhas=(elemento**)malloc(sizeof(elemento*)*m);
    if(raiz->colunas!=NULL && raiz->linhas!=NULL){
      for(i=0;i<n;i++){//estabelece todas as cabeças de linha
        raiz->colunas[i]=(elemento*)malloc(sizeof(elemento));
        raiz->colunas[i]->baixo=NULL;
        raiz->colunas[i]->valor=0;
      }

      for (i=0;i<m;i++) {// e todas as cabeças de coluna
        raiz->linhas[i]=(elemento*)malloc(sizeof(elemento));
        raiz->linhas[i]->direita= NULL;
        raiz->linhas[i]->valor=0;
      }
    }
  }

  return raiz;
}

void insert(matriz *raiz,int m,int n,int valor){//função para inserir elemento na matriz
  if(m<raiz->nlin && n<raiz->ncol){//checa se os valores podem ser usados
    elemento *aux=raiz->linhas[m];
    while(aux->direita!=NULL && aux->direita->coluna<=n)aux=aux->direita;//acha nas cabeças de coluna se for o caso
    if(aux->coluna==n)aux->valor=valor;
    else{
    	elemento *pnovo=(elemento*)malloc(sizeof(elemento));
    	if(pnovo!=NULL){//cria o elemento no meio da matriz
        	pnovo=novo(m,n,valor);
        	pnovo->direita=aux->direita;//estabelece ponteiro direito
        	aux->direita=pnovo;
			aux=raiz->colunas[n];
        	while(aux->baixo!=NULL && aux->baixo->linha<=m)aux=aux->baixo;
        	pnovo->baixo=aux->baixo;//estabelece ponteiro para baixo
        	aux->baixo=pnovo;
        }
    }
    printf("Elemento inserido!\n");
    return;
	}
  printf("O elemento nao pode ser colocado nesta matriz!\n");
  return;
}

void apagamatriz(matriz **raiz){//função que apaga toda a matriz
  int i;
  for (i=0; i< (*raiz)->nlin ;i++){//for para apagar as linhas e colunas
    elemento *aux=(*raiz)->linhas[i]->direita;//vetor de cabeças de coluna
    while(aux!=NULL){//while para apagar as colunas
      elemento *del=aux;
      aux=aux->direita;
      free(del);
    }
    free((*raiz)->linhas[i]);//apaga todo vetor de linha
  }
  free((*raiz)->linhas);//apaga a struct matriz esparsa
  free((*raiz)->colunas);
  free((*raiz));
  *raiz=NULL;
}

void consultavalor(matriz *raiz,int m,int n){//função para consultar valor de posição
	if(m<=raiz->nlin&& n<=raiz->ncol){
    elemento *aux=raiz->linhas[m];//para procurar em todas as linhas
    	while(aux->direita!=NULL && aux->direita->coluna<=n)aux=aux->direita;//acha nas cabeças de coluna
    	if(aux->coluna==n){//achado o elemento
    	printf("O elemento contem o valor de %d.\n",aux->valor);
    	return;
    	}
  	}
  printf("O elemento nao esta nesta matriz!\n");
  return;
}

void somacoluna(matriz *raiz,int coluna){//função soma coluna
	if(coluna<=raiz->ncol){
		elemento *x=raiz->colunas[coluna-1];//acha a cabeça de coluna no vetor principal
		int soma=0;
		while(x!=NULL){//faz a soma 
			soma=soma+x->valor;
			x=x->baixo;
		}
		printf("A soma da coluna %d eh %d\n",coluna,soma);
		return;
	}
	printf("A coluna nao pertence a matriz.\n");
	return;
	}

void somalinha(matriz *raiz,int linha){// função soma de linha com mesmo raciocinio da soma coluna
	if(linha<=raiz->nlin){
		elemento *y=raiz->linhas[linha-1];
		int soma=0;
		while(y!=NULL);{
			soma=soma+y->valor;
			y=y->direita;
		}
	printf("A soma da linha %d eh %d\n",linha,soma);
	return;
	}
	printf("A linha nao pertence a matriz.\n");
	return;
}

void main(){
	matriz *raiz=NULL;//definimos a matriz_esparsa principal
	int menu=0,m,n,valor;
	printf(" --- Matriz Esparsa --- \n");
	printf("Escolha as operacoes pelo menu abaixo:\n");
	printf("1.Criar a matriz M(linhas)xN(colunas);\n2.Excluir a matriz;\n3.Atribuir valor a posicao(aij);\n");
	printf("4.Consultar valor da posicao(aij);\n5.Consultar soma de linha;\n6.Consultar soma de coluna;\n");
	printf("7.Sair;\n");
	while(menu<7||menu>0){
		scanf("%d",&menu);
		switch(menu){
			case 1://aloca os vetores da cabeças com o tamanho dado pelo usuario
				printf("Qual o tamanho que deseja implementar(digite primeiro M depois N)?\n");
				scanf("%d %d",&m,&n);
				if(m<=0||n<=0){
					printf("Numeros invalidos.\n");
					break;
				}
				else{
					raiz=criamatriz(m,n);
					break;
				}
			case 2://apaga a matriz, voltando o programa ao zero
				if(raiz!=NULL){
					apagamatriz(&raiz);
					break;
				}
				else{
					printf("Implemente a matriz antes de usar esta funcao.\n");
					break;
				}
			
			case 3://atribui valor a elemento com coordenadas dadas pelo usuario
				if(raiz!=NULL){
					printf("Em qual posicao deseja atribuir valor?\nLinha:");
					scanf("%d",&m);
					printf("Coluna:");
					scanf("%d",&n);
					printf("Digite o valor a ser inserido na posicao:");
					scanf("%d",&valor);
					insert(raiz,m,n,valor);
					m=n=valor=0;
					break;
				}
				else{
					printf("Implemente a matriz primeiro!\n");
					break;
				}
			case 4:
				if(raiz!=NULL){
					printf("De qual posicao deseja saber o valor?\nLinha:");
					scanf("%d",&m);
					printf("Coluna:");
					scanf("%d",&n);
					consultavalor(raiz,m,n);
					break;
				}
				else{
					printf("Implemente a matriz primeiro!\n");
					break;
				}
			case 5:
				if(raiz!=NULL){
					printf("De qual linha deseja saber a soma?");
					scanf("%d",&m);
					somalinha(raiz,m);
					break;
				}
				else{
					printf("Implemente a matriz primeiro!\n");
					break;
				}
			case 6:
				if(raiz!=NULL){
					printf("De qual coluna deseja saber a soma?");
					scanf("%d",&n);
					somalinha(raiz,n);
					break;
				}
				else{
					printf("Implemente a matriz primeiro!\n");
					break;
				}
			case 7:
				printf("Adios!");
				return;
			default:
				printf("Entrada errada!\n");
				break;
		}
		printf("Escolha novamente:");
	}
}

