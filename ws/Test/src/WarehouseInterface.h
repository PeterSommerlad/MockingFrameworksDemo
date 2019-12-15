#ifndef SRC_WAREHOUSEINTERFACE_H_
#define SRC_WAREHOUSEINTERFACE_H_
#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"
#include <map>
struct WarehouseInterface {
	virtual ~WarehouseInterface() {
	}

	virtual int getInventory(std::string const & s) const = 0;
	virtual void remove(std::string const & s, int i) = 0;
	virtual void add(std::string const & s, int i) = 0;
	virtual bool hasInventory(std::string const & s, int i) const = 0;
};
#endif
