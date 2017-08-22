#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

namespace {
using namespace std::string_literals;

const auto TALISKER = "Talisker"s;
}

struct Warehouse {
	int getInventory(std::string const& s) const {
		return int();
	}

	void remove(std::string const & s, int i) {
	}

	void add(std::string const & s, int i) {
	}

	bool hasInventory(const std::string& s, int i) const {
		return bool();
	}
};

/// TDD Warehouse
void WarehouseHasInventory(){
	Warehouse warehouse{};
	ASSERT(not warehouse.hasInventory("Talisker"s,50));
}
void WarehouseAdd(){
	Warehouse warehouse{};
	warehouse.add(TALISKER,50);
	ASSERT( warehouse.hasInventory(TALISKER,50));
}


void WarehouseRemove(){
	Warehouse warehouse{};
	warehouse.add(TALISKER,50);
	warehouse.remove(TALISKER,50);
	ASSERT(not warehouse.hasInventory(TALISKER,50));
}

void WarehouseGetInventory(){
	Warehouse warehouse{};
	warehouse.add(TALISKER,50);
	ASSERT_EQUAL(50,warehouse.getInventory(TALISKER));
}
///TDD Order
struct Order {
	void fill(Warehouse& warehouse) {
	}

	bool isFilled() const {
		return bool();
	}

	Order(std::string const & whisky, int i) {
	}
};

void OrderCtor(){
	Order order(TALISKER,50);
	ASSERT(not order.isFilled());
}
void OrderFillFromWarehouse(){
	Order order(TALISKER,50);
	Warehouse warehouse{};
	order.fill(warehouse);
	ASSERT(not order.isFilled());
}




////
void thisIsATest() {
	ASSERTM("start writing tests", false);	
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	s.push_back(CUTE(WarehouseHasInventory));
	s.push_back(CUTE(WarehouseRemove));
	s.push_back(CUTE(WarehouseAdd));
	s.push_back(CUTE(WarehouseGetInventory));
	s.push_back(CUTE(OrderCtor));
	s.push_back(CUTE(OrderFillFromWarehouse));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
