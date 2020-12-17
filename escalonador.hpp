#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono> 
#include <random>
#include <time.h>
#include <stdio.h>
#include "processo.hpp"
#include "consoleCtrl.hpp"
#include "defines.hpp"



class Escalonador
{
public:
	std::vector<Processo> processos;

	Escalonador(int tempo_clock_ms);

	Escalonador& setPRIORIDADE();
	Escalonador& setRR();
	Escalonador& setLOTERIA();

	Escalonador& setAlgoritmo(std::string algoritmoName);
	Escalonador& setFracaoCPU(int clocks);

	void addProcesso(Processo processo);
	void start();
	void setConsoleProperties(std::string title, std::vector<std::string> header, int color);
	void printProcessos(int index);
	void reset();
	void pause();
	void unpause();
	void waitPause();
  	void resetProcessos();
	void resortPrioridades();
	void updateConsole(int clock);
	void updateConsole();

	int randomizeIndexByBilhete();
	int getNextProcIndex();

	bool executando();
	bool isPaused();

	bool isPrioridades();
	bool isRR();
	bool isLoteria();

	Processo& getProcessoExecutando();
	Processo* p_ProcessoExecutando = nullptr;
	int contProcFinalizados = 0;

	bool newProcAdded = false;

private:

	int escalonadorAtual;
	int fracaoCPU;
	int tempo_clock_ms;
	int currentProcIndex = 0;

	bool executing;
	bool paused;

	Processo processoExecutando;
	Console csl;

	// Algoritmos de escalonamento que vao ser chamados na funcao start
	void algoritmoPRIORIDADE();
	void algoritmoRR();
	void algoritmoLOTERIA();
};



class InfoEscalonador
{
public:

	std::string nomeAlgoritmo;
	int execTime;

	void set(std::string nomeAlgoritmo, int execTime);
};


bool prioritySorter(Processo const& leftProc, Processo const& rightProc);