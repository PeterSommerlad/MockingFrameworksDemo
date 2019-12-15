#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <map>

#include "mockator.h"
#include "OOMockTests.h"
namespace {
using namespace std::string_literals;

const auto TALISKER = "Talisker"s;
}
namespace TDD {
struct Warehouse {
	int getInventory(std::string const& s) const {
		auto const it = wh.find(s);
		return (it != wh.end())?it->second:0;
	}

	void remove(std::string const & s, int i) {
		wh[s]-=i;
	}

	void add(std::string const & s, int i) {
		wh[s]+=i;
	}

	bool hasInventory(const std::string& s, int i) const {
		return getInventory(s)>=i;
	}
	std::map<std::string,int> wh{};
};

/// TDD Warehouse
void WarehouseHasInventory(){
	Warehouse const warehouse { };
	ASSERT(not warehouse.hasInventory(TALISKER,50));
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
}


namespace CT {
struct Warehouse {
	int getInventory(std::string const& s) const {
		auto const it = wh.find(s);
		return (it != wh.end())?it->second:0;
	}

	void remove(std::string const & s, int i) {
		wh[s]-=i;
	}

	void add(std::string const & s, int i) {
		wh[s]+=i;
	}

	bool hasInventory(const std::string& s, int i) const {
		return getInventory(s)>=i;
	}
	std::map<std::string,int> wh{};
};

/// TDD Warehouse
void WarehouseHasInventory(){
	Warehouse const warehouse { };
	ASSERT(not warehouse.hasInventory(TALISKER,50));
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
struct Mailer{
	virtual void send(std::string const &)=0;
	virtual int numberOfSentMessages() const =0;
};
///TDD Order
template<typename T1 = Warehouse> struct OrderT {
	void fill(T1& warehouse) {
		if (warehouse.hasInventory(what, howmany)) {
			warehouse.remove(what, howmany);
			setFilled();
		} else if (theMailer){
			theMailer->send("please order: "+what);
		}
	}

	bool isFilled() const {
		return filled;
	}

	OrderT(std::string const & whisky, int i) :	what { whisky }, howmany { i } {	}
	void setMailer(Mailer *mailer){
		theMailer=mailer;
	}
private:
	void setFilled() {
		filled = true;
	}

	std::string const what;
	int const howmany;
	bool filled {};
	Mailer *theMailer{};
};
using Order = OrderT<>;

void OrderCtor(){
	Order order(TALISKER,50);
	ASSERT(not order.isFilled());
}



void OrderFillFromWarehouse(){
	INIT_MOCKATOR();
	static std::vector<calls> allCalls(1);
	struct MockWarehouse {
		size_t const mock_id;

		MockWarehouse()
		:mock_id(reserveNextCallId(allCalls))
		{
			allCalls[mock_id].push_back(call("MockWarehouse()"));
		}

		bool hasInventory(std::string const & what, int howmany) const {
			allCalls[mock_id].push_back(call("hasInventory(const std::string&, int) const", what, howmany));
			return true; // non-empty
		}

		void remove(std::string const & what, int howmany) const {
			allCalls[mock_id].push_back(call("remove(const std::string&, int) const", what, howmany));
		}
	};
	MockWarehouse warehouse { };
	OrderT<MockWarehouse> order(TALISKER,50);
	order.fill(warehouse);
	ASSERT( order.isFilled());
	calls expectedMockWarehouse{
		call("MockWarehouse()"),
		call("hasInventory(const std::string&, int) const", TALISKER,50),
		call("remove(const std::string&, int) const", TALISKER,50)
	};
	ASSERT_EQUAL(expectedMockWarehouse, allCalls[1]);
}

struct MockWarehouse {
	bool hasInventory(std::string const & what, int howmany) const {
		return bool();
	}

	void remove(std::string const & what, int howmany) const {
	}
};
struct MailServiceStub : Mailer {
	void send(std::string const &) {
		++numberOfMessages;
	}

	int numberOfSentMessages() const {
		return numberOfMessages;
	}
	int numberOfMessages{};
};

void UnfulfilledOrderSendsEmailIfMailerIsSet(){
	MockWarehouse wh{};
	MailServiceStub mailer{};
	OrderT<MockWarehouse> order(TALISKER,51);
	order.setMailer(&mailer);
	order.fill(wh);
	ASSERT_EQUAL(1,mailer.numberOfSentMessages());
}



}



bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	{ using namespace TDD;
	s.push_back(CUTE(WarehouseHasInventory));
	s.push_back(CUTE(WarehouseRemove));
	s.push_back(CUTE(WarehouseAdd));
	s.push_back(CUTE(WarehouseGetInventory));
	s.push_back(CUTE(OrderCtor));
	s.push_back(CUTE(OrderFillFromWarehouse));
	}
	{ using namespace CT;
	s.push_back(CUTE(WarehouseHasInventory));
	s.push_back(CUTE(WarehouseRemove));
	s.push_back(CUTE(WarehouseAdd));
	s.push_back(CUTE(WarehouseGetInventory));
	s.push_back(CUTE(OrderCtor));
	s.push_back(CUTE(OrderFillFromWarehouse));
	s.push_back(CUTE(UnfulfilledOrderSendsEmailIfMailerIsSet));
	}
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");	cute::suite OOMockTests = make_suite_OOMockTests();
	success &= runner(OOMockTests, "OOMockTests");

	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
