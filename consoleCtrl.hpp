#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <iostream>
#include <algorithm>
#include "processo.hpp"
#include "defines.hpp"


void console_clear();
int console_menu(std::vector<std::string> options);
std::string console_colorize(std::string text, int color);
std::string console_colorize(std::string text, int color, int color2);
std::string console_bar(int val, int max, int size, int color);
std::string console_header(std::string title, int color);
std::string console_header(std::string title, std::vector<std::string> cabecalho, int color);
std::string console_cell(bool header, int color, std::string text, int base_space);

std::string console_processo(Processo &processo);


class Console
{
public:

	std::string colorize(std::string text, int color);
	std::string colorize(std::string text, int color, int color2);

	std::string bar(int val, int max, int size, int color);
	std::string header();
	std::string cell(bool header, int color, std::string text, int base_space);
	std::string processo(Processo &processo);

	void set(std::vector<std::string> header, std::string title, int color_code, int numProcs, int clockMaxCPU);
	void clear();
	void updateClock();
	void updateClock(int clock);
	void updateNumProcs(int number);


private:
	std::string title;
	std::vector<std::string> head;
	int colorCode;

	int numProcs;
	int clockMaxCPU;

	int clockAtual = 0;
};