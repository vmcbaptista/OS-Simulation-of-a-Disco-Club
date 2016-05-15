/*
 * header.h
 *
 *  Created on: Oct 28, 2015
 *      Author: fabio
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#ifndef HEADER_H_
#define HEADER_H_

//Definir secções da discoteca
#define DISCOTECA 0
#define PISTA_NORMAL 1
#define PISTA_DANCA_SALAO 2
#define PISTA_POOL_PARTY 7
#define ZONA_SOFAS 4
#define SNACK_BAR 5
#define SALA_JOGOS 6
#define BAR 3

//Definir comandos que as tarefas podem executar
#define ENTROU 1
#define SAIU 2
#define ESPERA 3
#define CHEGOU 4
#define PRODUZIU 5
#define DESISTIU 6
#define ESPERA_PARCEIRO 7

//Definir tipos de tarefas
#define NORMAL 1
#define VIP 2
#define EMPREGADO_BAR 3
#define EMPREGADO_SNACK_BAR 4

int TEMPO_SIMULACAO;
int TEMPO_SIMULADO;

/*GUarda o número de pessoas numa sala ||||||||||||*******PROTEGER COM TRINCOS|SEMAFOROS******||||||||||||*/
struct lotacao{
	int lotTotal;		//Lotação máxima da discoteca
	int lotDS;			//Lotação Dancas de salao
	int lotNorm;		//Lotação Normal (Todos têm acesso)
	int lotPool; 		//Lotação Pool Party
	int lotSofas;		//Lotação Sofás
	int lotJogos;		//Lotação Sala Jogos
	int lotSnackBar;	//Lotação Snack Bar
	int lotBar;			//Lotacao Bar
}lotDisc,lotMax;


struct tempoEspera{
	int tempoEntrada;	//Tempo Entrada
	int tempoVip;		//Tempo VIP
	int tempoNorm;		//Tempo Normal (Todos têm acesso)
	int tempoPool; 		//Tempo Pool Party
	int tempoBar;		//Tempo Sofás
	int tempoJogos;		//Tempo Sala Jogos
	int tempoSnackBar;	//Tempo Snack Bar

}esperaTempo;

struct tempFuncionamento{
	float aberturaSB;
	float encerramentoJogos;
}funcionamento;

struct limiteFila{
	int limiteDD;		//Limite da fila  VIP
	int limiteNorm;		//Limite da fila  Normal (Todos têm acesso)
	int limitePool; 	//Limite da fila  Pool Party
	int limiteJogos;	//Limite da fila  Sala Jogos
	int limiteSnackBar;	//Limite da fila  Snack Bar

}limitePessoasFila;

struct mensagemMonitor{
	char* indicaEstado;		//Mensagem que precede a indicacao do estado da simulacao
	char* simTerminada;		//Mensagem que indica que a simulacao esta a decorrer
	char* simDecorre;		//Mensagem que indica que a simulacao esta a decorrer
	char* nUtilizadores;	//Mensgem que antecede o numero de utilizadores
	char* nDesistencias;	//Mensgem que antecede o numero de desistentes
	char* tempoMedioEspera; //Mensgem que antecede a indicacao do tempo medio de espera
	char* oCliente;			//O cliente
	char* entrou;			//entrou
	char* chegou;			//chegou
	char* saiu;				//saiu
	char* espera;			//espera
	char* esperaParceiro;	//espera por um parceiro
	char* produziu;			//produziu uma bebida
	char* desistiu;			//desisitiu
	char* disc;				//à discoteca
	char* pistaSalao;		//na Pista Dancas de Salao.
	char* pistaTodos;		//na Pista Todos.
	char* pistaPP;			//na Pista Pool Party.
	char* zns;				//na zona dos sofas.
	char* sb;				//no snack bar.
	char* sj;				//na sala de jogos.
	char* bar;				//bar
	char* normal;			//normal
	char* vip;				//VIP
	char* empregadoBar;		//Empregado do Bar
	char* empregadoSnack;	//Empregado do Snack Bar
	char* tempoSimulacao;

}*apGlobMsgMonitor;

// Tempos de abertura/encerramento na simulacao
time_t abreBar;
time_t fechaJogos;
pthread_mutex_t funcionamentoDisc;

//Apontador para o ficheiro onde se vão escrever os logs
FILE * logPointer;

pthread_mutex_t protEntrada, protSocket,protNDDanSalao, protJogos, protZonaSofas, protNorm;
sem_t normais ,fechaDanSalao, esperaParceiro, semJogos, semNorm;
//Fila quando a disco está cheia
sem_t vipwaitDiscoFull;
sem_t normwaitDiscoFull;

