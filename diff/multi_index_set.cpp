
//#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
//#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE


#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

using boost::multi_index_container;
using namespace boost::multi_index;

/* an employee record holds its ID, name and age */

struct employee
{
	int         id;
	std::string name;
	int         age;

	employee(int id_, std::string name_, int age_) :id(id_), name(name_), age(age_){}

	friend std::ostream& operator<<(std::ostream& os, const employee& e)
	{
		os << e.id << " " << e.name << " " << e.age << std::endl;
		return os;
	}
};

/* tags for accessing the corresponding indices of employee_set */

struct id{};
struct name{};
struct age{};

/* see Compiler specifics: Use of member_offset for info on
* BOOST_MULTI_INDEX_MEMBER
*/

/* Define a multi_index_container of employees with following indices:
*   - a unique index sorted by employee::int,
*   - a non-unique index sorted by employee::name,
*   - a non-unique index sorted by employee::age.
*/

typedef multi_index_container<
	employee,
	indexed_by<
	ordered_unique<
	tag<id>, BOOST_MULTI_INDEX_MEMBER(employee, int, id)>,
	ordered_non_unique<
	tag<name>, BOOST_MULTI_INDEX_MEMBER(employee, std::string, name)>,
	ordered_non_unique<
	tag<age>, BOOST_MULTI_INDEX_MEMBER(employee, int, age)> >
> employee_set;

template<typename Tag, typename MultiIndexContainer>
void print_out_by(const MultiIndexContainer& s)
{
	/* obtain a reference to the index tagged by Tag */

	const typename boost::multi_index::index<MultiIndexContainer, Tag>::type& i =
		get<Tag>(s);

	typedef typename MultiIndexContainer::value_type value_type;

	/* dump the elements of the index to cout */

	std::copy(i.begin(), i.end(), std::ostream_iterator<value_type>(std::cout));
}


int main()
{
	employee_set es;

	bool in_status = es.insert(employee(0, "Joe", 31)).second;
	in_status = es.insert(employee(1, "Robert", 27)).second;
	in_status = es.insert(employee(2, "John", 40)).second;

	/* next insertion will fail, as there is an employee with
	* the same ID
	*/

	in_status = es.insert(employee(2, "Aristotle", 2387)).second;

	in_status = es.insert(employee(3, "Albert", 20)).second;
	in_status = es.insert(employee(4, "John", 57)).second;

	/* list the employees sorted by ID, name and age */

	std::cout << "by ID" << std::endl;
	print_out_by<id>(es);
	std::cout << std::endl;

	std::cout << "by name" << std::endl;
	print_out_by<name>(es);
	std::cout << std::endl;

	std::cout << "by age" << std::endl;
	print_out_by<age>(es);
	std::cout << std::endl;

	return 0;
}

