/*
 * sockets.c
 *
 *  Created on: Nov 15, 2015
 *      Author: fabio
 */
#include "sockets.h"
#include "header.h"
#define MAXLINE 512

char menu()
{
	printf("##################Discoteca###################\n");
	printf("# Escolha uma opção:                         #\n");
	printf("#           1 - Iniciar simulação            #\n");
	printf("#           2 - Terminar programa            #\n");
	printf("##############################################\n");
	char opcao[MAXLINE];
	do{
		scanf("%s",opcao);
		if((opcao[0] == '1' || opcao[0] == '2' ) && strlen(opcao) == 1)
		{
			break;
		}
		else
		{
			printf("Opção inválida");
			opcao[0] = '\0';
		}
	}while(strlen(opcao) > 1 || strlen(opcao) <= 0 );

	return opcao[0];
}
/* Cliente do tipo socket stream.
   L string de fp e envia para sockfd.
   L string de sockfd e envia para stdout */
str_cli(int sockfd, int tipoPessoa, int acao, long id, int seccao)
{
	int n;
	char msgConcat[MAXLINE] = {};
	//Composicao da stream de envio pro socket
	sprintf(msgConcat,"%i %i %ld %i ",tipoPessoa,acao,id,seccao);
	n = write(sockfd,msgConcat,MAXLINE);
	if (n < 0) error("ERROR writing to socket");
}
/*Indica ao monitor que terminou a simulação*/
str_cliTermina(int sockfd)
{
	int n;
	n = write(sockfd,"finito",MAXLINE);
	if (n < 0) error("ERROR writing to socket");
}

/* Servidor fica em modo leitura, até receber um valor no buffer igual a termina */
bool str_echo(int sockfd)
{
	int n ;
	char buffer[MAXLINE];
	char msgConcat[MAXLINE];
	char tid[MAXLINE];
	for(;;){
		bzero(buffer,MAXLINE);
		bzero(msgConcat,MAXLINE);
		char tid[MAXLINE];
		int tipoPessoa, acao, id,seccao = -1;
		char _tipoPess[5], _acao[5], _id[200], _seccao[5];
		bzero(_tipoPess,5);
		bzero(_id,200);
		bzero(_seccao,5);
		bzero(_acao,5);
		n = read(sockfd,buffer,MAXLINE);
		if(n == 0) return false;
		if (n < 0) error("ERROR reading from socket");
		//No final da simulção o simulador envia para o socket o comando finito.
		if(!strcmp(buffer,"finito"))
		{
			return true;
		}
		else
		{
			//Desconstroi os codigos do socket e que vao ser escritos
			int j = 0;
			int k = 0;
			for(int i = 0 ;i <= 3;i++)
			{
				k= 0;
				for(;buffer[j] != ' ';j++)
				{
					//Acumula os valores do buffer
					if(i == 0)
					{
						_tipoPess[k] = buffer[j];
					}
					if(i == 1)
					{
						_acao[k]= buffer[j];
					}
					if(i== 2)
					{
						_id[k]= buffer[j];
					}
					if(i == 3)
					{
						_seccao[k] = buffer[j];
					}
					k++;
				}
				j++;
			}
			//Converte os valores lidos do buffer para tipo inteiro até ao primeiro valor livre
			tipoPessoa = atoi(_tipoPess);
			acao =	atoi(_acao);
			id = atoi(_id);
			seccao = atoi(_seccao);
			if(tipoPessoa == NORMAL || tipoPessoa == VIP)
			{
				strcat(msgConcat, apGlobMsgMonitor->oCliente);
			}
			else
			{
				strcat(msgConcat, "O funcionário ");
			}
			sprintf(tid, "%ld ", id);



			if(tipoPessoa == NORMAL)
				strcat(msgConcat,apGlobMsgMonitor->normal);
			else if(tipoPessoa == VIP)
				strcat(msgConcat,apGlobMsgMonitor->vip);
			strcat(msgConcat,tid);
			if(acao == CHEGOU)
				strcat(msgConcat,apGlobMsgMonitor->chegou);
			else if(acao == ENTROU)
				strcat(msgConcat,apGlobMsgMonitor->entrou);
			else if(acao == SAIU)
				strcat(msgConcat,apGlobMsgMonitor->saiu);
			else if(acao == ESPERA)
				strcat(msgConcat,apGlobMsgMonitor->espera);
			else if(acao == ESPERA_PARCEIRO)
				strcat(msgConcat,apGlobMsgMonitor->esperaParceiro);
			else if (acao == PRODUZIU)
				strcat(msgConcat,apGlobMsgMonitor->produziu);
			else if (acao == DESISTIU)
				strcat(msgConcat,apGlobMsgMonitor->desistiu);
			if(seccao == DISCOTECA)
				strcat(msgConcat,apGlobMsgMonitor->disc);
			else if(seccao == PISTA_NORMAL)
				strcat(msgConcat,apGlobMsgMonitor->pistaTodos);
			else if(seccao == PISTA_DANCA_SALAO)
				strcat(msgConcat,apGlobMsgMonitor->pistaSalao);
			else if(seccao == PISTA_POOL_PARTY)
				strcat(msgConcat,apGlobMsgMonitor->pistaPP);
			else if(seccao == ZONA_SOFAS)
				strcat(msgConcat,apGlobMsgMonitor->zns);
			else if(seccao == SNACK_BAR)
				strcat(msgConcat,apGlobMsgMonitor->sb);
			else if(seccao == SALA_JOGOS)
				strcat(msgConcat, apGlobMsgMonitor->sj);
			else if(seccao == BAR)
				strcat(msgConcat,apGlobMsgMonitor->bar);
			strcat(msgConcat, "\n");


		}
		//Escreve logs

		pthread_mutex_lock(&protStats);
		efetuaEstatisticas(seccao,acao);
		pthread_mutex_unlock(&protStats);
		escreveLog(logPointer,msgConcat);
	}
}

