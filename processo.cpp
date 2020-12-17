#include "processo.hpp"

Processo::Processo(std::string nome, int PID, int clocks, int prioridade, int UID, int qtdMemoria, bool isPreemptivo)
{
	this->nome = nome;
	this->PID = PID;
	this->totalClocks = clocks;
	this->prioridade = prioridade;
	this->UID = UID;
	this->qtdMemoria = qtdMemoria;
	this->preemptivo = isPreemptivo;
	this->clocksExecutados = 0;
	this->status = PROC_READY;
}

Processo::Processo() {}


std::string Processo::getName() { return this->nome; }
int Processo::getPID() { return this->PID; }
int Processo::getPrioridade() { return this->prioridade; }
int Processo::getUID() { return this->UID; }
int Processo::getQtdMemoria() { return this->qtdMemoria; }
int Processo::getClockTotal() { return this->totalClocks; }
int Processo::getStatus() { return this->status; }
void Processo::changeStatus(int status) { this->status = status; }


bool Processo::isPreemptivo() { return this->preemptivo; }
bool Processo::isReady() { return ( status == PROC_READY); }
bool Processo::isFinished() { return ( status == PROC_FINALIZADO); }
bool Processo::isExecuting() { return ( status == PROC_EXECUTANDO); }
bool Processo::isBlocked() { return ( status == PROC_BLOCKED); }


bool Processo::clocksTerminados()
{
	return (this->clocksExecutados == this->totalClocks);
}

void Processo::reset()
{
	this->clocksExecutados = 0;
	this->status = PROC_READY;
}