//PoolParty
pthread_mutex_t entraPP;
sem_t deixaEntrarPP,lotacaoPPFull;
int numFilaEsperaPP;
//Snack Bar
pthread_mutex_t protCliFilasBar;
sem_t esperaCliSB,esperaSerServido;
int cliFilaSB;
int tamFilaPP;
//end PoolParty
//Produtores consumidores
int balcao[10];
int prodPosition;
int consPosition;
sem_t canProduce;
sem_t  canConsume;
pthread_mutex_t protbalcao;
//end
int normalNaFila;
int vipNaFila ;
int numDancers;
int numFilaDD;
int filaJogos ;
int filaNormal;
int condicaoAbertura;

// Estatísticas
int desistenciasDisc, desistenciasZS, desistenciasNormal, desistenciasPP, desistenciasDS, desistenciasSJ, desistenciasBar, desistenciasSB; //contam desistencias

int nClientesDia; 					// contam numero de pessoas que tentaram entrar na discoteca

int nClientesEntraramDisc;			// contam numero de pessoas que entraram na discoteca
int nClientesEntraramDS;			// contam numero de pessoas que entraram na danca salao
int nClientesEntraramPistaNormal;	// contam numero de pessoas que entraram na pista normal
int nClientesEntraramPP;			// contam numero de pessoas que entraram na Pool Party
int nClientesEntraramBar;			// contam numero de pessoas que entraram no Bar
int nClientesEntraramZS;			// contam numero de pessoas que entraram na zona sofas
int nClientesEntraramSB;			// contam numero de pessoas que entraram no Snack Bar
int nClientesEntraramSJ;			// contam numero de pessoas que entraram na sala jogos

int nClientesSairamDisc;			// contam numero de pessoas que sairam da discoteca
int nClientesSairamDS;				// contam numero de pessoas que sairam da danca salao
int nClientesSairamPistaNormal;		// contam numero de pessoas que sairam da pista normal
int nClientesSairamPP;				// contam numero de pessoas que sairam da Pool Party
int nClientesSairamBar;				// contam numero de pessoas que sairam do Bar
int nClientesSairamZS;				// contam numero de pessoas que sairam da zona sofas
int nClientesSairamSB;				// contam numero de pessoas que sairam do Snack Bar
int nClientesSairamSJ;				// contam numero de pessoas que sairam da sala jogos

int nBebidasProd;					// conta numero de bebidas produzidas numa noite



/*Carrega os parametros iniciais do simulador*/
void carregaSimulador(char* simulador);
/*Carrega os parametros iniciais do monitor*/
struct mensagemMonitor* carregaMonitor(char* monitor);
/*FUnção que irá escrever os logs durante a execução do simulador*/
void escreveLog(FILE* log, char* mensagem);
/*FUnção que irá escrever no ecrã durante a execução do monitor*/
void escreveEcra(char* mensagem);
/*FUnção que irá criar utilizadores*/
void criaCliente(pthread_t *th,int arrayPosition, void *func_exec);
/*Elimina tarefas ou seja utilizadores*/
void elminaTarefas();
//Retorna o tempo atual desde a era UNIX
time_t tempoAtual();
//Realiza a soma entre o tempo atual e o número de segundo pretendido
time_t calculaTempo(float segundos);
// Retorna a diferença entre o tempo Atual e o tempo esperado para um acontecimento
time_t tempoFalta(time_t tempoEsperado);
//Retorna verdadeiro(1) se terminou o tempo esperado para um acontecimento
bool verificaPassagemTempo(time_t tempoEsperado);
// Converte um dado numero de minutos em segundos
float converteMinutos(float minutos);
// Converte um dado numero de segundos reais em tempo de
float converteSegundosReais(float segundosReais);


/*abre ficheiro na abertura da discoteca recebe o nome do ficheiro onde se vai escrever*/
FILE * abreFicheiro(char* logs);
/*fecha o ficheiro após encerramento da discoteca*/
void fechaFicheiro(FILE* log);

void *pistaNormal();
void pistaPP(int tipoPessoa,long id);
void *pistaDS(int tipoPessoa,long id);
void *zonaSofas(int tipoPessoa,long id);
void *bar(int tipoPessoa,long id);
void *snackBar(int tipoPessoa,long id);
void *salaJogos(int tipoPessoa,long id);
#endif /* HEADER_H_ */

