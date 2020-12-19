#pragma once
#include<stdexcept>
#include<memory.h>
#include<windows.h>
class Grid {
public:
	Grid() :m_last_state(9), m_is_end(false), m_side(1), m_limit(0), m_current_state(0), m_winner(0),
		m_last_pos{-1,-1},m_count(0){
		memset(m_grid, 0, sizeof m_grid);
	}

	int get_side() const { return m_side; }

	bool is_end()const { return m_is_end; }
	
	bool is_limit()const { return m_limit; }

	bool is_running()const { return !m_is_end; }

	int get(const POINT& pos)const {
		if (pos.x < 0 || pos.x>8 || pos.y < 0 || pos.y>8)
			throw std::out_of_range("����������Χ");
		return m_grid[pos.x][pos.y];
	}

	int get_winner(int state)const {
		if (state < 0 || state>8)
			throw std::out_of_range("�����ų�������");
		return m_grid[9][state];
	}

	int get_global_winner()const {
		if (is_running())
			throw std::runtime_error("������δ����");
		return m_winner;
	}

	int get_current_state()const { return m_current_state; }

	int get_state_count(int state)const {
		if (state < 0 || state>8)
			throw std::out_of_range("�����ų�������");
		return m_grid[10][state];
	}

	POINT get_last_pos()const {
		return m_last_pos;
	}

	int get_count()const { return m_count; }

	bool try_press(const POINT& pos) {
		if (m_is_end)return false;
		if (pos.x < 0 || pos.x>8 || pos.y < 0 || pos.y>8)return false;
		if (m_grid[pos.x][pos.y])return false;
		if (m_grid[9][pos.x])return false;
		if (m_limit && pos.x != m_current_state)return false;
		return true;
	}

	void press(const POINT& pos) {
		#pragma region �쳣�ж�
		if (m_is_end)
			throw std::runtime_error("�����ѽ���");
		if (pos.x < 0 || pos.x>8 || pos.y < 0 || pos.y>8)
			throw std::out_of_range("���Ϸ�������λ��");
		if (m_grid[pos.x][pos.y])
			throw std::runtime_error("��λ����������");
		if(m_grid[9][pos.x])
			throw std::runtime_error("�������ѱ�ռ��");
		if (m_limit && pos.x != m_current_state)
			throw std::runtime_error("�����ڵ�ǰ���Ƶ���������");
		#pragma endregion

		m_grid[pos.x][pos.y] = m_side;

		if (check(pos.x)) {
			set_winner(pos.x, m_side);
			if (check(9)) {
				m_winner = m_side;
				m_is_end = true;
			}
		}
		add_count(pos.x);
		if (get_state_count(pos.x) == 9 &&get_winner(pos.x) == 0)
		{
			set_winner(pos.x, 3);
		}
		if (get_winner(pos.x) && --m_last_state == 0) {
			m_is_end = true;
			m_winner = 3;
		}
		m_current_state = pos.y;
		if (get_winner(pos.y))m_limit = 0;
		else m_limit = 1;
		m_side = 3 - m_side;
		m_last_pos = pos;
		m_count++;
	}
private:
	//���ĳ�������Ƿ�ռ��
	//state������ţ���Ϊ9����Ϊ�������Ƿ��ѽ���
	//Ϊ�˷�ֹ�Գ飬����ûʲô�ã�������������
	bool check(int state) {
		if (state < 0 || state>9)
			throw std::out_of_range("�����ų�������");
		//0 1 2
		//3 4 5
		//6 7 8
		static const int
			check_num[8][3] = { {0,1,2},{3,4,5},{6,7,8},
								{0,3,6},{1,4,7},{2,5,8},
								{0,4,8},{2,4,6} };
		bool result = false;
		for (int i = 0; i < 8 && !result; i++) {
			if (m_grid[state][check_num[i][0]] == m_side &&
				m_grid[state][check_num[i][1]] == m_side &&
				m_grid[state][check_num[i][2]] == m_side)
				result = true;
		}
		return result;
	}

	void set_winner(int state, int winner) {
		if (state < 0 || state>8)
			throw std::out_of_range("�����ų�������");
		m_grid[9][state] = winner;
	}

	void add_count(int state) {
		if (state < 0 || state>8)
			throw std::out_of_range("�����ų�������");
		++m_grid[10][state];
	}
private:
	//���̣�m_grid[0-8][ ]Ϊ�������ݣ�
	//m_grid[9][ ]Ϊÿ�������ʤ��
	//m_grid[10][ ]Ϊÿ���������µ�����
	int m_grid[11][9];
	int m_last_state;//ʣ��Ŀ���������
	int m_side;//��ǰ�������
	bool m_is_end;//�����Ƿ��ѽ���
	bool m_limit;//�Ƿ�����������
	int m_current_state;//������������ƣ���ǰӦ�����ĸ�����
	int m_winner;//ʤ��
	POINT m_last_pos;//��һ�ε���������
	int m_count;//�����������ڵ���
};