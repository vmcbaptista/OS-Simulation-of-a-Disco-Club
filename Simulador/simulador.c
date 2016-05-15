#include "../lib/header.h"
#include "../lib/sockets.h"

int checkEntrou = 0;

void *clienteNormal (pthread_t* appthread);
void *clienteVIP(pthread_t* appthread);
void *funcionarioSnackBar(pthread_t* appthread);
void *funcionarioBar(pthread_t* appthread);

//inicializa a estrutura com as lotacoes das sala a cada instante
void inicializacoes()
{
	lotDisc.lotTotal = 0;
	lotDisc.lotNorm = 0;
	lotDisc.lotDS = 0;		//Lotação VIP
	lotDisc.lotNorm = 0;		//Lotação Normal (Todos têm acesso)
	lotDisc.lotPool = 0; 		//Lotação Pool Party
	lotDisc.lotSofas = 0;		//Lotação Sofás
	lotDisc.lotJogos = 0;		//Lotação Sala Jogos
	lotDisc.lotSnackBar = 0;	//Lotação Snack Bar
	lotDisc.lotBar = 0;			//Lotação Bar

	pthread_mutex_init(&funcionamentoDisc,NULL);

	pthread_mutex_init(&protEntrada,NULL);
	pthread_mutex_init(&protSocket,NULL);
	sem_init(&normais,0,0);
	normalNaFila= 0;
	vipNaFila=0;
	//Quando a fila fica cheia os vips e normais ficam bloqueados.
	sem_init(&vipwaitDiscoFull,0,0);
	sem_init(&normwaitDiscoFull,0,0);


	//Semaforos e trincos dancas de salao
	sem_init(&fechaDanSalao,0,2);
	sem_init(&esperaParceiro,0,0);
	pthread_mutex_init(&protNDDanSalao,NULL);
	numDancers =0;
	numFilaDD = 0;

	//Semáforos e trincos para a Pista Normal
		pthread_mutex_init(&protNorm,NULL);
		sem_init(&semNorm,0,lotMax.lotNorm);
		filaNormal = 0;

	//Produtores consumidores
	sem_init(&canConsume,0,0);
	sem_init(&canProduce,0,10);
	pthread_mutex_init(&protbalcao,NULL);
	consPosition = 0;
	prodPosition = 0;

	//Pool Party
	sem_init(&deixaEntrarPP,0,0);
	sem_init(&lotacaoPPFull,0,0);
	pthread_mutex_init(&entraPP,NULL);
	tamFilaPP = 0;
	numFilaEsperaPP = 0;
	//Snack Bar
	pthread_mutex_init(&protCliFilasBar,NULL);
	sem_init(&esperaCliSB,0,0);
	sem_init(&esperaSerServido,0,0);
	cliFilaSB = 0;
	//Semáforos e trincos para a zona de Jogos
	pthread_mutex_init(&protJogos,NULL);
	sem_init(&semJogos,0,lotMax.lotJogos);
	filaJogos = 0;

	//Semáforos e trincos para a zona de sofas
	pthread_mutex_init(&protJogos,NULL);
}
/**/

