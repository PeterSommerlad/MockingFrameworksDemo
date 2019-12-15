#ifndef SRC_WAREHOUSEINTERFACE1_H_
#define SRC_WAREHOUSEINTERFACE1_H_
#include "WithNamespaceExtractInteface.h"
#include "cute.h"
#include <string>
#include <map>
namespace OO {

struct WarehouseInterface {
	virtual ~WarehouseInterface() {
	}

	virtual int getInventory(std::string const & s) const = 0;
	virtual void remove(std::string const & s, int i) = 0;
	virtual void add(std::string const & s, int i) = 0;
	virtual bool hasInventory(std::string const & s, int i) const = 0;
};

}

#endif
