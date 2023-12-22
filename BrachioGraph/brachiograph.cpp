#include <iostream>
#include <cmath>
#include "plotter.cpp"


class BrachioGraph : public Plotter{

private:
    bool virtualPlotter;
    bool turtle;
    double turtle_coarseness;
    std::array<int, 4> bounds;
    double inner_arm = 8;
    double outer_arm = 8;
    double servo_1_parked_pw;
    double servo_2_parked_pw;
    double servo_1_degree_ms;
    double servo_2_degree_ms;
    double servo_1_parked_angle;
    double servo_2_parked_angle;
    double hysteresis_correction_1;
    double hysteresis_correction_2;
    std::vector<std::pair<double,double>> servo_1_angle_pws;
    std::vector<std::pair<double,double>> servo_2_angle_pws;
    std::map<double, std::map<std::string, double>> servo_1_angle_pws_bidi;
    std::map<double, std::map<std::string, double>> servo_2_angle_pws_bidi;
    double pw_up;
    double pw_down;
    double angular_step;
    double wait;
    double resolution;
    double x;
    double y;



public:

    BrachioGraph(
        bool virtualPlotter = false,
        bool turtle = false,
        double turtle_coarseness = 9999,
        std::array<int, 4> bounds = {-8, 4, 6, 13},
        double inner_arm = 8,
        double outer_arm = 8,
        double servo_1_parked_pw = 1500,
        double servo_2_parked_pw = 1500,
        double servo_1_degree_ms = -10,
        double servo_2_degree_ms = 10,
        double servo_1_parked_angle = -90,
        double servo_2_parked_angle = 90,
        double hysteresis_correction_1 = 0,
        double hysteresis_correction_2 = 0,
        std::vector<std::pair<double,double>> servo_1_angle_pws = std::vector<std::pair<double,double>>(),
        std::vector<std::pair<double,double>> servo_2_angle_pws = std::vector<std::pair<double,double>>(),
        std::map<double, std::map<std::string, double>> servo_1_angle_pws_bidi = std::map<double, std::map<std::string, double>>(),
        std::map<double, std::map<std::string, double>> servo_2_angle_pws_bidi = std::map<double, std::map<std::string, double>>(),
        double pw_up = 1100,
        double pw_down = 1500,
        double angular_step = 9999,
        double wait = 9999,
        double resolution = 9999
    ): Plotter(
        virtualPlotter,
        turtle,
        turtle_coarseness,
        bounds,
        servo_1_parked_pw,
        servo_2_parked_pw,
        servo_1_degree_ms,
        servo_2_degree_ms,
        servo_1_parked_angle,
        servo_2_parked_angle,
        hysteresis_correction_1,
        hysteresis_correction_2,
        servo_1_angle_pws,
        servo_2_angle_pws,
        servo_1_angle_pws_bidi,
        servo_2_angle_pws_bidi,
        pw_up,
        pw_down,
        angular_step,
        wait,
        resolution
    )
    {
        this->inner_arm = inner_arm;
        this->outer_arm = outer_arm;

        this->x = -1 * this->inner_arm;
        this->y = this->outer_arm;
    }

    void setup_turtle(double coarseness){
        //IMPLEMENT
    }

    double degrees(double radians) {
        return radians * (180.0 / M_PI);
    }

    double radians(double degrees) {
    return degrees * (M_PI / 180.0);
}

    void test_arcs(){
        this->park();
        double elbow_angle = 120;
        this->move_angles(9999,elbow_angle);

        for(int angle_1 = -135; angle_1 < 15; angle_1 += 15){
            this->move_angles(angle_1,9999,9999,9999,true);

            for(int angle_2 = elbow_angle; angle_2 < elbow_angle + 16; angle_2 += 1){
                this->move_angles(9999,angle_2,9999,9999,true);
            }
            for(int angle_2 = elbow_angle+16; angle_2 > elbow_angle - 16; angle_2 -= 1){
                this->move_angles(9999,angle_2,9999,9999,true);
            }
            for(int angle_2 = elbow_angle - 16; angle_2 < elbow_angle + 1; angle_2 += 1){
                this->move_angles(9999,angle_2,9999,9999,true);
            }
        }
    }

    //Returns the servo angles required to reacy any x/y position.
    std::pair<double,double> xy_to_angles(double x = 0, double y = 0) override{
        std::cout << "INSIDE BRACHIOGRAPH XY_TO_ANGLES" << std::endl;
        double hypotenuse = std::sqrt(x * x + y * y);

        if(hypotenuse > this->inner_arm + this->outer_arm){
            throw std::runtime_error("Cannot reach " + std::to_string(hypotenuse) +
                            "; total arm length is " + std::to_string(this->inner_arm + this->outer_arm));
        }

        double hypotenuse_angle = std::asin(x / hypotenuse);

        double inner_angle = std::acos(
            (hypotenuse * hypotenuse + this->inner_arm * this->inner_arm - this->outer_arm * this->outer_arm) 
            / (2 * hypotenuse * this->inner_arm)
        );

        double outer_angle = std::acos(
            (this->inner_arm * this->inner_arm + this->outer_arm * this->outer_arm - hypotenuse * hypotenuse) 
            / (2 * this->inner_arm * this->outer_arm)
        );

        double shoulder_motor_angle = hypotenuse_angle - inner_angle;
        double elbow_motor_angle = M_PI - outer_angle;
        return std::make_pair(degrees(shoulder_motor_angle),degrees(elbow_motor_angle));
    }       
    