void simulaDiscoteca(int tipoPessoa,long id)
{
	int fatoBanho = 0;
	int acao;
	int seccao,norm, segundosDisc;
	time_t tempoNaDiscoteca;


	int n = rand()%100;

	if(n < 10)
	{
		fatoBanho = 1;		//fatoBanho a 1 signifisimulaDiscotecaca que o cliente tem fato de banho
	}
	if(fatoBanho)
	{
		pthread_mutex_lock(&entraPP);
		tamFilaPP++;
		if(tamFilaPP < limitePessoasFila.limitePool)
		{
			numFilaEsperaPP++;
			pthread_mutex_unlock(&entraPP);
			pistaPP(tipoPessoa,id);

		}
		else
		{

			pthread_mutex_unlock(&entraPP);
			acao = DESISTIU;
			seccao = PISTA_POOL_PARTY;
			pthread_mutex_lock(&protSocket);
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
		}
	}
	else
	{
		segundosDisc = rand() % TEMPO_SIMULACAO + 1;
		tempoNaDiscoteca = calculaTempo(segundosDisc);

		while(!verificaPassagemTempo(tempoNaDiscoteca))
		{

			norm = (rand() % 7) + 1;

			if(norm == PISTA_DANCA_SALAO)
			{
				pthread_mutex_lock(&protNDDanSalao);
				numFilaDD++;

				if(numFilaDD < limitePessoasFila.limiteDD)
				{
					pthread_mutex_unlock(&protNDDanSalao);
					pistaDS(tipoPessoa, id);
				}
				else
				{
					pthread_mutex_unlock(&protNDDanSalao);
					pthread_mutex_lock(&protSocket);
					acao = DESISTIU;
					seccao = PISTA_DANCA_SALAO;
					str_cli(sockfd, tipoPessoa, acao, id, seccao);
					pthread_mutex_unlock(&protSocket);
				}
			}
			else if(norm == PISTA_NORMAL)
			{
				pthread_mutex_lock(&protNorm);
			    filaNormal++;
			    if (filaNormal < limitePessoasFila.limiteNorm)
			    {
			    	pthread_mutex_unlock(&protNorm);
			    	pistaNormal (tipoPessoa, id);
			    }
			    else
			    {
			    	filaNormal--;
			    	pthread_mutex_unlock(&protNorm);
			    	pthread_mutex_lock(&protSocket);
			    	acao = DESISTIU;
			    	seccao = PISTA_NORMAL;
			    	str_cli(sockfd, tipoPessoa, acao, id, seccao);
			    	pthread_mutex_unlock(&protSocket);
			    }
			}
			else if(norm == ZONA_SOFAS)
			{
				zonaSofas(tipoPessoa, id);

			}
			else if(norm == SALA_JOGOS)
			{
				pthread_mutex_lock(&protJogos);
				filaJogos++;
				if(filaJogos < limitePessoasFila.limiteJogos)
				{
					pthread_mutex_unlock(&protJogos);
					salaJogos(tipoPessoa, id);
				}
				else
				{
					filaJogos--;
					pthread_mutex_unlock(&protJogos);
					pthread_mutex_lock(&protSocket);
					acao = DESISTIU;
					seccao = SALA_JOGOS;
					str_cli(sockfd, tipoPessoa, acao, id, seccao);
					pthread_mutex_unlock(&protSocket);

				}

			}
			else if(norm == BAR)
			{

				bar(tipoPessoa,id);

			}
			else
			{
				pthread_mutex_lock(&protCliFilasBar);
				cliFilaSB++;

				if(cliFilaSB < limitePessoasFila.limiteSnackBar)
				{
					pthread_mutex_unlock(&protCliFilasBar);
					snackBar(tipoPessoa,id);
				}
				else
				{
					cliFilaSB--;
					pthread_mutex_unlock(&protCliFilasBar);
					acao = DESISTIU;
					seccao = SNACK_BAR;
					pthread_mutex_lock(&protSocket);
					str_cli(sockfd, tipoPessoa, acao, id, seccao);
					pthread_mutex_unlock(&protSocket);
				}

			}
		}
	}
}
void entrouNaDiscoteca(long id,int tipoPessoa)
{
	int acao = ENTROU;
	int seccao = DISCOTECA;

	pthread_mutex_lock(&protSocket);
	str_cli(sockfd, tipoPessoa, acao, id, seccao);
	pthread_mutex_unlock(&protSocket);

	checkEntrou++;
	lotDisc.lotTotal++;
}
/*aso a discoteca esteja cheia, liberta sempre uma tarefa, claramente priorizando os vips*/
void liberta_apos_sair()
{
	if(vipNaFila > 0)
	{
		sem_post(&vipwaitDiscoFull);
	}
	else if(normalNaFila > 0)
	{
		sem_post(&normwaitDiscoFull);
	}

}


