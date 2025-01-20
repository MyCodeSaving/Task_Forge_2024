#include "solve.h"
#include <iostream>

/*
策略一：
按照优先级高低，每次运送一个货物，（相同优先级下按id排序）且每次到达目的地后均返回驿站A（0）取件
若使用中断功能（_interrupt==true），则每次中断后重排未送的包裹
*/
void solve::strategy_1(bool _interrupt) {	// 默认不使用中途修改优先级功能
	std::cout << "\n---------- Strategy One ----------" << std::endl;
	std::vector<interrupt> itrs;	// 中断记录
	if (_interrupt) {
		std::cout << "请输入中断时间 修改包裹编号 修改后的优先级（格式：\"%d %d %d\"）" << std::endl;
		char c_tmp;
		// 输入循环
		do {
			int tmp = 0;
			interrupt i_tmp;
			std::cin >> tmp;
			i_tmp.time = tmp;
			std::cin >> tmp;
			i_tmp.pac_id = tmp;			
			std::cin >> tmp;
			i_tmp.pac_prior = tmp;
			itrs.push_back(i_tmp);
			std::cout << "是否输入完毕？（y/n）" << std::endl;
			std::cin >> c_tmp;
			while ((c_tmp != 'y') && (c_tmp != 'n')) {
				std::cout << "无效的输入！" << std::endl;
				std::cout << "是否输入完毕？（y/n）" << std::endl;
				std::cin >> c_tmp;
			}
		} while (c_tmp == 'n');
	}
	float time = 0.0;
	int count_false = 0;
	int w_able = cars[0].weight_limit - cars[0].weight_car;

	for (int i = 0;i < NUM_PACKAGES;i++) {
		std::cout << "---------- 第" << i+1 << "个包裹 ----------" << std::endl;
		std::cout << "包裹id:" << pacs[i].id << "   优先级：" << pacs[i].prior << std::endl;
		if (pacs[i].weight > w_able) {
			std::cout << "超重，舍弃" << std::endl;
			count_false++;
			continue;
		}
		// 找最短路径
		graph::Path p = g.find_path(0, pacs[i].destination);
		std::cout << "路径：";
		for (graph::Vertex& i : p.path) {
			std::cout << " " << i.id;
		}
		std::cout << "\n路程：" << p.distance << std::endl;
		float time_deliver = float(p.distance) / cars[0].v;
		std::cout << "预计半程时间：" << time_deliver << "   当前时间：" << time << std::endl;
		if (time + time_deliver > pacs[i].time_limit) {
			std::cout << "时间不足，舍弃" << std::endl;
			count_false++;
			continue;
		}
		else {
			time += 2 * time_deliver;
		}
		pacs[i].done = true;
		// 检测是否中断
		if (_interrupt) {
			for (auto it = itrs.begin(); it != itrs.end(); ) {
				if (time > it->time) {
					std::cout << "\n--- 中断 ---" << std::endl;
					std::cout << it->time << " " << it->pac_id << " " << it->pac_prior << std::endl;
					for (package& j : pacs) {
						if (it->pac_id == j.id) {
							if (j.done) {
								std::cout << "无效，待修改包裹已送达" << std::endl;
							}
							else {
								j.prior = it->pac_prior;
							}
							break;
						}
					}
					// 删除当前元素，更新迭代器
					it = itrs.erase(it);
				}
				else {
					++it; // 如果不删除当前元素，移动到下一个
				}
			}
			std::sort(pacs + i + 1, pacs + NUM_PACKAGES, [](const package& a, const package& b) {
				return a.prior > b.prior;
				});
		}
	}
	std::cout << "\n总时间：" << time << "   未完成的货物数：" << count_false << std::endl;
}

