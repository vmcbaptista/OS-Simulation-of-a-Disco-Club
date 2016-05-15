#include "../lib/header.h"
#include "../lib/sockets.h"

void * apresentaStats()
{
	sem_wait(&aguardaSimulStart);
	printf("%i\n", TEMPO_SIMULACAO);
	for(int i = 0; i < TEMPO_SIMULACAO / 5; i++)
	{
		pthread_mutex_lock(&protStats);
		printf("\n");
		printf(apGlobMsgMonitor->indicaEstado);
		printf("\n");
		printf(apGlobMsgMonitor->simDecorre);
		printf("\n");
		printf("%s tentaram entrar %i ",apGlobMsgMonitor->nUtilizadores,nClientesDia);
		printf("\n");
		printf("Utilizadores que entraram na discoteca: %i ", nClientesEntraramDisc);
		printf("\n");
		printf("Saidas da discoteca: %i ",nClientesSairamDisc);
		printf("\n");
		printf("%s %i ",apGlobMsgMonitor->nDesistencias, desistenciasDisc);
		printf("\n");
		printf("\n");
		printf("Pista Normal\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramPistaNormal);
		printf("Utilizadores que sairam: %i \n", nClientesSairamPistaNormal);
		printf("Utilizadores que desistiram: %i \n", desistenciasNormal);
		printf("\n");
		printf("Pista Dancas Salao\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramDS);
		printf("Utilizadores que sairam: %i \n", nClientesSairamDS);
		printf("Utilizadores que desistiram: %i \n", desistenciasDS);
		printf("\n");
		printf("Pista Pool Party\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramPP);
		printf("Utilizadores que sairam: %i \n", nClientesSairamPP);
		printf("Utilizadores que desistiram: %i \n", desistenciasPP);
		printf("\n");
		printf("Bar\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramBar);
		printf("Utilizadores que sairam: %i \n", nClientesSairamBar);
		printf("Utilizadores que desistiram: %i \n", desistenciasBar);
		printf("\n");
		printf("Snack Bar\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramSB);
		printf("Utilizadores que sairam: %i \n", nClientesSairamSB);
		printf("Utilizadores que desistiram: %i \n", desistenciasSB);
		printf("\n");
		printf("Sala de Jogos\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramSJ);
		printf("Utilizadores que sairam: %i \n", nClientesSairamSJ);
		printf("Utilizadores que desistiram: %i \n", desistenciasSJ);
		printf("\n");
		printf("Zona de Sofas\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramZS);
		printf("Utilizadores que sairam: %i \n", nClientesSairamZS);
		printf("Utilizadores que desistiram: %i \n", desistenciasZS);
		pthread_mutex_unlock(&protStats);
		sleep(5);
	}
}


int main(int argc,char* argv[])
{
	char * fconfig;
	if(argc == 1)
	{
		printf("Este programa requer introdução do ficheiro de configuração.");
		return 1;
	}
	else if (argc == 2 && strcmp(argv[1],"monitor.cfg") == 0)
	{
		char opcao;
		fconfig = argv[1];
		apGlobMsgMonitor = carregaMonitor(fconfig);
		//Abre o ficheiro onde se vai escrever os logs
		logPointer = abreFicheiro("logs.txt");
		pthread_mutex_init(&protStats,NULL);
		sem_init(&aguardaSimulStart,0,0);

		pthread_t criadora;
		pthread_create(&criadora, NULL,apresentaStats,NULL);

		criaSocketServidor();
		//Estatisticas

		pthread_mutex_lock(&protStats);
		printf("\n");
		printf(apGlobMsgMonitor->indicaEstado);
		printf("\n");
		printf(apGlobMsgMonitor->simTerminada);
		printf("\n");
		printf("%s tentaram entrar %i ",apGlobMsgMonitor->nUtilizadores,nClientesDia);
		printf("\n");
		printf("Utilizadores que entraram na discoteca: %i ", nClientesEntraramDisc);
		printf("\n");
		printf("Saidas da discoteca: %i ",nClientesSairamDisc);
		printf("\n");
		printf("%s %i ",apGlobMsgMonitor->nDesistencias, desistenciasDisc);
		printf("\n");
		printf("\n");
		printf("Pista Normal\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramPistaNormal);
		printf("Utilizadores que sairam: %i \n", nClientesSairamPistaNormal);
		printf("Utilizadores que desistiram: %i \n", desistenciasNormal);
		printf("\n");
		printf("Pista Dancas Salao\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramDS);
		printf("Utilizadores que sairam: %i \n", nClientesSairamDS);
		printf("Utilizadores que desistiram: %i \n", desistenciasDS);
		printf("\n");
		printf("Pista Pool Party\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramPP);
		printf("Utilizadores que sairam: %i \n", nClientesSairamPP);
		printf("Utilizadores que desistiram: %i \n", desistenciasPP);
		printf("\n");
		printf("Bar\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramBar);
		printf("Utilizadores que sairam: %i \n", nClientesSairamBar);
		printf("Utilizadores que desistiram: %i \n", desistenciasBar);
		printf("\n");
		printf("Snack Bar\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramSB);
		printf("Utilizadores que sairam: %i \n", nClientesSairamSB);
		printf("Utilizadores que desistiram: %i \n", desistenciasSB);
		printf("\n");
		printf("Sala de Jogos\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramSJ);
		printf("Utilizadores que sairam: %i \n", nClientesSairamSJ);
		printf("Utilizadores que desistiram: %i \n", desistenciasSJ);
		printf("\n");
		printf("Zona de Sofas\n");
		printf("Utilizadores que entraram: %i \n", nClientesEntraramZS);
		printf("Utilizadores que sairam: %i \n", nClientesSairamZS);
		printf("Utilizadores que desistiram: %i \n", desistenciasZS);
		pthread_mutex_unlock(&protStats);

		//Fecha fciheiro após escrita dos logs.
		fechaFicheiro(logPointer);
		printf("Prima [ENTER] para terminar... \n");
		getchar();
		while(getchar() != '\n');

		sem_destroy(&aguardaSimulStart);
		pthread_mutex_destroy(&protStats);

		return 0;
	}
	else
	{
		printf("O nome do ficheiro introduzido não é o esperado. \n");
		return 1;
	}


}
