#include "header.h"
#include "sockets.h"

void *pistaNormal(int tipoPessoa,long id){
	int acao;
	int seccao;
	time_t horaChegada, horaEntrada;
	float tempoEspera;
	pthread_mutex_lock(&protSocket);
	acao = ESPERA;
	seccao = PISTA_NORMAL;
	str_cli(sockfd, tipoPessoa, acao, id, seccao);
	pthread_mutex_unlock(&protSocket);
	horaChegada = tempoAtual();
	sem_wait(&semNorm);				//Semáforo inicialiado com a lotacao maxima do salão de jogos controla a lotação
	horaEntrada = tempoAtual();
	tempoEspera = (float)(horaEntrada - horaChegada);
	if(tempoEspera <= converteSegundosReais(converteMinutos(esperaTempo.tempoNorm)))
	{
		pthread_mutex_lock(&protNorm);
		filaNormal--;
		lotDisc.lotNorm++;
		pthread_mutex_unlock(&protNorm);
		pthread_mutex_lock(&protSocket);
		acao = ENTROU;
		seccao = PISTA_NORMAL;
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
		//DAN�A
		usleep(50000);
		pthread_mutex_lock(&protNorm);
		lotDisc.lotNorm--;
		pthread_mutex_unlock(&protNorm);
		pthread_mutex_lock(&protSocket);
		acao = SAIU;
		seccao = PISTA_NORMAL;
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
		sem_post(&semNorm);
	}
	else
	{
		pthread_mutex_lock(&protNorm);
		filaNormal--;
		pthread_mutex_unlock(&protNorm);
		acao = DESISTIU;
		seccao = PISTA_NORMAL;
		pthread_mutex_lock(&protSocket);
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
	}
}

void pistaPP(int tipoPessoa,long id){
	int acao;
	int seccao;
	time_t horaChegada, horaEntrada;
	float tempoEspera;
	pthread_mutex_lock(&entraPP);
	//printf("Vou tentar ir pa PP\n");
	if(lotDisc.lotPool < lotMax.lotPool)
	{
		if(numFilaEsperaPP == condicaoAbertura)
		{
			pthread_mutex_unlock(&entraPP);
			sem_post(&deixaEntrarPP);					//Desbloqueio um dos 30 que estão bloqueados.
			//printf("CHegou o salvador");
			pthread_mutex_lock(&entraPP);
			tamFilaPP--;
			lotDisc.lotPool++;
			pthread_mutex_unlock(&entraPP);
			//incrementa lotação em falta
			//Printa Mensagem
			acao = ENTROU;
			seccao = PISTA_POOL_PARTY;
			pthread_mutex_lock(&protSocket);
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
			//Entra na PP
			sleep(5);

			//Decrementa o número de pessoas na pista.
			pthread_mutex_lock(&entraPP);

			acao = SAIU;
			seccao = PISTA_POOL_PARTY;
			pthread_mutex_lock(&protSocket);
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);

			if(lotDisc.lotPool == lotMax.lotPool)
			{
				sem_post(&lotacaoPPFull);
			}

			lotDisc.lotPool--;

			pthread_mutex_unlock(&entraPP);


		}
		else
		{
			//existe para a mensagem ser impressa nos primeiros 29 a dizer que esperam

			pthread_mutex_unlock(&entraPP);
			pthread_mutex_lock(&protSocket);
			acao = ESPERA;
			seccao = PISTA_POOL_PARTY;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);

			horaChegada = tempoAtual();
			sem_wait(&deixaEntrarPP);
			horaEntrada = tempoAtual();
			tempoEspera = (float)(horaEntrada - horaChegada);
			if(tempoEspera <= converteSegundosReais(converteMinutos(esperaTempo.tempoPool)))
			{
				sem_post(&deixaEntrarPP);		//avisa a tarefa seguinte que pode avançar.
				pthread_mutex_lock(&entraPP);
				tamFilaPP--;
				lotDisc.lotPool++;
				//printf("Lotação na sala %i \n",lotDisc.lotPool);
				pthread_mutex_unlock(&entraPP);
				//msg print
				acao = ENTROU;
				seccao = PISTA_POOL_PARTY;
				pthread_mutex_lock(&protSocket);
				str_cli(sockfd, tipoPessoa, acao, id, seccao);
				pthread_mutex_unlock(&protSocket);
				//Entra na PP;
				sleep(5);

				//Decrementa o número de pessoas na pista.
				pthread_mutex_lock(&entraPP);
				acao = SAIU;
				seccao = PISTA_POOL_PARTY;
				pthread_mutex_lock(&protSocket);
				str_cli(sockfd, tipoPessoa, acao, id, seccao);
				pthread_mutex_unlock(&protSocket);

				if(lotDisc.lotPool == lotMax.lotPool)
				{
					sem_post(&lotacaoPPFull);
				}



				lotDisc.lotPool--;

				pthread_mutex_unlock(&entraPP);

			}
			else
			{
				sem_post(&deixaEntrarPP); //mesmo que desista diz as tarefas seguintes para entrar.
				pthread_mutex_lock(&entraPP);
				tamFilaPP--;
				pthread_mutex_unlock(&entraPP);
				//printf("Sou fino e já esperei muito! ADEUS! \n");
				acao = DESISTIU;
				seccao = PISTA_POOL_PARTY;
				pthread_mutex_lock(&protSocket);
				str_cli(sockfd, tipoPessoa, acao, id, seccao);
				pthread_mutex_unlock(&protSocket);
			}
		}
	}
	else
	{
		pthread_mutex_unlock(&entraPP);
		pthread_mutex_lock(&protSocket);
		acao = ESPERA;
		seccao = PISTA_POOL_PARTY;
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
		horaChegada = tempoAtual();
		sem_wait(&lotacaoPPFull);
		horaEntrada = tempoAtual();
		tempoEspera = (float)(horaEntrada - horaChegada);
		if(tempoEspera <= converteSegundosReais(converteMinutos(esperaTempo.tempoPool)))
		{
			pistaPP(tipoPessoa,id);
		}
		else
		{
			pthread_mutex_lock(&entraPP);
			tamFilaPP--;
			pthread_mutex_unlock(&entraPP);
			acao = DESISTIU;
			seccao = PISTA_POOL_PARTY;
			pthread_mutex_lock(&protSocket);
			//printf("Sou fino e já esperei muito! ADEUS! \n");
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
		}
	}
}

