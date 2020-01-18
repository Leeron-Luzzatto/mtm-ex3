#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include <vector>
#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "Vehicle.h"
#include "UniqueArray.h"

namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;
    using std::vector;
    const int fine_price=250;

    class ParkingLot {
        vector<UniqueArray<Vehicle> > blocks;
        bool isEmptyBlock4Vehicle(const VehicleType& type, VehicleType& block) const;
        ParkingResult getVehicle(const ParkingSpot& spot, Vehicle* result) const;
    public:

        explicit ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot()= default;
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);

    };

}

#endif //MTMPARKINGLOT_PARKINGLOT_H
