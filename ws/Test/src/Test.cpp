#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <map>

namespace {
using namespace std::string_literals;

const auto TALISKER = "Talisker"s;
}
struct WH1 {
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

	void fill(WH1& warehouse) {
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

void thisIsATest() {
	ASSERTM("start writing tests", false);	
}

bool runAllTests(int argc, char const *argv[]) {
	cute::suite s { };
	//TODO add your test here
	s.push_back(CUTE(thisIsATest));
	cute::xml_file_opener xmlfile(argc, argv);
	cute::xml_listener<cute::ide_listener<>> lis(xmlfile.out);
	auto runner = cute::makeRunner(lis, argc, argv);
	bool success = runner(s, "AllTests");
	return success;
}

int main(int argc, char const *argv[]) {
    return runAllTests(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
