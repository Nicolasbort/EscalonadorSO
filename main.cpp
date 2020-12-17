#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>   
#include "escalonador.hpp"
#include "processo.hpp"
#include "functions.hpp"


using namespace std;

void threadPauseEscalonador();
void threadGetNewProc();

//g++ -pthread main.cpp escalonador.cpp processo.cpp consoleCtrl.cpp functions.cpp -o main

// Tempo da simulacao.
#define FPS_EXECUCAO 280
#define TEMPO_CLOCK_MS (int)((60.0 / FPS_EXECUCAO) * 1000)

Escalonador escalonador(TEMPO_CLOCK_MS);

int main()
{

	float TEMPO_GLOBAL_PERCORRIDO_MS = 0;

	printf("Tempo por clock : %dms\n", TEMPO_CLOCK_MS);


	// Vetor com todos processos encontrados no txt
	vector<Processo> processos;				
	// Vetor com todos algoritmos e tempo de execucao encontrados no txt
	InfoEscalonador infoEscalonador;

	// Le os processos e escalonadores do arquivo e armazena nos vetores
	getProcessosFromFile("processos.txt", processos, infoEscalonador);


	// Adiciona os processos no escalonador
	for (int i=0; i<processos.size(); i++)
		escalonador.addProcesso(processos[i]);


	thread t1(threadGetNewProc);


	escalonador.setFracaoCPU(infoEscalonador.execTime).setAlgoritmo(infoEscalonador.nomeAlgoritmo).start();

	// Variavel para controle do terminal
	int lastProcIndex = 0;

	// Enquanto o escalonador estiver ativo
	while (escalonador.executando())
	{
		// Espera se tiver pausado
		escalonador.waitPause();

		unsigned PROC_INDEX = escalonador.getNextProcIndex();
		escalonador.p_ProcessoExecutando = &escalonador.processos[PROC_INDEX];

		// Reseta o clock do console se alterar o processo finalizado
		if (lastProcIndex != PROC_INDEX){
			lastProcIndex = PROC_INDEX;
			escalonador.updateConsole(0);
		}

		// Se ja acabou vai pro proximo
		if (escalonador.p_ProcessoExecutando->isFinished())
			continue;
			

		escalonador.p_ProcessoExecutando->changeStatus(PROC_EXECUTANDO);
		escalonador.p_ProcessoExecutando->clocksExecutados++;
		// Adiciona +1 no clock do terminal
		escalonador.updateConsole();

		// 
		if (escalonador.p_ProcessoExecutando->clocksTerminados()){
			escalonador.contProcFinalizados++;
			escalonador.p_ProcessoExecutando->changeStatus(PROC_FINALIZADO);
			continue;
		}

		escalonador.printProcessos(PROC_INDEX);

		escalonador.p_ProcessoExecutando->changeStatus(PROC_READY);

		this_thread::sleep_for (chrono::milliseconds(TEMPO_CLOCK_MS));	
	}


	t1.join();
	
}




// Thread responsavel por pegar o input do processo de maneira assincrona
void threadGetNewProc()
{
	while (escalonador.executando())
	{
		string s_newProc;

		getline(cin, s_newProc);

		escalonador.pause();

		vector<string> newProc = split('|', s_newProc);

		if (newProc.size() != 6){
			printf("Formato de processo invalido!\n\n");
			printf("Inserir: %s\n", colorizeStr("nomeProcesso|PID|tempoDeExecução|Prior./Bilh.|UID|qtdeMemoria", CYAN).c_str());
			printf("Processo enviado: %s\n", colorizeStr(s_newProc, RED).c_str());
			this_thread::sleep_for(chrono::seconds(5));
			escalonador.unpause();
			continue;
		}

		printf("\nAdionando procesos no escalonador...\n");

		Processo tempProc(
			newProc[PROC_NAME_INDEX],
			std::stoi(newProc[PROC_PID_INDEX]),
			std::stoi(newProc[PROC_EXECTIME_INDEX]),
			std::stoi(newProc[PROC_PRIOR_INDEX]),
			std::stoi(newProc[PROC_UID_INDEX]),
			std::stoi(newProc[PROC_MEMORY_INDEX]),
			false
		);

		escalonador.addProcesso(tempProc);
		
		escalonador.unpause();
	}
}


// SEM USO AGORA
// Thread responsavel por pausar o código e pegar o processo do input
void threadPauseEscalonador()
{
	while (escalonador.executando())
	{
		cin.get();
		
		if (escalonador.isPaused()){
			printf("ENTER pressionado. Escalonador despausado.\n");
			escalonador.unpause();
		}else{
			printf("ENTER pressionado. Escalonador pausado.\n");

			escalonador.pause();

			int opcao;

			printf("Adicionar processo? (1) SIM   (0) NAO - CONTINUAR\n");
			cin >> opcao;

			if (opcao)
				// Randomizando processo
				escalonador.addProcesso(
					Processo(
						"ProcAdded" + to_string(rand() % 15), 
						rand() % 50, 
						60,
						rand() % 15,
						0,
						69,
						false
					)
				);
		}
	}
}




