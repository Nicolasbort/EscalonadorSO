#include "escalonador.hpp"

Escalonador::Escalonador(int tempo_clock_ms)
{
	// default
	this->escalonadorAtual = NONE_ESCALONADOR;

	// tempo de clock do escalonador
	this->tempo_clock_ms = tempo_clock_ms;

	// Esta executando ou nao
	this->executing = false;

	// Esta pausado ou nao
	this->paused = false;

	// Seed para randomizar numeros
	std::srand(std::time(NULL));
}


Escalonador& Escalonador::setPRIORIDADE()
{
	this->escalonadorAtual = PRIORIDADE;
	return *this;
}


Escalonador& Escalonador::setRR()
{
	this->escalonadorAtual = RR;
	return *this;
}


Escalonador& Escalonador::setLOTERIA()
{
	this->escalonadorAtual = LOTERIA;
	return *this;
}


// Seta o algoritmo a ser usado atraves de uma string
Escalonador& Escalonador::setAlgoritmo(std::string algoritmoName)
{
	if (algoritmoName.compare(PRIORIDADE_NAME) == 0){
		this->escalonadorAtual = PRIORIDADE;
		setConsoleProperties("PRIORIDADE", {"PID","Nome","Clocks","Prioridade","UID","Mem.","Progresso"}, BG_RED);
	}
	else if (algoritmoName.compare(RR_NAME) == 0){
		this->escalonadorAtual = RR;
		setConsoleProperties("ROUND-ROBIN", {"PID","Nome","Clocks","Ordem","UID","Mem.","Progresso"}, BG_MAGENTA);
	}
	else if (algoritmoName.compare(LOTERIA_NAME) == 0){
		this->escalonadorAtual = LOTERIA;
		setConsoleProperties("LOTERIA", {"PID","Nome","Clocks","Bilhete","UID","Mem.","Progresso"}, BG_CYAN);
	}
	else
		std::printf("Algoritmo '%s' de escalonamento nao encontrado!\n", algoritmoName.c_str());

	return *this;
}


// Seta a fracao de CPU dos processos do escalonador
Escalonador& Escalonador::setFracaoCPU(int clocks)
{
	this->fracaoCPU = clocks;
	return *this;
}



// Adiciona o processo na lista de processos do escalonador
void Escalonador::addProcesso(Processo processo)
{
	this->processos.push_back(processo);
	this->newProcAdded = true;

	// Atualiza o console
	csl.updateNumProcs(processos.size());
}



// Inicia o escalonamento
void Escalonador::start()
{
	if (this->processos.empty()){
		printf("Nenhum processo adicionado. Parando o escalonador...\n");
		return;
	}

	if (fracaoCPU == 0){
		printf("Fracao de CPU nao encontrada...\n");
		return;
	}

	if (escalonadorAtual == NONE_ESCALONADOR){
		printf("Nenhum algoritmo de escalonamento setado...\n");
		return;
	}


	this->executing = true;
	return;

	// OLD
	// switch (this->escalonadorAtual)
	// {
	// 	case PRIORIDADE:
	// 		this->algoritmoPRIORIDADE();
	// 		break;

	// 	case RR:
	// 		this->algoritmoRR();
	// 		break;
		
	// 	case LOTERIA:
	// 		this->algoritmoLOTERIA();
	// 		break;

	// 	default:
	// 		printf("Algoritmo escalonador desconhecido...\n");
	// 		break;
	// }
}


void Escalonador::setConsoleProperties(std::string title, std::vector<std::string> header, int color)
{
	csl.set(header, title, color, processos.size(), fracaoCPU);
}


void Escalonador::printProcessos(int index)
{
	csl.clear();
	std::cout << csl.header();
	std::cout << csl.processo(processos[index]);

	for(Processo p : this->processos){
		if(processos[index].getPID() != p.getPID())
			std::cout << csl.processo(p);
	}

	printf("\nPressione %s para abrir o menu\n", csl.colorize(" ENTER ", BG_WHITE, BLACK).c_str());

	printf("\n%s < | Voltar ",csl.colorize(" 0 ", BG_WHITE, BLACK).c_str());
	printf("\n%s ⟲ | Reiniciar escalonador",csl.colorize(" 1 ",BG_WHITE, BLACK).c_str());
	printf("\n%s + | Adicionar processo \n",csl.colorize(" 2 ", BG_WHITE, BLACK).c_str());

	// system("clear");
	// std::cout << console_header();
	// std::cout << console_processo(processos[index]);
	
	// for(Processo p : this->processos){
	// 	if(this->processos[index].getPID() != p.getPID())
	// 		std::cout << console_processo(p);
	// }

	// printf("\nPressione %s para pausar a execução\n", console_colorize(" ENTER ", BG_WHITE, BLACK).c_str());

}