void *pistaDS(int tipoPessoa,long id){
	int acao;
	int seccao;
	time_t horaChegada, horaEntrada;
	float tempoEspera;

	pthread_mutex_lock(&protSocket);
	acao = ESPERA;
	seccao = PISTA_DANCA_SALAO;
	str_cli(sockfd, tipoPessoa, acao, id, seccao);
	pthread_mutex_unlock(&protSocket);
	horaChegada = tempoAtual();
	//Assegura que so estarão 2 pessoas a dançar
	sem_wait(&fechaDanSalao); //Semaforo iniciado a 2 que vai deixar passar smp 2 tarefas 2 de cd vez
	horaEntrada = tempoAtual();
	tempoEspera = (float)(horaEntrada - horaChegada);
	if(tempoEspera <= converteSegundosReais(converteMinutos(esperaTempo.tempoPool)))
	{
		pthread_mutex_lock(&protNDDanSalao);
		numFilaDD--;
		numDancers ++;
		//primeiro fica block
		if(numDancers == 1)
		{
			pthread_mutex_unlock(&protNDDanSalao);

			pthread_mutex_lock(&protSocket);
			acao = ESPERA_PARCEIRO;
			seccao = PISTA_DANCA_SALAO;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);

			sem_wait(&esperaParceiro);

			pthread_mutex_lock(&protSocket);
			acao = ENTROU;
			seccao = PISTA_DANCA_SALAO;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);

			//Dança
			usleep(50000);
		}
		else if(numDancers == 2)//segundo desblock o primeiro;
		{
			pthread_mutex_unlock(&protNDDanSalao);

			pthread_mutex_lock(&protSocket);
			acao = ENTROU;
			seccao = PISTA_DANCA_SALAO;
			str_cli(sockfd, tipoPessoa, acao, id,seccao);
			pthread_mutex_unlock(&protSocket);

			sem_post(&esperaParceiro);
			//Dança
			usleep(50000);
		}
		else
		{
			//If some thread enters here there will be an error with semaphores
			printf("BUG: ERROR #1");
		}

		//o ultimo a sair assinala duas vezes
		pthread_mutex_lock(&protNDDanSalao);
		//quando saiem dizem que querem sair
		numDancers--;
		pthread_mutex_lock(&protSocket);
		acao = SAIU;
		seccao = PISTA_DANCA_SALAO;
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
		if(numDancers == 0)
		{
			sem_post(&fechaDanSalao);
			sem_post(&fechaDanSalao);
		}

		pthread_mutex_unlock(&protNDDanSalao);
	}
	else
	{
		pthread_mutex_lock(&protNDDanSalao);
		numFilaDD--;
		pthread_mutex_unlock(&protNDDanSalao);
		acao = DESISTIU;
		seccao = PISTA_POOL_PARTY;
		pthread_mutex_lock(&protSocket);
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
	}
}

void *bar(int tipoPessoa,long id){
	int acao;
	int seccao;

	pthread_mutex_lock(&protSocket);
	acao = ESPERA;
	seccao = BAR;
	str_cli(sockfd, tipoPessoa, acao, id, seccao);
	pthread_mutex_unlock(&protSocket);

	sem_wait(&canConsume);

	pthread_mutex_lock(&protbalcao);
	balcao[consPosition] = 0;				//retira um produto do balcao
	consPosition = (consPosition+1) % 10;

	pthread_mutex_lock(&protSocket);
	acao = ENTROU;
	seccao = BAR;
	str_cli(sockfd, tipoPessoa, acao, id, seccao);
	pthread_mutex_unlock(&protSocket);
	usleep(5000);
	pthread_mutex_unlock(&protbalcao);
	sem_post(&canProduce);

	pthread_mutex_lock(&protSocket);
	acao = SAIU;
	seccao = BAR;
	str_cli(sockfd, tipoPessoa, acao, id, seccao);
	pthread_mutex_unlock(&protSocket);
}