/*
策略二：
按照优先级高低，每次运送尽可能多的货物，（相同优先级下按id排序）
每次到达目的地后若仍有包裹，则直接前往下一个包裹的目的地
当货车上没有包裹时返回驿站A（0）取件
若使用中断功能（_interrupt==true），则每次中断后重排未送的包裹
*/
void solve::strategy_2(bool _interrupt) {	// 默认不使用中途修改优先级功能
	std::cout << "\n---------- Strategy Two ----------" << std::endl;
	std::vector<interrupt> itrs;	// 中断记录
	if (_interrupt) {
		std::cout << "请输入中断时间 修改包裹编号 修改后的优先级（格式：\"%d %d %d\"）" << std::endl;
		char c_tmp;
		// 输入循环
		do {
			int tmp = 0;
			interrupt i_tmp;
			std::cin >> tmp;
			i_tmp.time = tmp;
			std::cin >> tmp;
			i_tmp.pac_id = tmp;
			std::cin >> tmp;
			i_tmp.pac_prior = tmp;
			itrs.push_back(i_tmp);
			std::cout << "是否输入完毕？（y/n）" << std::endl;
			std::cin >> c_tmp;
			while ((c_tmp != 'y') && (c_tmp != 'n')) {
				std::cout << "无效的输入！" << std::endl;
				std::cout << "是否输入完毕？（y/n）" << std::endl;
				std::cin >> c_tmp;
			}
		} while (c_tmp == 'n');
	}
	float time = 0.0;
	int count_false = 0;
	int w_able = cars[0].weight_limit - cars[0].weight_car;

	for (int i = 0, count_deliver = 1;i < NUM_PACKAGES;i++) {
		std::cout << "---------- 第" << count_deliver << "次运输 ----------" << std::endl;
		std::vector<int> current_pacs;
		int current_num = 0, current_weight = 0;
		// 取包裹
		while (pacs[i].weight > w_able) {
			std::cout << "包裹id:" << pacs[i].id << "   优先级：" << pacs[i].prior << std::endl;
			std::cout << "超重，舍弃" << std::endl;
			count_false++;
			pacs[i].done = true;
			i++;
			if (i >= NUM_PACKAGES) {
				break;
			}
		}
		if (i >= NUM_PACKAGES) {
			std::cout << "\n总时间：" << time << "   未完成的货物数：" << count_false << std::endl;
		}

		std::cout << "包裹id:" << pacs[i].id << "   优先级：" << pacs[i].prior << std::endl;
		current_pacs.push_back(pacs[i].id - 1);
		current_num++;
		current_weight += pacs[i].weight;
		// 检查是否能拿下一个包裹（按顺序）
		while (i + 1 < NUM_PACKAGES && pacs[i + 1].weight + current_weight <= w_able) {
			pacs[i].done = true;
			i++;
			std::cout << "包裹id:" << pacs[i].id << "   优先级：" << pacs[i].prior << std::endl;
			current_pacs.push_back(pacs[i].id - 1);
			current_num++;
			current_weight += pacs[i].weight;
		}
		std::cout << "当前货物数：" << current_num << "   总重量：" << current_weight << std::endl;

		// 找最短路径
		int current_vertex = 0;
		for (int id : current_pacs) {
			graph::Path p = g.find_path(current_vertex, pacs[id].destination);
			std::cout << "路径：";
			for (graph::Vertex& i : p.path) {
				std::cout << " " << i.id;
			}
			std::cout << "\n路程：" << p.distance << std::endl;
			float time_deliver = float(p.distance) / cars[0].v;
			std::cout << "预计时间：" << time_deliver << "   当前时间：" << time << std::endl;
			if (time + time_deliver > pacs[i].time_limit) {
				std::cout << "时间不足，舍弃" << std::endl;
				count_false++;
				continue;
			}
			else {
				time += time_deliver;
				current_vertex = pacs[id].destination;
			}
		}
		if (current_vertex != 0) {
			// 回到驿站
			graph::Path p = g.find_path(current_vertex, 0);
			std::cout << "路径：";
			for (graph::Vertex& i : p.path) {
				std::cout << " " << i.id;
			}
			std::cout << "\n路程：" << p.distance << std::endl;
			float time_deliver = float(p.distance) / cars[0].v;
			std::cout << "预计时间：" << time_deliver << "   当前时间：" << time << std::endl;
			time += time_deliver;

		}
		count_deliver++;	// 递增运输次数

		// 检测是否中断
		if (_interrupt) {
			for (auto it = itrs.begin(); it != itrs.end(); ) {
				if (time > it->time) {
					std::cout << "\n--- 中断 ---" << std::endl;
					std::cout << it->time << " " << it->pac_id << " " << it->pac_prior << std::endl;
					for (package& j : pacs) {
						if (it->pac_id == j.id) {
							if (j.done) {
								std::cout << "无效，待修改包裹已送达" << std::endl;
							}
							else {
								j.prior = it->pac_prior;
							}
							break;
						}
					}
					// 删除当前元素，更新迭代器
					it = itrs.erase(it);
				}
				else {
					++it; // 如果不删除当前元素，移动到下一个
				}
			}
			std::sort(pacs + i + 1, pacs + NUM_PACKAGES, [](const package& a, const package& b) {
				return a.prior > b.prior;
				});
		}
	}
	std::cout << "\n总时间：" << time << "   未完成的货物数：" << count_false << std::endl;
}