void efetuaEstatisticas(int seccao, int acao)
{
	if(acao == CHEGOU)
	{
		nClientesDia++;
	}
	else if(acao == ENTROU)
	{
		switch(seccao)
		{
		case DISCOTECA:
			nClientesEntraramDisc++;
			break;
		case PISTA_NORMAL:
			nClientesEntraramPistaNormal++;
			break;
		case PISTA_DANCA_SALAO:
			nClientesEntraramDS++;
			break;
		case PISTA_POOL_PARTY:
			nClientesEntraramPP++;
			break;
		case ZONA_SOFAS:
			nClientesEntraramZS++;
			break;
		case SNACK_BAR:
			nClientesEntraramSB++;
			break;
		case SALA_JOGOS:
			nClientesEntraramSJ++;
			break;
		case BAR:
			nClientesEntraramBar++;
			break;
		}
	}
	else if(acao == SAIU)
	{
		switch(seccao)
		{
		case DISCOTECA:
			nClientesSairamDisc++;
			break;
		case PISTA_NORMAL:
			nClientesSairamPistaNormal++;
			break;
		case PISTA_DANCA_SALAO:
			nClientesSairamDS++;
			break;
		case PISTA_POOL_PARTY:
			nClientesSairamPP++;
			break;
		case ZONA_SOFAS:
			nClientesSairamZS++;
			break;
		case SNACK_BAR:
			nClientesSairamSB++;
			break;
		case SALA_JOGOS:
			nClientesSairamSJ++;
			break;
		case BAR:
			nClientesSairamBar++;
			break;
		}
	}
	/*else if(acao == ESPERA)
	{
		switch(seccao)
		{
		case DISCOTECA:
			desistenciasDisc++;
			break;
		case PISTA_NORMAL:
			desistenciasNormal++;
			break;
		case PISTA_POOL_PARTY:
			desistenciasPP++;
			break;
		case PISTA_DANCA_SALAO:
			desistenciasDS++;
			break;
		case ZONA_SOFAS:
			desistenciasZS++;
			break;
		case SNACK_BAR:
			desistenciasSB++;
			break;
		case SALA_JOGOS:
			desistenciasSJ++;
			break;
		case BAR:
			desistenciasBar++;
			break;
		}
	}*/
	else if (acao == PRODUZIU)
	{
		nBebidasProd++;
	}
	else if (acao == DESISTIU)
	{
		switch(seccao)
		{
		case DISCOTECA:
			desistenciasDisc++;
			break;
		case PISTA_NORMAL:
			desistenciasNormal++;
			break;
		case PISTA_DANCA_SALAO:
			desistenciasDS++;
			break;
		case ZONA_SOFAS:
			desistenciasZS++;
			break;
		case SNACK_BAR:
			desistenciasSB++;
			break;
		case SALA_JOGOS:
			desistenciasSJ++;
			break;
		case BAR:
			desistenciasBar++;
			break;
		}
	}
}