void *zonaSofas(int tipoPessoa,long id){
	int acao;
	int seccao;
	pthread_mutex_lock(&protZonaSofas);
	if(lotDisc.lotSofas < lotMax.lotSofas)
	{
		lotDisc.lotSofas++;
		pthread_mutex_unlock(&protZonaSofas);

		pthread_mutex_lock(&protSocket);
		acao = ENTROU;
		seccao = ZONA_SOFAS;
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);

		sleep(2);

		pthread_mutex_lock(&protSocket);
		acao = SAIU;
		seccao = ZONA_SOFAS;
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
		pthread_mutex_lock(&protZonaSofas);
		lotDisc.lotSofas--;
		pthread_mutex_unlock(&protZonaSofas);
	}
	else
	{
		pthread_mutex_unlock(&protZonaSofas);
		acao = DESISTIU;
		seccao = ZONA_SOFAS;
		pthread_mutex_lock(&protSocket);
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
	}
}

void *snackBar(int tipoPessoa,long id)
{
	int acao;
	int seccao;
	pthread_mutex_lock(&funcionamentoDisc);
	if(abreBar < tempoAtual())
	{
		//printf("Tempo de abertura %i \n",abreBar);
		//printf("Tempo Atual %i \n",tempoAtual());
		pthread_mutex_unlock(&funcionamentoDisc);
		acao = ENTROU;
		seccao = SNACK_BAR;
		pthread_mutex_lock(&protSocket);
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);

		sem_post(&esperaCliSB);	//Avisa que está à espera de ser servido
		pthread_mutex_lock(&protSocket);
		acao = ESPERA;
		seccao = SNACK_BAR;
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
		sem_wait(&esperaSerServido);	//Espera até ser servido.

		pthread_mutex_lock(&protCliFilasBar);
		cliFilaSB--;
		pthread_mutex_unlock(&protCliFilasBar);
		acao = SAIU;
		seccao = SNACK_BAR;
		pthread_mutex_lock(&protSocket);
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
	}
	else
	{
		pthread_mutex_unlock(&funcionamentoDisc);
		pthread_mutex_lock(&protCliFilasBar);
		cliFilaSB--;
		pthread_mutex_unlock(&protCliFilasBar);
		//printf("Snack Bar está fechado e eu tenho fome! :(\n");
		//printf("Tempo de abertura %i \n",abreBar);
		//printf("Tempo Atual %i \n",tempoAtual());
	}
}

void *salaJogos(int tipoPessoa,long id){

	int acao;
	int seccao;
	time_t horaChegada, horaEntrada;
	float tempoEspera;
	pthread_mutex_lock(&funcionamentoDisc);
	if(fechaJogos > tempoAtual())
	{
		pthread_mutex_unlock(&funcionamentoDisc);
		pthread_mutex_lock(&protSocket);
		acao = ESPERA;
		seccao = SALA_JOGOS;
		str_cli(sockfd, tipoPessoa, acao, id, seccao);
		pthread_mutex_unlock(&protSocket);
		horaChegada = tempoAtual();
		sem_wait(&semJogos);				//Semáforo inicialiado com a lotacao maxima do salão de jogos controla a lotação
		horaEntrada = tempoAtual();
		tempoEspera = (float)(horaEntrada - horaChegada);
		if(tempoEspera <= converteSegundosReais(converteMinutos(esperaTempo.tempoPool)))
		{
			pthread_mutex_lock(&protJogos);
			filaJogos--;
			lotDisc.lotJogos++;
			pthread_mutex_unlock(&protJogos);
			pthread_mutex_lock(&protSocket);
			acao = ENTROU;
			seccao = SALA_JOGOS;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
			//JOGA
			usleep(5000000);
			pthread_mutex_lock(&protJogos);
			lotDisc.lotJogos--;
			pthread_mutex_unlock(&protJogos);
			pthread_mutex_lock(&protSocket);
			acao = SAIU;
			seccao = SALA_JOGOS;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
			sem_post(&semJogos);
		}
		else
		{
			pthread_mutex_lock(&protJogos);
			filaJogos--;
			pthread_mutex_unlock(&protJogos);
			acao = DESISTIU;
			seccao = PISTA_POOL_PARTY;
			pthread_mutex_lock(&protSocket);
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
		}
	}
	else
	{
		pthread_mutex_lock(&protJogos);
		filaJogos--;
		pthread_mutex_unlock(&protJogos);
		pthread_mutex_unlock(&funcionamentoDisc);
		//printf("Ohhhhhhhhhhh a sala de jogos está fechada! :(\n");
		//printf("Tempo de abertura %i \n",abreBar);
		//printf("Tempo Atual %i \n",tempoAtual());
	}
}