void * criaPessoa(pthread_t* appthread)
{
	int i = 0;
	time_t tempoPretendido = calculaTempo(TEMPO_SIMULACAO); // NAO ESQUECER SUBSTITUIR DPS POR TEMPO_SIMULACAO
	pthread_mutex_lock(&funcionamentoDisc);
	abreBar = calculaTempo(converteSegundosReais(converteMinutos(funcionamento.aberturaSB)));
	fechaJogos = calculaTempo(converteSegundosReais(converteMinutos(funcionamento.encerramentoJogos)));
	pthread_mutex_unlock(&funcionamentoDisc);
	while (!verificaPassagemTempo(tempoPretendido))
	{


		int r1  = (rand() % 100 ) +1; //NUmeros aleatorios entre 1 e 100
		for(int i = 0; i < 5000000; i++); //consome tempo
		if(r1 > 1)
		{
			pthread_t  buffer;
			buffer = malloc (sizeof (pthread_t));
			if(buffer != NULL)
			{
				i++;
				//printf("Criou Tarefa.");
				int n =  pthread_create(buffer, NULL,  clienteNormal, buffer);
				if(n)
				{
					printf("Erro ao criar tarefa ERROR: %i, Threads Criadas: %i. \n",n,i);
					exit(1);
				}
			}
			else
			{
				printf("Erro ao alocar memoria");
			}
		}
		if(r1 == 1){
			pthread_t  buffer;
			buffer = malloc (sizeof (pthread_t));
			if(buffer != NULL)
			{
				i++;
				//printf("Criou Tarefa.");
				int n =  pthread_create(buffer, NULL, clienteVIP, buffer);
				if(n)
				{
					printf("Erro ao criar tarefa ERROR: %i, Threads Criadas: %i. \n",n,i);
					exit(1);
				}
			}
			else
			{
				printf("Erro ao alocar memoria");
			}
		}

	}
	printf("Threads Criadas: %i. \n",i);
}


