#include "WithNamespaceExtractInteface.h"
#include "cute.h"
#include <string>
#include <map>

#include "WarehouseInterface1.h"
namespace {
using namespace std::string_literals;

const auto TALISKER = "Talisker"s;
}
namespace OO{
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
}

void thisIsAWithNamespaceExtractIntefaceTest() {
	ASSERTM("start writing tests", false);	
}

cute::suite make_suite_WithNamespaceExtractInteface() {
	cute::suite s { };
	s.push_back(CUTE(thisIsAWithNamespaceExtractIntefaceTest));
	return s;
}
