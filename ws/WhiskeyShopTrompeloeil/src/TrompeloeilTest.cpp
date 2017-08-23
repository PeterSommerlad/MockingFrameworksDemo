#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include "trompeloeil.hpp"

namespace cute {
	template <typename Listener=null_listener>
	struct ide_listener_trompeloeil: Listener
	{
		ide_listener_trompeloeil(std::ostream &os=std::cout):out(os) {}
		void begin(suite const &t,char const *info, size_t n_of_tests){
			out << std::dec << "\n#beginning " << info << " " << n_of_tests << '\n';
			Listener::begin(t,info,n_of_tests);
		}
		void end(suite const &t, char const *info){
			out << "\n#ending " << info << std::endl;
			Listener::end(t,info);
		}
		void start(test const &t){
			current=&t;
			out << "\n#starting " << t.name()<<'\n';
			Listener::start(t);
		}
		void success(test const &t, char const *msg){
			if (current == &t)
				out << "\n#success " <<  maskBlanks(t.name()) <<" " << msg<< '\n';
			Listener::success(t,msg);
			current = nullptr;
		}
		void failure(test const &t,test_failure const &e){
			auto const name= (current && current != &t)?current->name():t.name(); // for Trompeloeil
			out << std::dec <<  "\n#failure " << maskBlanks(name) << " " << e.filename << ":" << e.lineno << " " << (e.reason) << '\n';
			Listener::failure(t,e);
#ifdef _MSV_VER
			std::ostringstream os;
			os << std::dec << e.filename << "(" << e.lineno << ") : failure: " <<e.reason << " in " << name << std::endl;
			OutputDebugString(os.str().c_str());
#endif
			current = nullptr;
		}
		void error(test const &t, char const *what){
			out << "\n#error " << maskBlanks(t.name()) << " " << what <<'\n';
			Listener::error(t,what);
#ifdef _MSV_VER
			std::ostringstream os;
			os << what << " error in " << t.name() << std::endl;
			OutputDebugString(os.str().c_str());
#endif
			current = nullptr;
		}
		static std::string maskBlanks(std::string in) {
			std::transform(in.begin(),in.end(),in.begin(),blankToUnderscore());
			return in;
		}
	protected:
		struct blankToUnderscore{
            char operator()(char in) const {
			if (in == ' ') return '_';
			return in;
		}
        };
		test const *current=nullptr;
		std::ostream &out;
	};
}


namespace {
using namespace std::string_literals;

const auto TALISKER = "Talisker"s;
}


namespace OO{
struct Warehouse {
	virtual ~Warehouse()=default;
	virtual int getInventory(std::string const& s) const {
		return int();
	}

	virtual void remove(std::string const & s, int i) {
	}

	virtual void add(std::string const & s, int i) {
	}

	virtual bool hasInventory(const std::string& s, int i) const {
		return bool();
	}
};

/// TDD Warehouse
void WarehouseHasInventory(){
	Warehouse warehouse{};
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
		 warehouse.hasInventory(w, howmany);
	}

	bool isFilled() const {
		return bool();
	}

	Order(std::string const & whisky, int i) :w{whisky},howmany{i}{
	}
private:
	std::string w;
	int howmany;
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

struct MockWarehouse:Warehouse{
	MAKE_CONST_MOCK2(hasInventory,bool(std::string const &,int),override);
};

void testMockingWithTrompeloeil(){
	MockWarehouse wh;
	REQUIRE_CALL(wh,hasInventory(TALISKER,50)).RETURN(false).TIMES(2);
	Order order(TALISKER,50);
	order.fill(wh);
	ASSERT(not order.isFilled());
}
}

cute::test_failure prepare_trompeloeil_failure(char const * const file, unsigned long line, std::string msg) {
	std::string f = file;
	if (""s == f && 0 == line) {
		// search for at in msg and convert that
		auto pos = msg.find(" at ");
		if (pos != std::string::npos) {
			pos += 4;
			auto numpos = msg.find(':', pos);
			if (numpos != std::string::npos) {
				line = std::stoull(msg.substr(numpos + 1));
			}
			f = msg.substr(pos, numpos - pos);
		}
	}
	transform(msg.begin(),msg.end(),msg.begin(),[](auto c){if (c=='\n') return ' ';return c;});
	return { msg, f.c_str(), int(line) };
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(OO::WarehouseHasInventory));
	s.push_back(CUTE(OO::WarehouseRemove));
	s.push_back(CUTE(OO::WarehouseAdd));
	s.push_back(CUTE(OO::WarehouseGetInventory));
	s.push_back(CUTE(OO::testMockingWithTrompeloeil));
	s.push_back(CUTE(OO::OrderCtor));
	s.push_back(CUTE(OO::OrderFillFromWarehouse));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener_trompeloeil<>> lis(xmlfile.out);

	trompeloeil::set_reporter([&lis](
	    trompeloeil::severity s,
		const char*const file,
		unsigned long const line,
	    std::string const& msg)
	  {
		auto fail=prepare_trompeloeil_failure(file, line, msg);
	    if (s == trompeloeil::severity::fatal ) // can not throw when non_fatal, because already in stack-unwinding mode
	    {
	    		throw fail;
	    }
	    else
	    {
	    		cute::test trompeloeil{[]{},"Trompeloeil"}; // dummy test object for listener
	    		lis.failure(trompeloeil,fail); // hack listener so it parses from IDE, XML is fine?
	    }
	  });

	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
