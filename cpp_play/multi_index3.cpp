//#if defined(DEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
//#endif


#include <string>
#include <type_traits>
#include <boost/lexical_cast.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


struct employee_type{
	int         m_id;
	int         m_ss;
	std::string m_name;
	employee_type(int id, int ss, const std::string &name) :
		m_id{ id }, m_ss{ ss }, m_name{ name }{}

	std::string to_string() const{
		std::string out(boost::lexical_cast<std::string>(m_id)); out += " ";
		out += boost::lexical_cast<std::string>(m_ss); out += " ";
		out += m_name;
		return out;
	}

};

namespace mi = boost::multi_index;

struct  employee_type_map{

	typedef employee_type value_type;
	static_assert(std::is_standard_layout<value_type>::value, "value_type should have standard layout. ");
	struct id_index_type{
		typedef int 														key_type;
		typedef mi::member<value_type, key_type, &value_type::m_id> 		member_type;
	};
	struct ss_index_type{
		typedef int 														key_type;
		typedef mi::member<value_type, key_type, &value_type::m_ss>			member_type;
	};
	struct name_index_type{
		typedef std::string 												key_type;
		typedef mi::member<value_type, key_type, &value_type::m_name> 		member_type;
	};
private:

	typedef boost::multi_index_container<
		value_type,
		mi::indexed_by<
		mi::ordered_unique< mi::tag<id_index_type>, id_index_type::member_type >,
		mi::ordered_unique<mi::tag<ss_index_type>, ss_index_type::member_type >,
		mi::ordered_non_unique<mi::tag<name_index_type>, name_index_type::member_type >
		>
	> value_type_set;

private:
	value_type_set m_values;

public:

	template <typename index_type> auto begin() -> decltype (m_values.get<index_type>().begin()) {
		return m_values.get<index_type>().begin();
	}

	template <typename index_type> auto end() -> decltype (m_values.get<index_type>().end()) {
		return m_values.get<index_type>().end();
	}
	bool insert(const value_type &value){
		return m_values.insert(value).second;
	}

	template <typename index_type> const value_type* get(const typename index_type::key_type &key){
		auto iter = m_values.get<index_type>().find(key);
		if (m_values.get<index_type>().end() == iter){
			return nullptr;
		}
		else{
			return &(*iter);
		}
	}
	template <typename index_type> int erase(const typename index_type::key_type &key){
		return m_values.get<index_type>().erase(key);
	}

};

#include <iostream>

int main()
{
	employee_type_map map;

	map.insert(employee_type(0, 31, "Joe"));
	map.insert(employee_type(1, 27, "Robert"));
	map.insert(employee_type(2, 40, "John"));
	map.insert(employee_type(2, 2387, "Aristotle"));
	map.insert(employee_type(3, 20, "Albert"));
	map.insert(employee_type(4, 57, "John"));


	std::cout << "print_by_name" << std::endl;
	typedef employee_type_map::name_index_type nt;
	for (auto pos = map.begin<nt>(), end = map.end<nt>(); pos != end; ++pos){
		std::cout << pos->to_string() << std::endl;
	}

	std::cout << "get_by_id" << std::endl;
	const employee_type *e1 = map.get<employee_type_map::id_index_type>(2);
	const employee_type *e2 = map.get<employee_type_map::name_index_type>("Joe");

	std::cout << "erase_by_id" << std::endl;
	map.erase<employee_type_map::id_index_type>(2);


	for (auto pos = map.begin<nt>(), end = map.end<nt>(); pos != end; ++pos){
		std::cout << pos->to_string() << std::endl;
	}



	return 0;
}

