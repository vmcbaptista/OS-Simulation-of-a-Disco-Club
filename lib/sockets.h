/*
 * sockets.h
 *
 *  Created on: Nov 15, 2015
 *      Author: fabio
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef LIB_SOCKETS_H_
#define LIB_SOCKETS_H_

//trinco para proteger escritas e leituras no socket
pthread_mutex_t protegeSocket;
//socket onde se vai escrever é inicializado nas funcoes criaSocketServidor e cliente
int sockfd ;
//dados enviados pela tarefa para o socket
struct tarefaEnvia{
	char* mensagem;
	int tid;
}mensageThread;

pthread_mutex_t protStats;
sem_t aguardaSimulStart;

#define UNIXSTR_PATH "/tmp/s.unixstr"
#define UNIXDG_PATH  "/tmp/s.unixdgx"
#define UNIXDG_TMP   "/tmp/dgXXXXXXX"


int readn(int fd,char* ptr,int  nbytes);
int writen(int fd,char* ptr,int nbytes);
int readline(int fd,char* ptr,int maxlen);
err_dump(char* msg);
bzero(char * p,int n);
bcopy(char* p,char *q,int n);
void criaSocketCliente();
void criaSocketServidor();

str_cli(int sockfd, int tipoPessoa, int acao, long id, int seccao);
#endif /* LIB_SOCKETS_H_ */
