#include "solve.h"
#include <iostream>

/*
����һ��
�������ȼ��ߵͣ�ÿ������һ���������ͬ���ȼ��°�id������ÿ�ε���Ŀ�ĵغ��������վA��0��ȡ��
��ʹ���жϹ��ܣ�_interrupt==true������ÿ���жϺ�����δ�͵İ���
*/
void solve::strategy_1(bool _interrupt) {	// Ĭ�ϲ�ʹ����;�޸����ȼ�����
	std::cout << "\n---------- Strategy One ----------" << std::endl;
	std::vector<interrupt> itrs;	// �жϼ�¼
	if (_interrupt) {
		std::cout << "�������ж�ʱ�� �޸İ������ �޸ĺ�����ȼ�����ʽ��\"%d %d %d\"��" << std::endl;
		char c_tmp;
		// ����ѭ��
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
			std::cout << "�Ƿ�������ϣ���y/n��" << std::endl;
			std::cin >> c_tmp;
			while ((c_tmp != 'y') && (c_tmp != 'n')) {
				std::cout << "��Ч�����룡" << std::endl;
				std::cout << "�Ƿ�������ϣ���y/n��" << std::endl;
				std::cin >> c_tmp;
			}
		} while (c_tmp == 'n');
	}
	float time = 0.0;
	int count_false = 0;
	int w_able = cars[0].weight_limit - cars[0].weight_car;

	for (int i = 0;i < NUM_PACKAGES;i++) {
		std::cout << "---------- ��" << i+1 << "������ ----------" << std::endl;
		std::cout << "����id:" << pacs[i].id << "   ���ȼ���" << pacs[i].prior << std::endl;
		if (pacs[i].weight > w_able) {
			std::cout << "���أ�����" << std::endl;
			count_false++;
			continue;
		}
		// �����·��
		graph::Path p = g.find_path(0, pacs[i].destination);
		std::cout << "·����";
		for (graph::Vertex& i : p.path) {
			std::cout << " " << i.id;
		}
		std::cout << "\n·�̣�" << p.distance << std::endl;
		float time_deliver = float(p.distance) / cars[0].v;
		std::cout << "Ԥ�ư��ʱ�䣺" << time_deliver << "   ��ǰʱ�䣺" << time << std::endl;
		if (time + time_deliver > pacs[i].time_limit) {
			std::cout << "ʱ�䲻�㣬����" << std::endl;
			count_false++;
			continue;
		}
		else {
			time += 2 * time_deliver;
		}
		pacs[i].done = true;
		// ����Ƿ��ж�
		if (_interrupt) {
			for (auto it = itrs.begin(); it != itrs.end(); ) {
				if (time > it->time) {
					std::cout << "\n--- �ж� ---" << std::endl;
					std::cout << it->time << " " << it->pac_id << " " << it->pac_prior << std::endl;
					for (package& j : pacs) {
						if (it->pac_id == j.id) {
							if (j.done) {
								std::cout << "��Ч�����޸İ������ʹ�" << std::endl;
							}
							else {
								j.prior = it->pac_prior;
							}
							break;
						}
					}
					// ɾ����ǰԪ�أ����µ�����
					it = itrs.erase(it);
				}
				else {
					++it; // �����ɾ����ǰԪ�أ��ƶ�����һ��
				}
			}
			std::sort(pacs + i + 1, pacs + NUM_PACKAGES, [](const package& a, const package& b) {
				return a.prior > b.prior;
				});
		}
	}
	std::cout << "\n��ʱ�䣺" << time << "   δ��ɵĻ�������" << count_false << std::endl;
}

