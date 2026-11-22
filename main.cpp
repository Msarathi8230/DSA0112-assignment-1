#include <array>
#include <iostream>
#include <string>
enum class Zone { Inner, Middle, Outer, Airport };
constexpr std::array<long long,4> BASE={1000,2000,3000,6000};
class Pass {
    bool active;
    std::array<bool,4> covered;
public:
    Pass(bool a,int z1,int z2):active(a),covered{} {
        if(z1>=0&&z1<4) covered[z1]=true;
        if(z2>=0&&z2<4) covered[z2]=true;
    }
    bool covers(int z) const { return active&&z>=0&&z<4&&covered[z]; }
};
long long revisedFare(int zone,int hour,bool weekend,const Pass& pass) {
    if(zone<0||zone>=4||hour<0||hour>23) return -1;
    if(pass.covers(zone)) return 0;
    long long amount=BASE[zone];
    bool peak=(hour>=8&&hour<10)||(hour>=17&&hour<20);
    if(weekend) amount=amount*90/100; // weekend discount replaces peak surcharge
    else if(peak) amount=amount*120/100;
    return amount;
}
int main() {
    Pass monthly(true,0,2);
    std::cout<<"Airport weekday peak Rs "<<revisedFare(3,18,false,monthly)/100<<"\n";
    std::cout<<"Middle weekend Rs "<<revisedFare(1,18,true,monthly)/100<<"\n";
    std::cout<<"Inner monthly-pass fare Rs "<<revisedFare(0,12,false,monthly)/100<<"\n";
}