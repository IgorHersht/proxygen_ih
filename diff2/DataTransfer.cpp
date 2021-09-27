#include <type_traits>
#include <iostream>
#include <memory>
#include <queue>
#include<vector>
#include <tuple>
#include<typeinfo>
#include<thread>
#include <mutex>
#include <condition_variable>

enum ThreadSafetyType {NO_THREAD_SAFETY, CONDITIONAL_VARIABLE_THREAD_SAFETY, FEATURE_THREAD_SAFETY, BATON_THREAD_SAFETY};
template <typename T,  typename ContainerT, ThreadSafetyType  sst> struct DataQueue;

template <typename T> using SingleElementData = DataQueue<T, std::unique_ptr<T>, NO_THREAD_SAFETY >;
template <typename T> using QueueElementData = DataQueue<T, std::queue<std::unique_ptr<T> >, NO_THREAD_SAFETY  >;


template <typename T, typename ContainerT> using DataCVSafe= DataQueue<T, ContainerT, CONDITIONAL_VARIABLE_THREAD_SAFETY  >;
template <typename T> using SingleElementDataCVSafe=  DataCVSafe<T, SingleElementData<T> >;
template <typename T> using QueueElementDataCVSafe =  DataCVSafe<T, QueueElementData<T> >;


template <typename T> struct DataQueue<T, std::unique_ptr<T>, NO_THREAD_SAFETY >{
	using Type = T;
	using ElementT = 	std::unique_ptr<T>;
	using  ContainerT = std::unique_ptr<T>;
	void moveIn(ElementT &&element){
		m_container = std::move(element);
	}
	ElementT moveOut(){
		return std::move(m_container);// std::move on nonlocal!!!
	}
	bool empty() const{
		return (m_container == nullptr) ;
	}
	bool full() const{
			return (m_container != nullptr);
	}
private:
	 ContainerT m_container;
};

constexpr int MAX_QUEUE_SIZE = 20;
/////
template <typename T> struct DataQueue<T, std::queue<std::unique_ptr<T> >, NO_THREAD_SAFETY >{
	using Type = T;
	using ElementT = std::unique_ptr<T>;
	using  ContainerT = std::queue< ElementT >;// value_type
	void moveIn(ElementT &&element){
		m_container.push(std::move(element));
	}
	ElementT moveOut(){
		ElementT el = std::move(m_container.front());
		m_container.pop();
		return el;// // no std::move on local!!!
	}
	bool empty() const{
		return m_container.empty() ;
	}
	bool full() const{
			return (m_container.size() > MAX_QUEUE_SIZE);
	}
private:
	 ContainerT m_container;
};

//////
/// thread safe
template <typename T,typename ContainerT > struct  DataQueue<T, ContainerT, CONDITIONAL_VARIABLE_THREAD_SAFETY  >{
	using Type = typename ContainerT::Type;;
	using ElementT = typename ContainerT::ElementT;

	void moveIn(ElementT &&element){
		std::unique_lock<std::mutex> lk( m_mutex);
		m_cond_not_full.wait(
						lk,[this]{return !m_container.full();});
		m_container.moveIn(std::move(element));
		m_cond_not_empty.notify_one();

	}
	ElementT moveOut(){
		std::unique_lock<std::mutex> lk( m_mutex);
		m_cond_not_empty.wait(
				lk,[this]{return !m_container.empty();});
		ElementT el = m_container.moveOut();
		m_cond_not_full.notify_one();
		return el;
	   }

	bool empty() const{
		std::lock_guard<std::mutex> lk( m_mutex);
		return m_container.empty();
	}

	bool full() const{
		std::lock_guard<std::mutex> lk( m_mutex);
		return m_container.full();
	}

private:
	 ContainerT m_container;
	 mutable std::mutex m_mutex;
	 std::condition_variable m_cond_not_empty;
	 std::condition_variable m_cond_not_full;

};




//////////////////// test
void testSingleElement(){
	SingleElementData<int> ud;
	auto d1 = std::make_unique<int>(int(5));
	ud.moveIn(std::move(d1));
	bool empty = ud.empty();
	std::unique_ptr<int> d11 =ud.moveOut() ;
	empty = ud.empty();
	bool is_null = (d11 == nullptr);
}

void testQueueElement(){
	QueueElementData<int> ud;
	auto d1 = std::make_unique<int>(int(5));
	auto d2 = std::make_unique<int>(int(10));
	ud.moveIn(std::move(d1));
	bool empty = ud.empty();
	ud.moveIn(std::move(d2));

	std::unique_ptr<int> d11 = ud.moveOut();
	std::unique_ptr<int> d12 = ud.moveOut();

	empty = ud.empty();
	bool is_null = (d11 == nullptr);
	is_null = (d12 == nullptr);
	int id1 = *d11;
	int id2 = *d12;

}

template <typename QT> void push( QT &q)
{
   using Type = typename QT::Type;
   for(int i = 0; i <20; ++i){
       q.moveIn(std::make_unique<Type>(i));

   }
}

template <typename QT> void pop(QT &q)
{
   for(int i = 0; i <20; ++i){
       auto p = std::move(q.moveOut());
      std::cout <<"pop:"<<*p <<std::endl;
   }
}

void testSingleElementDataCVSafe(){
	using QT = SingleElementDataCVSafe<int>;
	std::shared_ptr<QT> qpl(new QT);

	std::thread t1([qp = qpl]{ pop(*qp) ;});
	std::thread t2([qp = qpl]{ push(*qp) ;});
	t2.join();
	t1.join();

}

void testQueueElementDataCVSafe(){
	using QT = QueueElementDataCVSafe<int>;
	std::shared_ptr<QT> qpl(new QT);

	std::thread t1([qp = qpl]{ pop(*qp) ;});
	std::thread t2([qp = qpl]{ push(*qp) ;});
	t2.join();
	t1.join();

}



int main(int argc, char** argv){

	testSingleElement();
	testQueueElement();
	testSingleElementDataCVSafe();
	testQueueElementDataCVSafe();


 return 0;
}