/*
���Զ���
�������ȼ��ߵͣ�ÿ�����;����ܶ�Ļ������ͬ���ȼ��°�id����
ÿ�ε���Ŀ�ĵغ������а�������ֱ��ǰ����һ��������Ŀ�ĵ�
��������û�а���ʱ������վA��0��ȡ��
��ʹ���жϹ��ܣ�_interrupt==true������ÿ���жϺ�����δ�͵İ���
*/
void solve::strategy_2(bool _interrupt) {	// Ĭ�ϲ�ʹ����;�޸����ȼ�����
	std::cout << "\n---------- Strategy Two ----------" << std::endl;
	std::vector<interrupt> itrs;	// �жϼ�¼
	if (_interrupt) {
		std::cout << "�������ж�ʱ�� �޸İ������ �޸ĺ�����ȼ�����ʽ��\"%d %d %d\"��" << std::endl;
		char c_tmp;
		// ����ѭ��
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
			std::cout << "�Ƿ�������ϣ���y/n��" << std::endl;
			std::cin >> c_tmp;
			while ((c_tmp != 'y') && (c_tmp != 'n')) {
				std::cout << "��Ч�����룡" << std::endl;
				std::cout << "�Ƿ�������ϣ���y/n��" << std::endl;
				std::cin >> c_tmp;
			}
		} while (c_tmp == 'n');
	}
	float time = 0.0;
	int count_false = 0;
	int w_able = cars[0].weight_limit - cars[0].weight_car;

	for (int i = 0, count_deliver = 1;i < NUM_PACKAGES;i++) {
		std::cout << "---------- ��" << count_deliver << "������ ----------" << std::endl;
		std::vector<int> current_pacs;
		int current_num = 0, current_weight = 0;
		// ȡ����
		while (pacs[i].weight > w_able) {
			std::cout << "����id:" << pacs[i].id << "   ���ȼ���" << pacs[i].prior << std::endl;
			std::cout << "���أ�����" << std::endl;
			count_false++;
			pacs[i].done = true;
			i++;
			if (i >= NUM_PACKAGES) {
				break;
			}
		}
		if (i >= NUM_PACKAGES) {
			std::cout << "\n��ʱ�䣺" << time << "   δ��ɵĻ�������" << count_false << std::endl;
		}

		std::cout << "����id:" << pacs[i].id << "   ���ȼ���" << pacs[i].prior << std::endl;
		current_pacs.push_back(pacs[i].id - 1);
		current_num++;
		current_weight += pacs[i].weight;
		// ����Ƿ�������һ����������˳��
		while (i + 1 < NUM_PACKAGES && pacs[i + 1].weight + current_weight <= w_able) {
			pacs[i].done = true;
			i++;
			std::cout << "����id:" << pacs[i].id << "   ���ȼ���" << pacs[i].prior << std::endl;
			current_pacs.push_back(pacs[i].id - 1);
			current_num++;
			current_weight += pacs[i].weight;
		}
		std::cout << "��ǰ��������" << current_num << "   ��������" << current_weight << std::endl;

		// �����·��
		int current_vertex = 0;
		for (int id : current_pacs) {
			graph::Path p = g.find_path(current_vertex, pacs[id].destination);
			std::cout << "·����";
			for (graph::Vertex& i : p.path) {
				std::cout << " " << i.id;
			}
			std::cout << "\n·�̣�" << p.distance << std::endl;
			float time_deliver = float(p.distance) / cars[0].v;
			std::cout << "Ԥ��ʱ�䣺" << time_deliver << "   ��ǰʱ�䣺" << time << std::endl;
			if (time + time_deliver > pacs[i].time_limit) {
				std::cout << "ʱ�䲻�㣬����" << std::endl;
				count_false++;
				continue;
			}
			else {
				time += time_deliver;
				current_vertex = pacs[id].destination;
			}
		}
		if (current_vertex != 0) {
			// �ص���վ
			graph::Path p = g.find_path(current_vertex, 0);
			std::cout << "·����";
			for (graph::Vertex& i : p.path) {
				std::cout << " " << i.id;
			}
			std::cout << "\n·�̣�" << p.distance << std::endl;
			float time_deliver = float(p.distance) / cars[0].v;
			std::cout << "Ԥ��ʱ�䣺" << time_deliver << "   ��ǰʱ�䣺" << time << std::endl;
			time += time_deliver;

		}
		count_deliver++;	// �����������

		// ����Ƿ��ж�
		if (_interrupt) {
			for (auto it = itrs.begin(); it != itrs.end(); ) {
				if (time > it->time) {
					std::cout << "\n--- �ж� ---" << std::endl;
					std::cout << it->time << " " << it->pac_id << " " << it->pac_prior << std::endl;
					for (package& j : pacs) {
						if (it->pac_id == j.id) {
							if (j.done) {
								std::cout << "��Ч�����޸İ������ʹ�" << std::endl;
							}
							else {
								j.prior = it->pac_prior;
							}
							break;
						}
					}
					// ɾ����ǰԪ�أ����µ�����
					it = itrs.erase(it);
				}
				else {
					++it; // �����ɾ����ǰԪ�أ��ƶ�����һ��
				}
			}
			std::sort(pacs + i + 1, pacs + NUM_PACKAGES, [](const package& a, const package& b) {
				return a.prior > b.prior;
				});
		}
	}
	std::cout << "\n��ʱ�䣺" << time << "   δ��ɵĻ�������" << count_false << std::endl;
}