/*Rotina que todos os clientes normais vão executar*/
void *clienteNormal (pthread_t* appthread){

	int tipoPessoa = NORMAL;
	int acao;
	long id = pthread_self();
	int seccao;
	int norm;
	time_t horaChegada, horaEntrada;
	float tempoEspera;
	pthread_mutex_lock(&protEntrada);
	normalNaFila++;
	pthread_mutex_unlock(&protEntrada);
	pthread_mutex_lock(&protSocket);
	acao = CHEGOU;
	seccao = DISCOTECA;
	str_cli(sockfd,tipoPessoa,acao,id,seccao);
	pthread_mutex_unlock(&protSocket);
	pthread_mutex_lock(&protEntrada);
	if(lotDisc.lotTotal < lotMax.lotTotal)
	{
		if(vipNaFila > 0)
		{
			pthread_mutex_unlock(&protEntrada);
			pthread_mutex_lock(&protSocket);
			acao = ESPERA;
			seccao = DISCOTECA;
			str_cli(sockfd,tipoPessoa,acao,id,seccao);
			pthread_mutex_unlock(&protSocket);
			horaChegada = tempoAtual();
			sem_wait(&normais);						//Fica bloqueado em espera
			tempoEspera = (float)(horaEntrada - horaChegada);
			//Verifica se já passou o tempo máximo que o cliente deve esperar.
			if(tempoEspera <= converteSegundosReais(converteMinutos(esperaTempo.tempoEntrada)))
			{
				pthread_mutex_lock(&protEntrada);
				normalNaFila--;
				//Se não existem vips na fila mas existem pessoas normais
				if(vipNaFila == 0 && normalNaFila >= 1)
				{
					sem_post(&normais);
					pthread_mutex_unlock(&protEntrada);
				}
				else
				{
					pthread_mutex_unlock(&protEntrada);
				}
				//entra na discoteca

				pthread_mutex_lock(&protEntrada);
				entrouNaDiscoteca(id,tipoPessoa);

				pthread_mutex_unlock(&protEntrada);
				simulaDiscoteca(tipoPessoa,id);
				//Saiu da discoteca
				pthread_mutex_lock(&protSocket);
				acao = SAIU;
				seccao = DISCOTECA;
				str_cli(sockfd, tipoPessoa, acao, id, seccao);
				pthread_mutex_unlock(&protSocket);
				pthread_mutex_lock(&protEntrada);
				lotDisc.lotTotal--;
				liberta_apos_sair();
				pthread_mutex_unlock(&protEntrada);
			}
			else
			{

				//Dessitiu da fila portanti saí
				pthread_mutex_lock(&protEntrada);
				normalNaFila--;
				pthread_mutex_unlock(&protEntrada);

				acao = DESISTIU;
				seccao = DISCOTECA;
				pthread_mutex_lock(&protSocket);
				str_cli(sockfd, tipoPessoa, acao, id, seccao);
				pthread_mutex_unlock(&protSocket);
			}
		}
		else
		{
			//entra logo
			normalNaFila--;
			entrouNaDiscoteca(id,tipoPessoa);
			pthread_mutex_unlock(&protEntrada);
			simulaDiscoteca(tipoPessoa,id);
			//Saiu da discoteca
			pthread_mutex_lock(&protSocket);
			acao = SAIU;
			seccao = DISCOTECA;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
			pthread_mutex_lock(&protEntrada);
			lotDisc.lotTotal--;
			liberta_apos_sair();
			pthread_mutex_unlock(&protEntrada);
		}
	}
	else
	{
		pthread_mutex_unlock(&protEntrada);

		//norm ficam a espera que saia alguem.
		pthread_mutex_lock(&protSocket);
		acao = ESPERA;
		seccao = DISCOTECA;
		str_cli(sockfd,tipoPessoa,acao,id,seccao);
		pthread_mutex_unlock(&protSocket);
		horaChegada = tempoAtual();
		sem_wait(&normwaitDiscoFull);					//Fica bloqueado em espera
		horaEntrada = tempoAtual();
		tempoEspera = (float)(horaEntrada - horaChegada);
		//Verifica se já passou o tempo máximo que o cliente deve esperar.
		if(tempoEspera <= converteSegundosReais(converteMinutos(esperaTempo.tempoEntrada)))
		{


			pthread_mutex_lock(&protEntrada);
			normalNaFila--;

			entrouNaDiscoteca(id,tipoPessoa);
			pthread_mutex_unlock(&protEntrada);
			simulaDiscoteca(tipoPessoa,id);
			//Saiu da discoteca.
			pthread_mutex_lock(&protSocket);
			acao = SAIU;
			seccao = DISCOTECA;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
			pthread_mutex_lock(&protEntrada);
			lotDisc.lotTotal--;
			liberta_apos_sair();
			pthread_mutex_unlock(&protEntrada);
		}
		else
		{

			pthread_mutex_lock(&protEntrada);
			normalNaFila--;
			pthread_mutex_unlock(&protEntrada);
			acao = DESISTIU;
			seccao = DISCOTECA;
			pthread_mutex_lock(&protSocket);
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);
		}
	}

	free(appthread);
}

