#include <boost/circular_buffer.hpp>
#include <memory>
#include <assert.h>

typedef std::unique_ptr<int> ET;
boost::circular_buffer<ET> cb(3);

int main()
{

	ET e1 = std::make_unique<int>(1);
	ET e2 = std::make_unique<int>(2);
	ET e3 = std::make_unique<int>(3);
	ET e4 = std::make_unique<int>(4);

	cb.push_back(std::move(e1));
	cb.push_back(std::move(e2));
	cb.push_back(std::move(e3));
	cb.push_back(std::move(e4));

	assert(*cb[0] == 2);
	assert(*cb[1] == 3);
	assert(*cb[2] == 4);

	assert(*cb.front() == 2);
	cb.pop_front();
	assert(*cb.front() == 3);


    return 0;
}

