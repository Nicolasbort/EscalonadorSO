#include "functions.hpp"


void readFile(const char* file_name)
{
	std::ifstream file(file_name);
	std::string line;

	while (std::getline(file, line))
	{
		std::cout << line << "\n\n";
	}
}



std::vector<std::string> split(char delimiter, std::string str_to_split)
{
	std::replace(str_to_split.begin(), str_to_split.end(), delimiter, ' ');

	std::vector<std::string> strings_array;

	std::stringstream ss(str_to_split);
	std::string temp;
	while (ss >> temp)
		strings_array.push_back(temp);

	return strings_array;
}



void getProcessosFromFile(const char* file_name, std::vector<Processo> &processosOutput, InfoEscalonador &escalonadorOutput)
{
	std::ifstream file(file_name);
	std::string line;
	std::vector<std::string> line_splited;

	while (std::getline(file, line))
	{
		// Recebe um vetor de strings com cada parte do split
		line_splited = split('|', line);

		if (line_splited.size() == 6)
		{

			Processo temp_proc(
						line_splited[PROC_NAME_INDEX],
						std::stoi(line_splited[PROC_PID_INDEX]),
						std::stoi(line_splited[PROC_EXECTIME_INDEX]),
						std::stoi(line_splited[PROC_PRIOR_INDEX]),
						std::stoi(line_splited[PROC_UID_INDEX]),
						std::stoi(line_splited[PROC_MEMORY_INDEX]),
						false
					);


			// Adiciona um novo processo no vetor de processos
			processosOutput.push_back(temp_proc);
		}
		// Recebe o algoritmo e o tempo de CPU
		else if (line_splited.size() == 2)
		{
			escalonadorOutput.set(
				line_splited[ESCALONAMENTO_TYPE_INDEX],
				std::stoi(line_splited[ESCALONAMENTO_TIME_INDEX])
				);
		}
		else{
			printf("Linha invalida!\n");
		}
	}	

	file.close();
}


//Recebe uma string e retorna a string colorida
std::string colorizeStr(std::string text, int color)
{
  return "\x1B["+std::to_string(color)+"m"+text+"\033[0m";
}



// Thread para pegar o input
void t_getInput()
{
	std::cin.get();


	printf("Clicado!\n");
}