/*Rotina que todos os clientes vip da discote vão executar*/
void *clienteVIP(pthread_t* appthread){

	int tipoPessoa = VIP;
	int acao;
	long id = pthread_self();
	int seccao, norm;
	int fatoBanho;
	time_t horaChegada, horaEntrada;
	float tempoEspera;
	pthread_mutex_lock(&protEntrada);
	vipNaFila++;
	pthread_mutex_unlock(&protEntrada);
	pthread_mutex_lock(&protSocket);
	acao = CHEGOU;
	seccao = DISCOTECA;
	str_cli(sockfd,tipoPessoa,acao,id,seccao);
	pthread_mutex_unlock(&protSocket);
	//perdeCPU();
	pthread_mutex_lock(&protEntrada);
	if(lotDisc.lotTotal < lotMax.lotTotal)
	{
		vipNaFila--;
		//diz para os normais entrarem e e o vip entra porque não existem vips na fila
		if(vipNaFila == 0)
		{
			entrouNaDiscoteca(id,tipoPessoa);
			pthread_mutex_unlock(&protEntrada);

			sem_post(&normais);
			simulaDiscoteca(tipoPessoa,id);

			pthread_mutex_lock(&protSocket);
			acao = SAIU;
			seccao = DISCOTECA;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);

			pthread_mutex_lock(&protEntrada);
			lotDisc.lotTotal--;
			liberta_apos_sair();
			pthread_mutex_unlock(&protEntrada);
		}
		//o vip entra.
		else
		{

			entrouNaDiscoteca(id,tipoPessoa);
			pthread_mutex_unlock(&protEntrada);
			simulaDiscoteca(tipoPessoa,id);

			pthread_mutex_lock(&protSocket);
			acao = SAIU;
			seccao = DISCOTECA;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);

			pthread_mutex_lock(&protEntrada);
			lotDisc.lotTotal--;
			liberta_apos_sair();
			pthread_mutex_unlock(&protEntrada);
		}
	}
	else
	{
		pthread_mutex_unlock(&protEntrada);
		//Vips ficam a espera que saia alguem.
		pthread_mutex_lock(&protSocket);

		acao = ESPERA;
		seccao = DISCOTECA;
		str_cli(sockfd,tipoPessoa,acao,id,seccao);
		pthread_mutex_unlock(&protSocket);
		horaChegada = tempoAtual();
		sem_wait(&vipwaitDiscoFull);
		horaEntrada = tempoAtual();
		tempoEspera = (float)(horaEntrada - horaChegada);
		if(tempoEspera <= converteSegundosReais(converteMinutos(esperaTempo.tempoEntrada)))
		{
			pthread_mutex_lock(&protEntrada);
			vipNaFila--;
			entrouNaDiscoteca(id,tipoPessoa);
			pthread_mutex_unlock(&protEntrada);
			simulaDiscoteca(tipoPessoa,id);

			pthread_mutex_lock(&protSocket);
			acao = SAIU;
			seccao = DISCOTECA;
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);

			pthread_mutex_lock(&protEntrada);
			lotDisc.lotTotal--;
			liberta_apos_sair();
			pthread_mutex_unlock(&protEntrada);
		}
		else
		{
			pthread_mutex_lock(&protEntrada);
			vipNaFila--;
			pthread_mutex_unlock(&protEntrada);
			acao = DESISTIU;
			seccao = DISCOTECA;
			pthread_mutex_lock(&protSocket);
			str_cli(sockfd, tipoPessoa, acao, id, seccao);
			pthread_mutex_unlock(&protSocket);

		}

	}

	//tempo de espera
	free(appthread);
}

void *funcionarioBar(pthread_t* appthread){
	while(true)
	{
		int tipoPessoa = EMPREGADO_BAR;
		int acao = PRODUZIU;
		int id = pthread_self();
		int seccao = BAR;
		sem_wait(&canProduce);
		pthread_mutex_lock(&protbalcao);
		//Produção
		balcao[prodPosition] = 1; // Produziu uma unidade
		prodPosition = (prodPosition + 1 ) % 10;
		pthread_mutex_lock(&protSocket);
		str_cli(sockfd,tipoPessoa,acao,id,seccao);
		pthread_mutex_unlock(&protSocket);
		usleep(5000);
		pthread_mutex_unlock(&protbalcao);
		sem_post(&canConsume);
	}
}

