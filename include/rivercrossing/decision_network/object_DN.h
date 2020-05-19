#ifndef DN_OBJECT_H
#define DN_OBJECT_H

#include "network/network.h"
#include "tools/defines.h"

#include "rivercrossing/dsworld.h"
#include "rivercrossing/decision_network/base_DN.h"

class Object_DN: public base_DN
{
private:
 vector<unsigned> topology { 6,4,5 };
 vector<double> evolved_genome { -1.32564 , 0.502005 , -0.713916 , 0.886583 , -0.127856 , 1.09454 , 0.254552 , -0.00668612 , -1.31411 , 0.211547 , 0.108453 , 0.0104434 , -0.768644 , -0.467826 , 1.2694 , 0.0789407 , -0.210895 , -0.246784 , -0.590775 , -0.414117 , 1.43094 , 0.205156 , -1.09432 , 2.14164 , -1.27007 , -0.590775 , -1.80643 , 0.525009 , -0.0967102 , 1.2694 , -0.467826 , -0.768644 , 0.0104434 , 0.108453 , 0.779014 , -1.31411 , -0.00668612 , 0.254552 , 1.09454 , -0.127856 , 0.886583 , -0.713916 , -0.0905274 , -1.32564  };
 void build(vector<double> g);
public:
 void buildNetwork();
 void buildNetwork(vector<double> genome);

 void loadInputs(dsWorld &world, vector<double> &inputs, int object, bool _carrying);
 void convertOutputs();
};

#endif // DN_OBJECT_H
