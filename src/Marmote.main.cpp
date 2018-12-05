#include "../inc/Elementary_laser.h"
#include "../inc/Electron_presence.h"
#include "../inc/QW_elementary_laser.h"
#include "../inc/Laser_2D.h"
#include <string>
#include <map>


using namespace std;


/**
 * @brief main Testing software for occupancies and QW lasers
 * @return
 */
int main()
{
   const unsigned int laser_number=36;
   const unsigned int mode_number =2;
   const unsigned int lasing_level[2]={399,401};

    double cavity_escape_rates[2]={0.5,0.5};
    double local_pump[laser_number];
    double temperature[laser_number];
    std::fill_n(local_pump, laser_number, 10);
    std::fill_n(temperature, laser_number, 300);

   Electron_presence::initialize_size(mode_number,800);

   map<double,Electron_presence*> occ_map;

   Laser_2D::Laser_init_parameters parameters = {mode_number, laser_number, 1, 1, cavity_escape_rates,local_pump,temperature};
   Laser_2D *laser_2D= new Laser_2D(parameters,lasing_level,1,occ_map);

   cout << "mode number " << laser_2D->getMode_number()<<endl;
   occ_map.clear();
   delete laser_2D;

   return 0;
}
