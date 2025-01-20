#pragma once
#include <graph.h>
#include <algorithm>
#define NUM_CARS 1
#define NUM_PACKAGES 10

class solve {
public:
	struct package {
		int id;
		int weight;
		int time_limit;
		int destination;
		int prior;
		bool done;
	};
	struct car {
		int id;
		int v;
		int weight_car;
		int weight_limit;
	};
	struct interrupt {
		int time;
		int pac_id;
		int pac_prior;
	};

	solve() : g(), cars(), pacs() {}
	solve(graph _g, car _cars[], package _pacs[]) {
		g = _g;
		std::copy(_cars, _cars + NUM_CARS, cars);
		std::copy(_pacs, _pacs + NUM_PACKAGES, pacs);
		/*
		调整pacs数组顺序，按优先级降序
		std::sort 的使用：std::sort 是 C++ 标准库提供的排序算法，时间复杂度为 𝑂(𝑛log𝑛)。第三个参数是自定义的比较函数，这里使用了 Lambda 表达式。
		*/
		std::sort(pacs, pacs + NUM_PACKAGES, [](const package& a, const package& b) {
			return a.prior > b.prior; // 优先级高的排在前面
		});
	}
	~solve() = default;

	void strategy_1(bool interrupt = false);
	void strategy_2(bool interrupt = false);
private:
	graph g;
	struct car cars[NUM_CARS];
	struct package pacs[NUM_PACKAGES];
};