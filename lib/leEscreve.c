/*
 * leEscreve.c
 *
 *  Created on: Oct 28, 2015
 *      Author: fabio
 */
#include "header.h"

int valores[5]={};
/*Carrega os parametros iniciais do simulador*/
void carregaSimulador(char* simulador){
	int i  = 0;
	int j = 0;

	int MAXCHARSPORLINHA = 500; //Linhas devem ter no maximo 500 carateres
	char linha[MAXCHARSPORLINHA];
	char numeros[20];			//Numeros podem ter ate 20 carateres, no entanto se tiverem tantos ocorrem riscos de overflow

	int armazenaParametro[23];

	FILE* abreFile = fopen(simulador, "r");
	if(abreFile==NULL)
		printf("Erro não foi possível abrir o ficheiro %s.", simulador);
	else
	{
		while ( fgets(linha, sizeof(linha), abreFile) )
		{
			//Ignora linhas cuja primeria letra seja # por ler a linha de 1x
			if (*linha == '#')
			{    continue;   }

			//Pega na linha e le os valores numeros como string e guarda-os em numero
			if ( sscanf(linha, "%s", numeros) == 1 )
			{
				armazenaParametro[i] = atoi(numeros); //converte os para int
				//printf("%i \n ", armazenaParametro[i]);
				i++;
			}
		}
	}
	//Depois de carregado no vetor vai carregar na estrutura.

	TEMPO_SIMULACAO =armazenaParametro[j];
	j++;
	TEMPO_SIMULADO = armazenaParametro[j];
	j++;
	esperaTempo.tempoEntrada = armazenaParametro[j];
	j++;
	esperaTempo.tempoVip = armazenaParametro[j];
	j++;
	esperaTempo.tempoNorm = armazenaParametro[j];
	j++;
	esperaTempo.tempoPool =armazenaParametro[j];
	j++;
	esperaTempo.tempoBar =armazenaParametro[j];
	j++;
	esperaTempo.tempoJogos = armazenaParametro[j];
	j++;
	esperaTempo.tempoSnackBar = armazenaParametro[j];
	j++;
	lotMax.lotTotal = armazenaParametro[j];
	j++;
	lotMax.lotDS = armazenaParametro[j];
	j++;
	lotMax.lotNorm = armazenaParametro[j];
	j++;
	lotMax.lotPool = armazenaParametro[j];
	j++;
	lotMax.lotSofas = armazenaParametro[j];
	j++;
	lotMax.lotJogos = armazenaParametro[j];
	j++;
	limitePessoasFila.limiteDD = armazenaParametro[j];
	j++;
	limitePessoasFila.limiteNorm = armazenaParametro[j];
	j++;
	limitePessoasFila.limitePool = armazenaParametro[j];
	j++;
	limitePessoasFila.limiteJogos = armazenaParametro[j];
	j++;
	limitePessoasFila.limiteSnackBar = armazenaParametro[j];
	j++;
	condicaoAbertura = armazenaParametro[j];
	j++;
	funcionamento.aberturaSB = armazenaParametro[j];
	j++;
	funcionamento.encerramentoJogos = armazenaParametro[j];



	fclose(abreFile);
}

/*Carrega os parametros iniciais do monitor
 * Carrega as string que vai apresentar no ecrã
 *
 * */
struct mensagemMonitor* carregaMonitor(char* monitor){
	int i  = 0;
	//int mon;

	size_t len = 0;
	ssize_t read;
	FILE* abreFile = fopen(monitor, "r");