void Escalonador::updateConsole(int clock){
	csl.updateClock(clock);
}

void Escalonador::updateConsole(){
	csl.updateClock();
}


// Reseta o escalonador ao seu valor inicial
void Escalonador::reset()
{
	this->processos.clear();
	this->escalonadorAtual = PRIORIDADE;
}


void Escalonador::waitPause()
{
	while(paused){ 
		std::this_thread::sleep_for(std::chrono::milliseconds(tempo_clock_ms));
	}
}

void Escalonador::pause() { this->paused = true; }
void Escalonador::unpause() { this->paused = false; }
bool Escalonador::isPaused() { return this->paused; }
bool Escalonador::executando() { return this->executing; }
bool Escalonador::isPrioridades() { return (escalonadorAtual == PRIORIDADE); }
bool Escalonador::isRR() { return (escalonadorAtual == RR); }
bool Escalonador::isLoteria() { return (escalonadorAtual == LOTERIA); }

void Escalonador::resortPrioridades()
{
	std::sort(processos.begin(), processos.end(), &prioritySorter);
}


// Retorna o processo em execucao
Processo& Escalonador::getProcessoExecutando()
{
	return *p_ProcessoExecutando;
}


// Reseta os processos do escalonador ao seu valor inicial
void Escalonador::resetProcessos()
{
	for(int i=0; i<this->processos.size(); i++){
		this->processos[i].reset();
	}
}


int Escalonador::getNextProcIndex()
{
	switch (this->escalonadorAtual)
	{
		case PRIORIDADE:

			// Se é o primeiro processo
			if (p_ProcessoExecutando == nullptr){
				currentProcIndex = 0;
				break;
			}

			// Se foi adicionado outro processo
			if (newProcAdded){
				resortPrioridades();
				currentProcIndex = 0;
				newProcAdded = false;
				break;
			}

			if (p_ProcessoExecutando->isFinished()){
				currentProcIndex++;
			
				// Se esta no final da lista de processos reseta
				if (currentProcIndex == processos.size()){
					this->executing = false;
					currentProcIndex = 0;
				}
			}
			break;

		case RR:

			// Se é o primeiro processo
			if (p_ProcessoExecutando == nullptr){
				currentProcIndex = 0;
				break;
			}

			// Se deu o tempo de CPU ou terminou os clock totais
			if (p_ProcessoExecutando->clocksExecutados % fracaoCPU == 0 || p_ProcessoExecutando->clocksTerminados())
			{
				// Terminou todos processos
				if (contProcFinalizados == processos.size())
					this->executing = false;

				currentProcIndex++;

				// Se esta no final da lista de processos reseta
				if (currentProcIndex == processos.size())
					currentProcIndex = 0;
			}
			break;
		
		case LOTERIA:

			// Se é o primeiro processo
			if (p_ProcessoExecutando == nullptr){
				currentProcIndex = randomizeIndexByBilhete();
				break;
			}
			

			// Se deu o tempo de CPU do processo
			if (p_ProcessoExecutando->clocksExecutados % fracaoCPU == 0 || p_ProcessoExecutando->clocksTerminados())
			{
				currentProcIndex = randomizeIndexByBilhete();

				// Verifica se terminou os processos
				if (currentProcIndex == -1){
					this->executing = false;
					currentProcIndex = 0;
				}
			}

			break;
	}	

	return currentProcIndex;
}


// Realiza uma randomizacao baseada no peso do bilhete
int Escalonador::randomizeIndexByBilhete()
{
	int sumBilhetes = 0;

	for (int i=0; i<this->processos.size(); i++){
		if (processos[i].isReady())
			sumBilhetes += this->processos[i].prioridade;
	}
	
	int rndWeight = 0;
	
	// Verifica para nao dar divisao por 0
	if (sumBilhetes != 0)
		rndWeight = rand() % sumBilhetes;


	for (int i=0; i<this->processos.size(); i++)
	{
		if (processos[i].isReady()){
			if (rndWeight < this->processos[i].prioridade)
				return i;
			rndWeight -= this->processos[i].prioridade;
		}
	}

	return -1;
}