/*Cria um socket que será utilizado como cliente, mas que no entanto será
 * incialmente visto como servidor uma vez que fica em escuta.*/
void criaSocketCliente()
{
	/*int sockfd*/int servlen;
	struct sockaddr_un serv_addr;
	int n;
	char buffer[MAXLINE];

	/* Cria socket stream */

	if ((sockfd= socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		err_dump("client: can't open stream socket");


	bzero((char *) &serv_addr, sizeof(serv_addr)); // Limpa o socket coloca 0 em todas as posicoes de memoria
	serv_addr.sun_family = AF_UNIX; //Dominio que se vai utilizar

	strcpy(serv_addr.sun_path, UNIXSTR_PATH);
	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
	//Tenta ligar-se ao ao servidor
	if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
		err_dump("client: can't connect to server");
	//Fica à escuta do simulador
	for (;;)
	{
		bzero(buffer,MAXLINE);
		n = read(sockfd,buffer,MAXLINE);
		if (n < 0) error("ERROR reading from socket");

		if(!strcmp(buffer,"inicia"))
		{
			break;
		}
		else if(!strcmp(buffer,"finito"))
		{
			exit(1);
		}
	}

}

/*Cria um socket que incialmente irá atuar como cliente, mas que mais tarde será
 * será visto como servidor*/
void criaSocketServidor(){
	/*int sockfd,*/int newsockfd, clilen, childpid, servlen;
	struct sockaddr_un cli_addr, serv_addr;
	int n ;
	char option;
	/* Cria socket stream */
	if ((sockfd = socket(AF_UNIX,SOCK_STREAM,0)) < 0)
		err_dump("server: can't open stream socket");
	//Apontador pra buffer e tamanho buffer limpa o buffer
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sun_family = AF_UNIX;
	strcpy(serv_addr.sun_path, UNIXSTR_PATH);


	servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);
	//Elimina o socket pré existente
	unlink(UNIXSTR_PATH);
	//sockfd nome do socket
	if (bind(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
		err_dump("server, can't bind local address");

	listen(sockfd, 5);

	clilen = sizeof(cli_addr);
	printf("\n A aguardar pelo simulador ... \n");
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");

	//Interface para o utilizador
	option = menu();

	switch(option){
	case '1':
		//Envia comando para o simulador para que este inicie
		n = write(newsockfd,"inicia",6);
		if (n < 0){ error("ERROR writing to socket");}

		sem_post(&aguardaSimulStart);
		break;
	case '2':

		n = write(newsockfd,"finito",6);
		if (n < 0) error("ERROR writing to socket");
		close(newsockfd);
		close(sockfd);
		printf("Prima [ENTER] para terminar... \n");
		getchar();

		//exit(1);
		return;
	}

	//Depois irá ficar a escutar
	for(;;)
	{
		if(str_echo(newsockfd) == true)
		{
			break;
		};
	}

	//espera por enter termina
	close(newsockfd);
	close(sockfd);
	//exit(0);
	return;

}
