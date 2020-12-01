#include <future>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <chrono>


struct MakeOrder{

	void A_s(){
		for(int j =0; j < m_max_iter; ++j){
			A_once_s();
		}
	}

	void B_s(){
		for(int j =0; j < m_max_iter; ++j){
			B_once_s();
		}
	}

	void A_once_s(){
		std::unique_lock<std::mutex> lk(m_m);
		m_bcv.wait(lk,[this]{return a_can_go() ;});
		A_once();
		m_b_last = false;
		m_acv.notify_one();

	}
	void B_once_s(){
		std::unique_lock<std::mutex> lk(m_m);
		m_acv.wait(lk,[this]{return b_can_go() ;});
		B_once();
		m_b_last = true;
		m_bcv.notify_one();
	}


private:
void A_once(){
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout <<"A";
}
void B_once(){
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout <<"B";
}

bool a_can_go(){
	return m_b_last;
}
bool b_can_go(){
	return !m_b_last;
}

private:
	int m_max_iter{5};
	bool m_b_last {true};
	std::condition_variable m_acv;
	std::condition_variable m_bcv;

	mutable std::mutex m_m;

};

int main(){
	MakeOrder mo;
	std::thread ta(&MakeOrder::A_s, &mo);
	std::thread tb(&MakeOrder::B_s, &mo);
	tb.join();
	ta.join();
	std::cout <<"\nDone";
	return 0;
}

