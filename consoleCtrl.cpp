#include "consoleCtrl.hpp"
#include "processo.hpp"

//Limpa o console
void console_clear(){
  system("clear");
}

//Retorna o index selecionado pelo usuário
int console_menu(std::vector<std::string> options){
  for(int i = 0; i < options.size(); i++){
    std::cout << console_colorize(' '+std::to_string(i)+' ', BG_WHITE, BLACK) << " - " << options[i] << std::endl;
  }
  std::string input;
  std::getline(std::cin, input);
  return std::stoi(input.c_str());
}

//Recebe uma string e retorna a string colorida
std::string console_colorize(std::string text, int color){
  return "\x1B["+std::to_string(color)+"m"+text+"\033[0m";
}

std::string console_colorize(std::string text, int color, int color2){
  return "\x1B["+std::to_string(color)+"m\x1B["+std::to_string(color2)+"m"+text+"\033[0m";
}

//Retorna uma barra de progresso em string
std::string console_bar(int val, int max, int size, int color){
  int full_char = val*size/max;
  int empty_char = size - full_char;
  std::string ret = "";
  for(int i = 0; i < full_char; i ++){
    ret += "■";
  }
  ret = console_colorize(ret, color);
  for(int i = 0; i < empty_char; i ++){
    ret += "□";
  }
  return ret;
}

//Retorna a linha do cabeçalho em string
std::string console_header(std::string title, int color){
  std::string ret = "";
  ret += console_colorize("█▓▒░░  "+title+"  ░░▒▓█", color) + "\n\n";
  ret += console_cell(true, color, "PID ", 1);
  ret += console_cell(true, color, "Nome ", 3);
  ret += console_cell(true, color, "Clocks ", 2);
  ret += console_cell(true, color, "Prioridade ", 3);
  ret += console_cell(true, color, "UID ", 2);
  ret += console_cell(true, color, "Mem. ", 2);
  ret += console_cell(true, color, "Progresso ", 1);
  return ret + '\n';
}

//Retorna a linha do cabeçalho em string
std::string console_header(std::string title, std::vector<std::string> cabecalho, int color){
	if (cabecalho.size() != 7)
		return "Cabecalho invalido. Mudar no ctrl linha 142.\n";

	std::string ret = "";
	ret += console_colorize("█▓▒░░  "+title+"  ░░▒▓█", color) + "\n";
  //TO-DO
  ret += "█ Tempo Exec.:\t15s\n";
  ret += "█ Nº Processos:\t10\n";
  ret += "█ Quantum:\t\t" +console_bar(0,20,6,GREEN) + "\n\n";
	ret += console_cell(true, color, cabecalho[0], 1);
	ret += console_cell(true, color, cabecalho[1], 3);
	ret += console_cell(true, color, cabecalho[2], 2);
	ret += console_cell(true, color, cabecalho[3], 3);
	ret += console_cell(true, color, cabecalho[4], 2);
	ret += console_cell(true, color, cabecalho[5], 2);
	ret += console_cell(true, color, cabecalho[6], 1);
	return ret + '\n';
}


//Retorna uma célula em string
std::string console_cell(bool header, int color, std::string text, int base_space){
  if(text.size() <= 2){
    text = "0"+text;
  }
  int space = base_space;
  if(text.size() <= base_space*4){
    space += text.size()/4;
  }else{
    space -= text.size()/4;
  }

  if(header)
    return console_colorize("█ " + text + std::string(space,'\t'), color);
  else
    return "│ " + text + std::string(space,'\t');
}


std::string console_processo(Processo &processo)
{
	std::string row_pid = console_cell(false, WHITE, std::to_string(processo.PID), 1);
	std::string row_nome = console_cell(false, WHITE, processo.nome, 3);
	std::string row_clocks = console_cell(false, WHITE, std::to_string(processo.totalClocks), 2);
	std::string row_prioridade = console_cell(false, WHITE, std::to_string(processo.prioridade), 4);
	std::string row_uid = console_cell(false, WHITE, std::to_string(processo.UID), 3);
	std::string row_memoria = console_cell(false, WHITE, std::to_string(processo.qtdMemoria), 2);
	std::string row_bar = console_bar(processo.clocksExecutados, processo.totalClocks-1, 10, (processo.isFinished()?BRIGHT_BLACK:(processo.isExecuting()?GREEN:WHITE)));

  	std::string ret = row_pid + row_nome + row_clocks + row_prioridade + row_uid + row_memoria + row_bar + '\n';

	if (processo.isFinished())
    	return console_colorize(ret, BRIGHT_BLACK);
	else if(processo.isExecuting())
    	return console_colorize(ret, BRIGHT_GREEN);
	else
		return console_colorize(ret, BRIGHT_WHITE);
}





