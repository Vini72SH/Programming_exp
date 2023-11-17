#include <stdio.h>
#include <stdlib.h>

/*
 * evento_t possui os dados que descrevem os eventos 
 */
struct evento_t {
    int tempo;  /* tempo logico do evento */
    int tipo;   /* inteiro para diferenciar os diferentes tipos de evento */
    int dado1;  /* um inteiro para guardar dados do evento */
    int dado2;  /* um inteiro para guardar dados do evento */
};

/*
 * nodo_t eh uma estrutura interna que representa cada nodo da LEF.
 * Externamente o usuario da biblioteca nao vai receber essa estrutura para 
 * evitar acesso direto ao ponteiro prox.
 */
struct nodo_lef_t {
    struct evento_t   *evento;
    struct nodo_lef_t *prox;
} ;

/*
 * Representacao da LEF
 */
struct lef_t {
    struct nodo_lef_t *primeiro;
};

/* 
 * Cria um evento contendo os parâmetros informados.
 * Retorna um ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2){
    struct evento_t *novoEvento;

    novoEvento = malloc(sizeof(struct evento_t));
    if (novoEvento == NULL){
        return NULL;
    }

    novoEvento->tempo = tempo;
    novoEvento->tipo = tipo;
    novoEvento->dado1 = dado1;
    novoEvento->dado2 = dado2;

    return novoEvento;
}

/* 
 * Informa se a LEF está vazia.
 * Retorna 1 se vazia e 0 senao ou se falhar.
 */
int vazia_lef (struct lef_t *l){

    if(l->primeiro == NULL){
        return 1;
    }

    return 0;
}

/* 
 * Destroi um evento e retorna NULL.
 */ 
struct evento_t *destroi_evento (struct evento_t *e){
    
    if(e != NULL){
        free(e);
    }

    return NULL;   
}

/* 
 * Cria uma LEF vazia 
 * Retorna um ponteiro para a LEF ou NULL se falhar.
 */
struct lef_t *cria_lef (){
    struct lef_t *novaLef;

    novaLef = malloc(sizeof(struct lef_t));
    if(novaLef == NULL){
        return NULL;
    }

    novaLef->primeiro = NULL;

    return novaLef;
}

/* 
 * Destroi a LEF, liberando a memória ocupada por ela, inclusive os eventos.
 * Retorna NULL.
 */ 
struct lef_t *destroi_lef (struct lef_t *l){
    struct evento_t *evento;
    struct nodo_lef_t *aux, *no;

    if(l != NULL){
        aux = l->primeiro;
        while(aux != NULL){
            no = aux->prox;
            evento = aux->evento;
            destroi_evento(evento);
            free(aux);
            aux = no;
        }

        free(l);

        return NULL;
    }
}

/*
 * Insere o evento apontado na LEF na ordem de tempos crescentes.
 * Eventos com o mesmo tempo devem respeitar a politica FIFO.
 * Retorna 1 em caso de sucesso ou 0 caso contrario.
*/
int insere_lef (struct lef_t *l, struct evento_t *e){
    struct nodo_lef_t *novoNodo, *aux;

    novoNodo = malloc(sizeof(struct nodo_lef_t));
    if(novoNodo == NULL){
        return 0;
    }

    novoNodo->evento = e;

    if (vazia_lef(l) || novoNodo->evento->tempo < l->primeiro->evento->tempo){
        novoNodo->prox = l->primeiro;
        l->primeiro = novoNodo;

        return 1;
    }

    aux = l->primeiro;
    while((aux->prox != NULL) && (aux->prox->evento->tempo <= novoNodo->evento->tempo)){
        aux = aux->prox;
    }

    novoNodo->prox = aux->prox;
    aux->prox = novoNodo;

    return 1;
}

/* 
 * Retira o primeiro evento da LEF.
 * Retorna ponteiro para o evento ou NULL se falhar.
 */
struct evento_t *retira_lef (struct lef_t *l){
    struct nodo_lef_t *aux;
    struct evento_t *evento;

    if(vazia_lef(l)){
        return NULL;
    }

    aux = l->primeiro;
    evento = aux->evento;
    l->primeiro = aux->prox;

    free(aux);

    return evento;
}

/* 
 * Imprime a LEF. Não faz parte do TAD, mas serve para depuração.
 * Formato de saída:
 * tempo tipo dado1 dado2
 * tempo tipo dado1 dado2
 * ...
 * total XX eventos
 */
void imprime_lef (struct lef_t *l){
    int cont = 0; 
    struct nodo_lef_t *aux;

    printf("LEF:\n");
    if(!(vazia_lef(l))){
        aux = l->primeiro;
        while(aux != NULL){
            cont++;
            printf("  tempo %d tipo %d d1 %d d2 %d\n", aux->evento->tempo, aux->evento->tipo, aux->evento->dado1, aux->evento->dado2);
            aux = aux->prox;
        }
    }
    printf("Total %d eventos\n", cont);
}

int main(){
  int tempo, tipo, dado1, dado2, n, i;
  struct lef_t *l ;
  struct evento_t *ev, *ret;

  // cria uma lista L vazia
  l = cria_lef ();
  printf("Lista criada com sucesso!\n");

  printf("Digite o número de eventos:\n");
  scanf("%d", &n);

  for(i = 1; i <= n; i++){
    printf("Digite um evento para inserir:\n");
    printf("Tempo:\n");
    scanf("%d", &tempo);
    printf("Tipo:\n");
    scanf("%d", &tipo);
    printf("Dado1:\n");
    scanf("%d", &dado1);
    printf("Dado2:\n");
    scanf("%d", &dado2);


    ev = cria_evento(tempo, tipo, dado1, dado2);
    insere_lef(l, ev);
  }

  printf("Impressão da Lista:\n");
  imprime_lef(l);

  printf("Retirando o primeiro evento...\n");
  ret = retira_lef(l);

  printf("Impressão da Lista:\n");
  imprime_lef(l);

  // destrói L
  l = destroi_lef (l);
  printf("Lista destruida com sucesso!\n");

  // encerra
  return (0);

  /*struct lef_t *l ;
  struct evento_t *ev ;

  // cria uma lista L vazia
  l = cria_lef () ;

  // imprime L
  imprime_lef (l) ;

  // insere eventos na LEF
  for (int i = 0; i < 4; i++)
  {
    ev = cria_evento (i, 0, 0, 0) ;
    insere_lef (l, ev) ;
  }

  // imprime L
  imprime_lef (l) ;

  // insere mais eventos na LEF
  for (int i = 2; i < 6; i++)
  {
    ev = cria_evento (i, 1, 1, 1) ;
    insere_lef (l, ev) ;
  }

  // imprime L
  imprime_lef (l) ;

  // enquanto L não estiver vazia
  while (!vazia_lef (l))
  {
    //   retira o primeiro evento de L
    ev = retira_lef (l) ;
    if (ev)
      printf ("retirei evento %d %d %d %d\n", ev->tempo, ev->tipo,
              ev->dado1, ev->dado2) ;
    else
      printf ("sem evento\n") ;
    destroi_evento (ev) ;

    // imprime L
    imprime_lef (l) ;
  }

  // destrói L
  l = destroi_lef (l) ;

  // encerra
  return (0) ;
  */
}