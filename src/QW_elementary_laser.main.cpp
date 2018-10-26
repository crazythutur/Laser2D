#include "../inc/Elementary_laser.h"
#include "../inc/Electron_presence.h"
#include "../inc/QW_elementary_laser.h"
#include <string>
#include <map>


using namespace std;


/**
 * @brief main Testing software for occupancies and QW lasers
 * @return
 */
int main()
{
   const unsigned int lasing_level[2]={399,401};
   Electron_presence::initialize_size(2,800);
   double q1=0.962090;
   double q2=0.97;
   Electron_presence occ (lasing_level,q1);
   Electron_presence *occ2 = new Electron_presence(lasing_level,q1);
   cout << "affichage occupancies"  << endl;
   //cout << occ;
   //cout << *occ2;

   map<double,Electron_presence*> occ_map;
   //occ_map[q1]= &occ;
   double J=1.2,T1=2.2,T2=2.9;


   cout << "making of the 2 QW with differents tempeture"  << endl;
   QW_elementary_laser *las1=new QW_elementary_laser(J,T1,q1,lasing_level,occ_map);

   cout << "caca" << endl;

   cout << "this should appear " << las1->getExponential_temperature()  << endl;

   QW_elementary_laser *las2=new QW_elementary_laser(J,T2,q2,lasing_level,occ_map);
   cout << "infos las2 " << las2->getExponential_temperature()  << endl;
   Electron_presence electron_presence = las2->getElectron_presence();
   cout << electron_presence;

   /** @todo DEBUG that**/
   //cout << las2->getElectron_presence();

   cout << electron_presence.getProbability()[795][Electron_presence::PUMPING_LEVEL] << endl;






   delete las1;
   delete las2;
   delete occ2;


   return 0;
}