void *funcionarioSnackBar(pthread_t* appthread){
	while(true)
	{
		sem_wait(&esperaCliSB);		//ESpera que o cliente chegue
		//Atende cliente
		usleep(5000);
		pthread_mutex_lock(&protCliFilasBar);
		sem_post(&esperaSerServido); //Avisa o cliente quepode ir embora
		cliFilaSB--;
		pthread_mutex_unlock(&protCliFilasBar);
	}
}

void* criaFuncionarios()
{
	int i = 0;
	for(int i = 0; i< 10; i++)
	{
		pthread_t  buffer;
		buffer = malloc (sizeof (pthread_t));
		if(buffer != NULL)
		{
			i++;
			//printf("Criou Tarefa.");
			int n =  pthread_create(buffer, NULL,  funcionarioBar, buffer);
			if(n)
			{
				printf("Erro ao criar tarefa ERROR: %i, Threads Criadas: %i. \n",n,i);
				exit(1);
			}
		}
		else
		{
			printf("Erro ao alocar memoria");
		}
	}
	pthread_t  buffer;
	buffer = malloc (sizeof (pthread_t));
	if(buffer != NULL)
	{
		i++;
		//printf("Criou Tarefa.");
		int n =  pthread_create(buffer, NULL, funcionarioSnackBar, buffer);
		if(n)
		{
			printf("Erro ao criar tarefa ERROR: %i, Threads Criadas: %i. \n",n,i);
			exit(1);
		}
	}
	else
	{
		printf("Erro ao alocar memoria");
	}
}


int main(int argc,char* argv[])
{

	int i = 0;
	char * fconfig;


	if(argc == 1)
	{
		printf("Este programa requer introdução do ficheiro de configuração.");
		return 1;
	}
	else if (argc == 2 && strcmp(argv[1],"simulador.cfg") == 0)
	{
		fconfig = argv[1];
		carregaSimulador(fconfig);
		//Carrega parametros para as mensagens

		//Abre Socket
		criaSocketCliente();
		inicializacoes();
		printf("A discoteca abriu \n");
		pthread_t criadora;
		criaFuncionarios();
		//Inicia tarefa criadora de tarefas
		int n = pthread_create(&criadora,NULL,criaPessoa,NULL);
		if(n)
		{
			printf("Não foi possivel criar uma tarefa. ERRO %i",n);
			exit(1);
		}
		//a main espera que a tarefa que cria termina
		pthread_join(criadora,NULL);
		//Simulador avisa o monitor que já terminou.
		str_cliTermina(sockfd);
		pthread_mutex_lock(&protEntrada);
		printf("%i \n",checkEntrou);
		pthread_mutex_unlock(&protEntrada);

		// Destruir trincos e semáforos
		sem_destroy(&vipwaitDiscoFull);
		sem_destroy(&normwaitDiscoFull);
		sem_destroy(&normais);
		sem_destroy(&fechaDanSalao);
		sem_destroy(&esperaParceiro);
		sem_destroy(&semJogos);
		sem_destroy(&semNorm);
		sem_destroy(&deixaEntrarPP);
		sem_destroy(&lotacaoPPFull);
		sem_destroy(&esperaCliSB);
		sem_destroy(&esperaSerServido);
		sem_destroy(&canProduce);
		sem_destroy(&canConsume);

		pthread_mutex_destroy(&protNDDanSalao);
		pthread_mutex_destroy(&protJogos);
		pthread_mutex_destroy(&protNorm);
		pthread_mutex_destroy(&protZonaSofas);
		pthread_mutex_destroy(&protSocket);
		pthread_mutex_destroy(&protEntrada);
		pthread_mutex_destroy(&entraPP);
		pthread_mutex_destroy(&protCliFilasBar);
		pthread_mutex_destroy(&protbalcao);


		printf("Discoteca fechou \n");


		return 0;
	}
	else
	{
		printf("O nome do ficheiro introduzido não é o esperado.");
		return 1;
	}

}