    //Returns the x/y co-ordinates represented by a pair of servo angles.
    std::pair<double,double> angles_to_xy(double shoulder_motor_angle, double elbow_motor_angle) override{
        std::cout << "INSIDE BRACHIOGRAPH ANGLES_TO_XY" << std::endl;
        elbow_motor_angle = radians(elbow_motor_angle);
        shoulder_motor_angle = radians(shoulder_motor_angle); 

        double hypotenuse = std::sqrt(
            (this->inner_arm * this->inner_arm + this->outer_arm * this->outer_arm
            - 2 * this->inner_arm * this->outer_arm * std::cos(M_PI - elbow_motor_angle))
        );

        double base_angle = std::acos(
            (hypotenuse * hypotenuse + this->inner_arm * this->inner_arm - this->outer_arm * this->outer_arm)
            / (2 * hypotenuse * this->inner_arm)
        );
        
        double inner_angle = base_angle + shoulder_motor_angle;
        double x = std::sin(inner_angle) * hypotenuse;
        double y = std::cos(inner_angle) * hypotenuse;
        return std::make_pair(x,y);
    }

    void report(){

        std::cout << "               -----------------|-----------------" << std::endl;
        std::cout << "               Servo 1          |  Servo 2        " << std::endl;
        std::cout << "               -----------------|-----------------" << std::endl;

        double h1 = this->hysteresis_correction_1;
        double h2 = this->hysteresis_correction_2;
        std::cout << "hysteresis                 " << std::setw(4) << std::fixed << std::setprecision(1) << h1 << "  |              " << std::setw(4) << std::fixed << std::setprecision(1) << h2 << std::endl;
        std::pair<double,double> pws = this->get_pulse_widths();
        double pw_1 = pws.first;
        double pw_2 = pws.second;

        std::cout << "pulse-width               " << std::setw(4) << pw_1 << "  |             " << std::setw(4) << pw_2 << std::endl;

        double angle_1 = this->get_angle_1();
        double angle_2 = this->get_angle_2();

        if(angle_1 != 9999 && angle_2 != 9999){
            std::cout << "      angle               " << std::setw(4) << angle_1 << "  |             " << std::setw(4) << angle_2 << std::endl;
        }

        std::cout << "               -----------------|-----------------" << std::endl;
        std::cout << "               min   max   mid  |  min   max   mid" << std::endl;
        std::cout << "               -----------------|-----------------" << std::endl;

        if(!this->get_angle_used_1().empty() && !this->get_angle_used_2().empty()
        && !this->get_pulse_widths_used_1().empty() && !this->get_pulse_widths_used_2().empty()){
            double min1 = *std::min_element(this->get_pulse_widths_used_1().begin(),this->get_pulse_widths_used_1().end());
            double max1 = *std::max_element(this->get_pulse_widths_used_1().begin(),this->get_pulse_widths_used_1().end());
            double mid1 = (min1 + max1) / 2;
            double min2 = *std::min_element(this->get_pulse_widths_used_2().begin(), this->get_pulse_widths_used_2().end());
            double max2 = *std::max_element(this->get_pulse_widths_used_2().begin(), this->get_pulse_widths_used_2().end());
            double mid2 = (min2 + max2) / 2;

            std::cout << "pulse-widths  " << std::setw(4) << min1 << "  " << std::setw(4) << max1 << "  " << std::setw(4) << mid1 << "  | " << std::setw(4) << min2 << "  " << std::setw(4) << max2 << "  " << std::setw(4) << mid2 << std::endl;

            min1 = *std::min_element(this->get_angle_used_1().begin(), this->get_angle_used_1().end());
            max1 = *std::max_element(this->get_angle_used_1().begin(), this->get_angle_used_1().end());
            mid1 = (min1 + max1) / 2;
            min2 = *std::min_element(this->get_angle_used_2().begin(), this->get_angle_used_2().end());
            max2 = *std::max_element(this->get_angle_used_2().begin(), this->get_angle_used_2().end());
            mid2 = (min2 + max2) / 2;

            std::cout << "      angles  " << std::setw(4) << min1 << "  " << std::setw(4) << max1 << "  " << std::setw(4) << mid1 << "  | " << std::setw(4) << min2 << "  " << std::setw(4) << max2 << "  " << std::setw(4) << mid2 << std::endl;
        }
        else {
            std::cout << "No data recorded yet. Try calling the BrachioGraph.box() method first." << std::endl;
        }
    
    }

};