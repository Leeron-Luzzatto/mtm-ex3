#include "Vehicle.h"
#include <iostream>
#include "ParkingLotPrinter.h"

using std::ostream;
using ParkingLotUtils::ParkingLotPrinter;

namespace MtmParkingLot {
	unsigned int Vehicle::getPrice(Time exit, unsigned int fine_price) const {
		Time::Hour parking_duration = (exit - entrance).toHours();
		if (parking_duration > max_time.toHours()) {
			parking_duration = max_time.toHours();
		}
		unsigned int price = 0;
		for (Time::Hour i = 0; i < parking_duration; i++) {
			(i == 0) ? price += price_for_first : price += price_for_extra;
		}
		return price + fine_price * fine;
	}

	ostream& operator<<(ostream& os, const Vehicle& vehicle) {
		return ParkingLotPrinter::printVehicle(os,vehicle.getType(),
			vehicle.licensePlate, vehicle.entrance);
		}
}