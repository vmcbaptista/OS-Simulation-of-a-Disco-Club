#include "header.h"


//Retorna o tempo atual desde a era UNIX
time_t tempoAtual(){
	return time(NULL);
}
//Realiza a soma entre o tempo atual e o número de segundo pretendido
time_t calculaTempo(float segundos){
	return tempoAtual() + (time_t) segundos;
}
// Retorna a diferença entre o tempo Atual e o tempo esperado para um acontecimento
time_t tempoFalta(time_t tempoEsperado){
	return  tempoEsperado - tempoAtual();
}
//Retorna verdadeiro(1) se terminou o tempo esperado para um acontecimento
bool verificaPassagemTempo(time_t tempoEsperado){
	if(tempoFalta(tempoEsperado) == (time_t) 0){
		return true;}
	else{
		return false;}
}
// Converte um dado numero de minutos em segundos
float converteMinutos(float minutos){
	return minutos * 60;
}

// Converte um dado numero de segundos reais em tempo de
float converteSegundosReais(float segundosReais){
	return segundosReais*TEMPO_SIMULACAO/converteMinutos(TEMPO_SIMULADO);
}
/*
void main(){
	time_t tempoPretendido = calculaTempo(TEMPO_REAL_SIM);
	//while(!verificaPassagemTempo(tempoPretendido)){
		//for(int i = 0; i < 10000; i++);
		printf("%s %lu %s", "Faltam",tempoFalta(calculaTempo(converteSegundosReais(converteMinutos(800), converteMinutos(TEMPO_SIMULADO), TEMPO_REAL_SIM))),"segundos");
	//}
}
*/