// OLD
void Escalonador::algoritmoPRIORIDADE()
{
  
	this->resetProcessos();

	for (int i=0; i<this->processos.size(); i++)
	{
		p_ProcessoExecutando = &processos[i];
		p_ProcessoExecutando->changeStatus(PROC_EXECUTANDO);

		for (int clocks=0; clocks<p_ProcessoExecutando->getClockTotal(); clocks++)
		{
			this->waitPause();

			p_ProcessoExecutando->clocksExecutados += 1;
			this->printProcessos(i);

			std::this_thread::sleep_for (std::chrono::milliseconds(this->tempo_clock_ms));	
		}

		p_ProcessoExecutando->changeStatus(PROC_FINALIZADO);
	}
}


// OLD
void Escalonador::algoritmoRR()
{
	printf("\nChamando RR...\n");

	this->resetProcessos();

	for (int i=0; i<processos.size(); i++){
		processos[i].prioridade = i+1;
	}

	unsigned PROC_INDEX = 0; 
	int COUNT_FINALIZADOS = 0;

	while (true)
	{
		p_ProcessoExecutando = &processos[PROC_INDEX];

		for (int clock=0; clock<this->fracaoCPU; clock++)
		{
			this->waitPause();

			p_ProcessoExecutando->changeStatus(PROC_EXECUTANDO);
			p_ProcessoExecutando->clocksExecutados++;


			// Se executou todos clocks seta o processo como finalizado
			if (p_ProcessoExecutando->clocksExecutados == p_ProcessoExecutando->totalClocks)
			p_ProcessoExecutando->changeStatus(PROC_FINALIZADO);

			this->printProcessos(PROC_INDEX);
			
			// Se executou todos clocks da break
			if (p_ProcessoExecutando->isFinished()){
				COUNT_FINALIZADOS++;
				break;
			}


			p_ProcessoExecutando->changeStatus(PROC_READY);
			
			std::this_thread::sleep_for (std::chrono::milliseconds(this->tempo_clock_ms));
		}

		if (COUNT_FINALIZADOS == processos.size())
			break;
	
		PROC_INDEX++;

		while (p_ProcessoExecutando->isFinished())
		{
			PROC_INDEX++;
			if (PROC_INDEX >= processos.size())
				PROC_INDEX = 0;
		}
	}
}


// OLD
void Escalonador::algoritmoLOTERIA()
{
	printf("\nChamando LOTERIA...\n");

	this->resetProcessos();


	while (true)
	{
		// Recebe o index de acordo com o "peso" do bilhete
		int PROC_INDEX = this->randomizeIndexByBilhete();

		// Verifica se existe processos ainda
		if (PROC_INDEX == -1)
			break;

		p_ProcessoExecutando = &processos[PROC_INDEX];

		for (int clock=0; clock<this->fracaoCPU; clock++)
		{
			this->waitPause();
			
			p_ProcessoExecutando->changeStatus(PROC_EXECUTANDO);
			p_ProcessoExecutando->clocksExecutados++;


			// Se executou todos clocks seta o processo como finalizado
			if (p_ProcessoExecutando->clocksExecutados == p_ProcessoExecutando->totalClocks)
				p_ProcessoExecutando->changeStatus(PROC_FINALIZADO);

			this->printProcessos(PROC_INDEX);
			
			// Se executou todos clocks da break
			if (p_ProcessoExecutando->isFinished())
				break;


			p_ProcessoExecutando->changeStatus(PROC_READY);
			
			std::this_thread::sleep_for (std::chrono::milliseconds(this->tempo_clock_ms));
		}
	}
}

void InfoEscalonador::set(std::string nomeAlgoritmo, int execTime){
	this->nomeAlgoritmo = nomeAlgoritmo;
	this->execTime = execTime;
}

// Funcao usada no std::sort do algoritmo de bilhete
bool prioritySorter(Processo const& leftProc, Processo const& rightProc)
{
	return leftProc.prioridade > rightProc.prioridade;
}