	if(abreFile==NULL)
		printf("Erro não foi possível abrir o ficheiro %s.",monitor);
	else
	{
		struct mensagemMonitor *msgMonitor = malloc(sizeof(struct mensagemMonitor));
		len = getline(&(msgMonitor->indicaEstado), &len, abreFile);
		if((msgMonitor->indicaEstado)[len - 1] == '\n') {
			((msgMonitor->indicaEstado))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->simTerminada), &len, abreFile);
		if((msgMonitor->simTerminada)[len - 1] == '\n') {
			((msgMonitor->simTerminada))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->simDecorre), &len, abreFile);
		if((msgMonitor->simDecorre)[len - 1] == '\n') {
			((msgMonitor->simDecorre))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->nUtilizadores), &len, abreFile);
		if((msgMonitor->nUtilizadores)[len - 1] == '\n') {
			((msgMonitor->nUtilizadores))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->nDesistencias), &len, abreFile);
		if((msgMonitor->nDesistencias)[len - 1] == '\n') {
			((msgMonitor->nDesistencias))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->tempoMedioEspera), &len, abreFile);
		if((msgMonitor->tempoMedioEspera)[len - 1] == '\n') {
			((msgMonitor->tempoMedioEspera))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->oCliente), &len, abreFile);
		if((msgMonitor->oCliente)[len - 1] == '\n') {
			((msgMonitor->oCliente))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->entrou), &len, abreFile);
		if((msgMonitor->entrou)[len - 1] == '\n') {
			((msgMonitor->entrou))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->chegou), &len, abreFile);
		if((msgMonitor->chegou)[len - 1] == '\n') {
			((msgMonitor->chegou))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->saiu), &len, abreFile);
		if((msgMonitor->saiu)[len - 1] == '\n') {
			((msgMonitor->saiu))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->espera), &len, abreFile);
		if((msgMonitor->espera)[len - 1] == '\n') {
			((msgMonitor->espera))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->esperaParceiro), &len, abreFile);
		if((msgMonitor->esperaParceiro)[len - 1] == '\n') {
			((msgMonitor->esperaParceiro))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->produziu), &len, abreFile);
		if((msgMonitor->produziu)[len - 1] == '\n') {
			((msgMonitor->produziu))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->desistiu), &len, abreFile);
		if((msgMonitor->desistiu)[len - 1] == '\n') {
			((msgMonitor->desistiu))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->disc), &len, abreFile);
		if((msgMonitor->disc)[len - 1] == '\n') {
			((msgMonitor->disc))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->pistaSalao), &len, abreFile);
		if((msgMonitor->pistaSalao)[len - 1] == '\n') {
			((msgMonitor->pistaSalao))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->pistaTodos), &len, abreFile);
		if((msgMonitor->pistaTodos)[len - 1] == '\n') {
			((msgMonitor->pistaTodos))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->pistaPP), &len, abreFile);
		if((msgMonitor->pistaPP)[len - 1] == '\n') {
			((msgMonitor->pistaPP))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->zns), &len, abreFile);
		if((msgMonitor->zns)[len - 1] == '\n') {
			((msgMonitor->zns))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->sb), &len, abreFile);
		if((msgMonitor->sb)[len - 1] == '\n') {
			((msgMonitor->sb))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->sj), &len, abreFile);
		if((msgMonitor->sj)[len - 1] == '\n') {
			((msgMonitor->sj))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->bar), &len, abreFile);
		if((msgMonitor->bar)[len - 1] == '\n') {
			((msgMonitor->bar))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->normal), &len, abreFile);
		if((msgMonitor->normal)[len - 1] == '\n') {
			((msgMonitor->normal))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->vip), &len, abreFile);
		if((msgMonitor->vip)[len - 1] == '\n') {
			((msgMonitor->vip))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->empregadoBar), &len, abreFile);
		if((msgMonitor->empregadoBar)[len - 1] == '\n') {
			((msgMonitor->empregadoBar))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->empregadoSnack), &len, abreFile);
		if((msgMonitor->empregadoSnack)[len - 1] == '\n') {
			((msgMonitor->empregadoSnack))[len - 1] = '\0';
		}
		len = getline(&(msgMonitor->tempoSimulacao), &len, abreFile);
		if((msgMonitor->tempoSimulacao)[len - 1] == '\n') {
			((msgMonitor->tempoSimulacao))[len - 1] = '\0';
		}
		TEMPO_SIMULACAO = atoi(msgMonitor->tempoSimulacao);
		fclose(abreFile);
		return msgMonitor;

	}
}


/*abre ficheiro na abertura da discoteca recebe o nome do ficheiro onde se vai escrever*/
FILE * abreFicheiro(char* logs)
{
	FILE* log = fopen(logs, "a");
	return log;
}
/*fecha o ficheiro após encerramento da discoteca*/
void fechaFicheiro(FILE* log)
{
	fclose(log);
}

/*Recebe o nome do ficheiro onde se quer escrever os logs.*/
void escreveLog(FILE* log, char* mensagem ){

	fprintf(log, "%s \n", mensagem);
}

/*FUnção que irá escrever no ecrã durante a execução do monitor*/
void escreveEcra(char* mensagem)
{
	printf("%s ",mensagem);
}