// console class
std::string Console::colorize(std::string text, int color){
  return "\x1B["+std::to_string(color)+"m"+text+"\033[0m";
}

std::string Console::colorize(std::string text, int color, int color2){
  return "\x1B["+std::to_string(color)+"m\x1B["+std::to_string(color2)+"m"+text+"\033[0m";
}

std::string Console::bar(int val, int max, int size, int color){
  int filled = val*size/max;
  int empty = size - filled;
  std::string ret = "";
  for(int i = 0; i < filled; i++){
    ret += "▰";
  }
  ret = console_colorize(ret, GREEN);
  for(int i = 0; i < empty; i++){
    ret += "▱";
  }
  return " "+ret;
}


//espaços: 4, 15, 7, 10, 4, barra
std::string Console::header(){
	if (head.size() != 7)
		return "Cabecalho invalido. Mudar no ctrl linha 142.\n";

	std::string ret = "";
	ret += colorize("█  "+title+"  ", colorCode) + "\n";
	ret += "█ Nº Processos:\t "+ std::to_string(numProcs) +"\n";
	ret += "█ Fracao CPU:\t" +bar(clockAtual, clockMaxCPU-1, clockMaxCPU, GREEN) + "\n\n";
	ret += cell(true, colorCode, head[0], 6);
	ret += cell(true, colorCode, head[1], 15);
	ret += cell(true, colorCode, head[2], 8);
	ret += cell(true, colorCode, head[3], 12);
	ret += cell(true, colorCode, head[4], 6);
	ret += cell(true, colorCode, head[5], 6);
	ret += cell(true, colorCode, head[6], 15);
	return ret + '\n';
}

std::string Console::cell(bool header, int color, std::string text, int max_size){
  if(text.size() < max_size+(header?-1:0)){
    int temp = max_size-text.size();
    temp = (temp>0?temp:0);
    text += std::string(temp,' ');
  }
  return (header?console_colorize(" "+text,color,BLACK):console_colorize(" "+text, color));
}

//espaços: 4, 15, 7, 10, 4, barra
std::string Console::processo(Processo &processo){
	std::string row_pid = cell(false, WHITE, std::to_string(processo.PID), 6);
	std::string row_nome = cell(false, WHITE, processo.nome, 15);
	std::string row_clocks = cell(false, WHITE, std::to_string(processo.totalClocks), 8);
	std::string row_prioridade = cell(false, WHITE, std::to_string(processo.prioridade), 12);
	std::string row_uid = cell(false, WHITE, std::to_string(processo.UID), 6);
	std::string row_memoria = cell(false, WHITE, std::to_string(processo.qtdMemoria), 6);
	std::string row_bar = bar(processo.clocksExecutados, processo.totalClocks-1, 10, (processo.isFinished()?BRIGHT_BLACK:(processo.isExecuting()?GREEN:WHITE)));

  std::string ret = row_pid + row_nome + row_clocks + row_prioridade + row_uid + row_memoria + row_bar + '\n';

	if (processo.isFinished())
    	return colorize(ret, BRIGHT_BLACK);
	else if(processo.isExecuting())
    	return colorize(ret, BRIGHT_GREEN);
	else
		return colorize(ret, BRIGHT_WHITE);
}

void Console::set(std::vector<std::string> header, std::string title, int color_code, int numProcs, int clockMaxCPU){
	this->title = title;
	this->head = header;
	this->colorCode = color_code;

	this->numProcs = numProcs;
	this->clockMaxCPU = clockMaxCPU;
}

void Console::clear(){
	system("clear");
}

void Console::updateClock(){
	clockAtual++;

	if (clockAtual == clockMaxCPU)
		clockAtual = 0;
}


void Console::updateClock(int clock){
	clockAtual = clock;
}

void Console::updateNumProcs(int number){
	this->numProcs = number;
}