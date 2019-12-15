#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace {
using namespace std::string_literals;

[[gsl::suppress("Res-not-CAPS")]] auto const TALISKER = "Talisker"s;
}



struct Warehouse {

	virtual ~Warehouse() noexcept =default;

	virtual int getInventory(std::string const & s) const =0;

	virtual void remove(std::string const & s, int i) =0;

	virtual void add(std::string const & s, int i) =0;

	virtual bool hasInventory(std::string const & s, int i) const =0;
};
struct Order {
	Order(std::string const & whisky, int i)
	:what{whisky},howmany{i}{
	}
	void fill(Warehouse& warehouse) {
		if (warehouse.hasInventory(what, howmany)){
			warehouse.remove(what,howmany);
			setFilled();
		}
	}

	bool isFilled() const {
		return filled;
	}

private:
	void setFilled() {
		filled=true;
	}
std::string const what;
int const howmany;
	bool filled{};
};

struct MockWarehouse: public Warehouse {
public:
	MOCK_CONST_METHOD1(getInventory, int(std::string const&));
	MOCK_METHOD2(remove, void(std::string const&,int));
	MOCK_METHOD2(add, void(std::string const&,int));
	MOCK_CONST_METHOD2(hasInventory, bool(std::string const&,int));
};
using namespace ::testing;

TEST(OrderTest, EmptyWarehouse)
{
	MockWarehouse warehouse{};
	Order order{TALISKER,50};
	EXPECT_CALL(warehouse,hasInventory(TALISKER,50)).WillOnce(Return(false));
	//ON_CALL(warehouse,hasInventory(TALISKER,50)).WillByDefault(Return(false));
	order.fill(warehouse);
	ASSERT_FALSE(order.isFilled());
}

TEST(OrderTest, FilledWarehouse)
{
	MockWarehouse warehouse{};
	InSequence s{};
	EXPECT_CALL(warehouse,hasInventory(TALISKER,50)).WillOnce(Return(true));
	EXPECT_CALL(warehouse,remove(TALISKER,50));
	Order order{TALISKER,50};
	order.fill(warehouse);
	ASSERT_TRUE(order.isFilled());
}

int main(int argc, char** argv) {
  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}



