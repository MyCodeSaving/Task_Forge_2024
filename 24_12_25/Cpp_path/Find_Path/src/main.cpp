#include <iostream>
#include <fstream>
#include <solve.h>
#include <map>
#include <vector>
#include <cstdio>
#include <sstream>

std::map<std::string, int> dictionary = {
	{"A",0},
	{"B",1},
	{"C",2},
	{"D",3},
	{"E",4},
	{"F",5},
	{"G",6},
	{"H",7},
	{"I",8},
	{"J",9}
};

int main() {
	/*
	地图初始化
	*/
	graph map = graph(NUM_VERTEX, NUM_EDGE);

	std::cout << "---------- Reading map data ----------" << std::endl;
	std::ifstream map_data;
	map_data.open ("resource/map_edge.csv", std::ios::in);
	if (!map_data.is_open()) {
		std::cout << "Error: opening file fail" << std::endl;
		exit(1);
	}
	else {
		std::string line;
		std::vector<std::string> words;
		std::string word;
		// 跳过表头
		getline(map_data, line);

		std::istringstream sin;	// 中间流
		int u[NUM_EDGE] = { 0 }, v[NUM_EDGE] = { 0 }, w[NUM_EDGE] = {0}, i = 0;
		while (getline(map_data, line)) {
			words.clear();
			sin.clear();
			sin.str(line);
			while (getline(sin, word, ',')) {	// 注意单引号表示char
				// std::cout << word << std::endl;
				words.push_back(word);
			}
			// 处理读取的words
			u[i] = dictionary[words[0]];
			v[i] = dictionary[words[1]];
			w[i] = std::stoi(words[2]);
			std::cout << words[0] << " " << words[1] << " " << words[2] << std::endl;
			i++;
		}
		map.init(u, v, w);
		map_data.close();
	}

	/*
	包裹初始化
	*/
	solve::package pacs[NUM_PACKAGES];

	std::cout << "---------- Reading package data ----------" << std::endl;
	std::ifstream pac_data;
	pac_data.open("resource/test_package.csv", std::ios::in);
	if (!pac_data.is_open()) {
		std::cout << "Error: opening file fail" << std::endl;
		exit(1);
	}
	else {
		std::string line;
		std::vector<std::string> words;
		std::string word;
		// 跳过表头
		getline(pac_data, line);

		std::istringstream sin;	// 中间流
		int i = 0;
		while (getline(pac_data, line)) {
			words.clear();
			sin.clear();
			sin.str(line);
			while (getline(sin, word, ',')) {	// 注意单引号表示char
				// std::cout << word << std::endl;
				words.push_back(word);
			}
			// 处理读取的words
			pacs[i].id = std::stoi(words[0]);
			pacs[i].weight = std::stoi(words[1]);
			pacs[i].time_limit = std::stoi(words[2]);
			pacs[i].destination = dictionary[words[3]];
			pacs[i].prior = std::stoi(words[4]);
			pacs[i].done = false;
			std::cout << words[0] << " " << words[1] << " " << words[2] << " " << words[3] << " " << words[4] << std::endl;
			i++;
		}
		pac_data.close();

		while (i < NUM_PACKAGES) {
			pacs[i].id = -1;
			i++;
		}
	}

	/*
	货车初始化
	*/
	solve::car cars[NUM_CARS];

	std::cout << "---------- Reading car data ----------" << std::endl;
	std::ifstream car_data;
	car_data.open("resource/test_car.csv", std::ios::in);
	if (!car_data.is_open()) {
		std::cout << "Error: opening file fail" << std::endl;
		exit(1);
	}
	else {
		std::string line;
		std::vector<std::string> words;
		std::string word;
		// 跳过表头
		getline(car_data, line);

		std::istringstream sin;	// 中间流
		int i = 0;
		while (getline(car_data, line)) {
			words.clear();
			sin.clear();
			sin.str(line);
			while (getline(sin, word, ',')) {	// 注意单引号表示char
				// std::cout << word << std::endl;
				words.push_back(word);
			}
			// 处理读取的words
			cars[i].id = i;
			cars[i].v = std::stoi(words[0]);
			cars[i].weight_car = std::stoi(words[1]);
			cars[i].weight_limit = std::stoi(words[2]);
			std::cout << words[0] << " " << words[1] << " " << words[2] << std::endl;
			i++;
		}
		car_data.close();

		while (i < NUM_CARS) {
		 	pacs[i].id = -1;
		 	i++;
		}
	}

	/*
	solve初始化
	*/
	solve solution1 = solve(map, cars, pacs);
	solve solution2 = solve(map, cars, pacs);
	solve solution3 = solve(map, cars, pacs);
	solve solution4 = solve(map, cars, pacs);

	// 策略一，无中断
	solution1.strategy_1();
	// 策略一，有中断
	//solution2.strategy_1(true);

	// 策略二，无中断
	solution3.strategy_2();
	// 策略二，有中断
	//solution4.strategy_2(true);
}