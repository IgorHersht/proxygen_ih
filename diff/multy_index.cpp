

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

namespace mi = boost::multi_index;

struct employee_type
	{
	  int         m_id;
	  int         m_ss;
	  std::string m_name;
	  employee_type(int id, int ss, const std::string &name):
		  m_id{id}, m_ss{ss},m_name{name}{}
	  friend std::ostream& operator<<(std::ostream& os,const employee_type& e)
		{
		  os<<e.m_id<<" "<<e.m_ss<<" "<<e.m_name<<std::endl;
		  return os;
		}
	};

struct  employee_type_map{

bool insert(const employee_type &employee ){
	return m_employee.insert(employee).second;
}
void print(){
	for(auto em: m_employee){
		std::cout <<em;
	}
}
void print_by_name(){
	for(auto em: m_employee.get<2>()){
		std::cout <<em;
	}
}

void print_by_sequetcy(){
	for(auto em: m_employee.get<3>()){
		std::cout <<em;
	}
}
const employee_type* get_by_id(int id){
	auto iter = m_employee.get<0>().find(id);
	if(m_employee.get<0>().end() == iter ){
		return nullptr;
	}
	else{
		return &(*iter);
	}
}
int erase_by_id(int id){
	return m_employee.get<0>().erase(id);

}
private:


	typedef boost::multi_index_container<
	  employee_type,

	  mi::indexed_by<
		   mi::ordered_unique<mi::member<employee_type,int,&employee_type::m_id> >,
		   mi::ordered_unique<mi::member<employee_type,int,&employee_type::m_ss> >,
		   mi::ordered_non_unique<mi::member<employee_type,std::string,&employee_type::m_name> >,
		   mi::sequenced<>

	  >
	> employee_type_set;

private:
	employee_type_set m_employee;

};
int main()
{
	employee_type_map map;
	  std::cout << "all employ" <<std::endl;
	  map.insert(employee_type(0, 31, "Joe"));
	  map.insert(employee_type(1,27,"Robert"));
	  map.insert(employee_type(2,40,"John"));
	  map.insert(employee_type(2,2387,"Aristotle"));
	  map.insert(employee_type(3, 20,"Albert"));
	  map.insert(employee_type(4, 57,"John"));
	  map.print();// no duplicates

	  std::cout << "print_by_name" <<std::endl;
	  map.print_by_name();// no duplicates

	  std::cout << "print_by_sequetcy" <<std::endl;
	  map.print_by_sequetcy();// no duplicates

	  std::cout << "get_by_id" <<std::endl;
	  const employee_type *e1 = map.get_by_id(2);
	  if(e1) std::cout << *e1;
	  const employee_type *e2 = map.get_by_id(33);
	  if(e2) std::cout << *e2;

	  std::cout << "erase_by_id" <<std::endl;
	  map.erase_by_id(2);
	  map.print();




  return 0;
}

