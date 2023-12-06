#include <iostream>
#include "brachiograph.cpp"

std::vector<std::pair<double, double>> servo_1_angle_pws1 = {
    {-162, 2470},
    {-144, 2250},
    {-126, 2050},
    {-108, 1860},
    { -90, 1690},
    { -72, 1530},
    { -54, 1350},
    { -36, 1190},
    { -18, 1010},
    {   0,  840},
    {  18,  640}
};

std::vector<std::pair<double, double>> servo_2_angle_pws2 = {
    {0, 660},
    {18, 840},
    {36, 1030},
    {54, 1180},
    {72, 1340},
    {90, 1490},
    {108, 1640},
    {126, 1830},
    {144, 2000},
    {162,  2200},
    {180,  2410}
};

std::array<int, 4> bounds = {-8, 4, 8, 13};


BrachioGraph bg = new BrachioGraph(
    false,
    false,
    9999,
    bounds,
    8,
    8,
    1500,1500,-10,10,0,0,0,0,servo_1_angle_pws1,servo_2_angle_pws2,
    std::map<double, std::map<std::string, double>>(),std::map<double, 
    std::map<std::string, double>>(),1850,
    1200,9999,9999,9999);
    
 