#include "OOMockTests.h"
#include "cute.h"
#include <map>
#include <string>
namespace {
using namespace std::literals;

const auto TALISKER = "Talisker"s;
}
struct WarehouseInterface {
	virtual ~WarehouseInterface() {
	}

	virtual int getInventory(std::string const & s) const = 0;
	virtual void remove(std::string const & s, int i) = 0;
	virtual void add(std::string const & s, int i) = 0;
	virtual bool hasInventory(std::string const & s, int i) const = 0;
};

struct Warehouse: WarehouseInterface {
	int getInventory(std::string const & s) const {
		auto const it = wh.find(s);
		return (it != wh.end()) ? it->second : 0;
	}

	void remove(std::string const & s, int i) {
		wh[s] -= i;
	}

	void add(std::string const & s, int i) {
		wh[s] += i;
	}

	bool hasInventory(std::string const & s, int i) const {
		return getInventory(s) >= i;
	}

	std::map<std::string, int> wh { };
};
struct Order {

	void fill(WarehouseInterface& warehouse) {
		if (warehouse.hasInventory(what, howmany)){
			warehouse.remove(what,howmany);
			setFilled();
		}
	}

	bool isFilled() const {
		return filled;
	}

	Order(std::string const & whisky, int i)
	:what{whisky},howmany{i}{
	}
private:
	void setFilled() {
		filled=true;
	}
	bool filled{};
std::string const what;
int const howmany;
};
struct EmptyWarehouse :WarehouseInterface{
	void add(std::string const & s, int i) {
	}

	int getInventory(std::string const & s) const {
		return int();
	}

	bool hasInventory(std::string const & s, int i) const {
		return bool();
	}

	void remove(std::string const & s, int i) {
	}
};

void OrderFillFromWarehouse(){
	Order order(TALISKER,50);
	EmptyWarehouse warehouse{};
	order.fill(warehouse);
	ASSERT(not order.isFilled());
}


void thisIsAOOMockTestsTest() {
	ASSERTM("start writing tests", false);	
}

cute::suite make_suite_OOMockTests() {
	cute::suite s { };
	s.push_back(CUTE(thisIsAOOMockTestsTest));
	s.push_back(CUTE(OrderFillFromWarehouse));
	return s;
}
