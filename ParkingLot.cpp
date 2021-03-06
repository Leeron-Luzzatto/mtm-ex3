#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
#include "UniqueArray.h"
#include "Vehicle.h"
#include "Car.h"
#include "Motorbike.h"
#include "Handicapped.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <assert.h>

namespace MtmParkingLot{

    using std::cout;
    using std::endl;
    using std::sort;

    const int vehicle_types=3;

    ParkingLot::ParkingLot(unsigned int *parkingBlockSizes):
    blocks() {
        for(int i=0; i<vehicle_types; i++)
        blocks.push_back((UniqueArray<Vehicle>(parkingBlockSizes[i])));
    }

    Vehicle* ParkingLot::createVehicle(VehicleType vehicleType, const LicensePlate& licensePlate,
            const Time& entranceTime){
        if(vehicleType==CAR) return new Car(licensePlate, entranceTime);
        if(vehicleType==MOTORBIKE) return new Motorbike(licensePlate, entranceTime);
        return new Handicapped(licensePlate, entranceTime);
    }

    ParkingResult ParkingLot::getVehicle(const ParkingSpot &spot, UniqueArray<Vehicle>::iterator& result) {
        VehicleType block=spot.getParkingBlock();
        unsigned index=spot.getParkingNumber();
        for(UniqueArray<Vehicle>::iterator i=blocks[block].begin(); i!=blocks[block].end(); ++i) {
            if ((*i).getSpot().getParkingNumber() == index) {
                result = i;
                return SUCCESS;
            }
        }
        return VEHICLE_NOT_FOUND;
    }

    ParkingResult ParkingLot::getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const{
        VehicleType block;
        for(int b=0; b<vehicle_types; b++){
            block=(VehicleType)b;
            for(UniqueArray<Vehicle>::const_iterator i= blocks[block].const_begin(); i!=blocks[block].const_end(); ++i)
                if((*i).getLicensePlate()==licensePlate){

                    parkingSpot = (*i).getSpot();
                    return SUCCESS;
                }
        }
        return VEHICLE_NOT_FOUND;
    }

    bool ParkingLot::isEmptyBlock4Vehicle(const VehicleType& type, VehicleType &block) const{
        if(blocks[type].getSize()>blocks[type].getCount()){
            block=type;
            return true;
        }
        else if(type==HANDICAPPED && blocks[CAR].getSize()>blocks[CAR].getCount()){
            block=CAR;
            return true;
        }
        return false;
    }

    void ParkingLot::insertVehicleToBlock(VehicleType vehicleType, LicensePlate licensePlate,
                              Time entranceTime, VehicleType block){
        Vehicle* new_v=createVehicle(vehicleType, licensePlate, entranceTime);
        cout<<*new_v;
        unsigned index=blocks[block].insert_by_reference(*new_v);
        ParkingSpot spot=ParkingSpot(block, index);
        (*new_v).setSpot(spot);
        ParkingLotPrinter::printEntrySuccess(cout, spot);
    }

    ParkingResult ParkingLot::enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime){
        ParkingSpot spot(MOTORBIKE); //temporary init
        if(getParkingSpot(licensePlate, spot)==SUCCESS){
            UniqueArray<Vehicle>::iterator found = blocks[(VehicleType)0].begin(); // init found
            getVehicle(spot, found);
            cout<<*found;
            ParkingLotPrinter::printEntryFailureAlreadyParked(cout, spot);
            return VEHICLE_ALREADY_PARKED;
        }
        VehicleType block=MOTORBIKE; //temporary init
        if(isEmptyBlock4Vehicle(vehicleType, block)){
            insertVehicleToBlock(vehicleType, licensePlate, entranceTime, block);
            return SUCCESS;
        }
        Vehicle* new_v=createVehicle(vehicleType, licensePlate, entranceTime);
        cout<<*new_v;
        delete(new_v);
        ParkingLotPrinter::printEntryFailureNoSpot(cout);
        return NO_EMPTY_SPOT;
    }

    ParkingResult ParkingLot::exitParking(LicensePlate licensePlate, Time exitTime){
        ParkingSpot spot(MOTORBIKE); //temporary init
        if(getParkingSpot(licensePlate, spot)==VEHICLE_NOT_FOUND){
            ParkingLotPrinter::printExitFailure(cout, licensePlate);
            return VEHICLE_NOT_FOUND;
        }
        UniqueArray<Vehicle>::iterator found = blocks[(VehicleType)0].begin(); // init i
        getVehicle(spot, found); // we've made sure the vehicle does exist.
        cout<<*found;
        ParkingLotPrinter::printExitSuccess(cout, spot, exitTime, (*found).getPrice(exitTime, fine_price));
        blocks[spot.getParkingBlock()].remove(*found);
        return SUCCESS;
    }

    void ParkingLot::inspectParkingLot(Time inspectionTime){
        VehicleType block;
        unsigned fines=0;
        for(int b=0; b<vehicle_types; b++){
            block=(VehicleType)b;
            for(UniqueArray<Vehicle>::iterator i= blocks[block].begin(); i!=blocks[block].end(); ++i)
                if((*i).fineVehicle(inspectionTime)){
                    cout<<*i<<endl;
                    fines++;
                }
        }
        ParkingLotPrinter::printInspectionResult(cout, inspectionTime, fines);
    }

    //function used for sorting vector of vehicles by their spot in ascending order.
    bool diffBySpot(const Vehicle* v1, const Vehicle* v2){
        return ((*v1).getSpot()<(*v2).getSpot());
    }

    ostream& operator<<(ostream& os, const ParkingLot& parkingLot) {
        vector<const Vehicle*> all_vehicles; //we'll put al vehicles into one single vector
        VehicleType block;
        for(int b=0; b<vehicle_types; b++){
            block=(VehicleType)b;
            for(UniqueArray<Vehicle>::const_iterator i= parkingLot.blocks[block].const_begin();
            i!=parkingLot.blocks[block].const_end(); ++i)
                all_vehicles.push_back(&(*i));
        }
        sort(all_vehicles.begin(), all_vehicles.end(), diffBySpot);
        ParkingLotPrinter::printParkingLotTitle(os);
        for(vector<const Vehicle*>::const_iterator i=all_vehicles.begin(); i!=all_vehicles.end(); i++){
            os<<*(*i);
            ParkingLotPrinter::printParkingSpot(os,(*i)->getSpot());
        }
        return os;
    }
}