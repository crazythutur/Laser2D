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
   const unsigned int mode_number =2;
   const unsigned int lasing_level[2]={50,51};
   Electron_presence::initialize_size(mode_number,100);
   double q1=0.962090;
   double q2=0.97;
   double q3=0.98;
   //Electron_presence occ (lasing_level,q1);

   cout << "affichage occupancies"  << endl;
   //cout << occ;
   //cout << *occ2;

   map<double,Electron_presence*> occ_map;
   //occ_map[q1]= &occ;
   double J=1.2;


   cout << "making of the 2 QW with differents tempeture"  << endl;
   QW_elementary_laser *las1=new QW_elementary_laser(J,q1,lasing_level,occ_map);

   cout << "caca" << endl;

   cout << "this should appear " << las1->getTemperature()  << endl;

   QW_elementary_laser *las2=new QW_elementary_laser(J,q2,lasing_level,occ_map);
   cout << "infos las2 " << las2->getTemperature()  << endl;
   cout << "coco" <<endl;
   Electron_presence *electron_presence = las2->getElectron_presence();
   cout << *electron_presence;
   /** @todo DEBUG that**/
   cout << las1->getElectron_presence()->getLasing_lvl_prob(97,1) << endl;
   cout << electron_presence->getLasing_lvl_prob(97,2) << endl;

   cout << "QW testing" <<endl;
   QW_elementary_laser *qw1 = new QW_elementary_laser(J, q1,lasing_level, occ_map);
   QW_elementary_laser *qw2 = new QW_elementary_laser(J, q2,lasing_level, occ_map);
   QW_elementary_laser *qw3 = new QW_elementary_laser(J, q3,lasing_level, occ_map);


   cout << "electron presence1 " << qw1->getElectron_presence()->getLasing_lvl_prob(97,1) << endl;
   cout << "Jloc " <<qw1->getPumping_local_rate() <<  " q " << qw1->getTemperature() <<endl;
   cout << "electron presence2 " << qw2->getElectron_presence()->getLasing_lvl_prob(97,1) << endl;
   cout << "electron presence3 " << qw3->getElectron_presence()->getLasing_lvl_prob(97,1) << endl;
   cout << occ_map.size() << endl;



   occ_map.clear();
   delete las1;
   delete las2;
   delete qw1;
   delete qw2;
   //delete occ2;


   return 0;
}
