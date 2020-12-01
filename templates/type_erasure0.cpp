
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <numeric>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <typeinfo>

struct Weapon {
	bool can_attack() const { return true; } // All weapons can do damage
};

struct Armor {
	bool can_attack() const { return false; } // Cannot attack with armor...
};

struct Helmet {
	bool can_attack() const { return false; } // Cannot attack with helmet...
};

struct Scroll {
	bool can_attack() const { return false; }
};

struct FireScroll {
	bool can_attack() const { return true; }
};

struct Potion {
	bool can_attack() const { return false; }
};


struct PoisonPotion {
	bool can_attack() const { return true; }
};


class Object {
	struct ObjectConcept {
		virtual ~ObjectConcept() {}
		virtual bool has_attack_concept() const = 0;
		virtual std::string name() const = 0;
	};

	template< typename T > struct ObjectModel : ObjectConcept {
		ObjectModel(const T& t) : object(t) {}
		virtual ~ObjectModel() {}
		virtual bool has_attack_concept() const
		{
			return object.can_attack();
		}
		virtual std::string name() const
		{
			return "xxxx";
		}
	private:
		T object;
	};

	std::shared_ptr<ObjectConcept> object;

public:
	template< typename T > Object(const T& obj) :
		object(new ObjectModel<T>(obj)) {}

	std::string name() const
	{
		return object->name();
	}

	bool has_attack_concept() const
	{
		return object->has_attack_concept();
	}
};

int main() {
	typedef std::vector< Object >    Backpack;
	typedef Backpack::const_iterator BackpackIter;

	Backpack backpack;

	backpack.push_back(Object(Weapon()));
	backpack.push_back(Object(Armor()));
	backpack.push_back(Object(Potion()));
	backpack.push_back(Object(Scroll()));
	backpack.push_back(Object(FireScroll()));
	backpack.push_back(Object(PoisonPotion()));

	std::cout << "Items I can attack with:" << std::endl;
	for (BackpackIter item = backpack.begin(); item != backpack.end(); ++item)
	if (item->has_attack_concept())
		std::cout << item->name();
}

