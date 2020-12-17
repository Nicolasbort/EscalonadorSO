#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "processo.hpp"
#include "escalonador.hpp"


// algoritmoDeEscalonamento|fraçãoDeCPU
// Indexes do vetor de string do split '|'
#define ESCALONAMENTO_TYPE_INDEX 0
#define ESCALONAMENTO_TIME_INDEX 1

// nomeProcesso|PID|tempoDeExecução|Prior./Bilh.|UID|qtdeMemoria
// Indexes do vetor de string do split '|'
#define PROC_NAME_INDEX 0
#define PROC_PID_INDEX 1
#define PROC_EXECTIME_INDEX 2
#define PROC_PRIOR_INDEX 3
#define PROC_UID_INDEX 4
#define PROC_MEMORY_INDEX 5



template<typename T>
void printfVector(std::vector<T> vec)
{
	for (int i=0; i<vec.size(); i++)
		std::cout << vec[i] << " ";
	std::cout << "\n";
}

void readFile(const char* file_name);
std::vector<std::string> split(char delimiter, std::string str_to_split);
void getProcessosFromFile(const char* file_name, std::vector<Processo> &processosOutput, InfoEscalonador &escalonadorOutput);
std::string colorizeStr(std::string text, int color);