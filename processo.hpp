#pragma once

#include <vector>
#include <string>
#include "defines.hpp"


class Processo
{
public:

	Processo(std::string nome, int PID, int clocks, int prioridade, int UID, int qtdMemoria, bool isPreemptivo);

	Processo();

	std::string getName();
	int getPID();
	int getPrioridade();
	int getUID();
	int getQtdMemoria();
	int getClockTotal();

	void reset();
	void changeStatus(int status);

	int getStatus();

	bool isPreemptivo();
	bool isReady();
	bool isFinished();
	bool isExecuting();
	bool isBlocked();

	bool clocksTerminados();


	std::string nome;
	int PID;
	int prioridade;
	int UID;
	int qtdMemoria;
	int totalClocks;
	int clocksExecutados;

private:

	int status;
	bool preemptivo;
};