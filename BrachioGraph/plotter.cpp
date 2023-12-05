#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <iomanip>
#include <xtensor/xtensor.hpp>
#include <xtensor/xarray.hpp>
#include <map>
#include <set>
#include <cmath>
//#include "utils.cpp"
#include <pigpio.h>

//namespace ct = cturtle;

class Pen{
    private:
        Plotter* bg;
        double pw_up;
        double pw_down;
        double virtual_pw;
        int pin;
        double transition_time;
        char position[10];
        bool virtualPlotter;
    public:
        Pen(Plotter* bg = nullptr,
            double pw_up = 1700,
            double pw_down = 1300,
            int pin = 18,
            double transition_time = 0.25,
            bool virtualPlotter = false
        ){
            this->bg = bg;
            this->pin = pin;
            this->pw_up = pw_up;
            this->pw_down = pw_down;
            this->transition_time = transition_time;
            strcpy(this->position, "down");
            this->virtualPlotter = virtualPlotter;

            if(this->virtualPlotter){
                std::cout << "Initialising virtual Pen" << std::endl;
            }
            else{
                gpioSetPWMfrequency(this->pin, 50);
            }

            this->up();
        
        
        }

        char* getPosition(){
            return this->position;
        }

        void down(){

            if(strcmp(this->position, "up") == 0){
                if(this->virtualPlotter){
                    this->virtual_pw = this->pw_down;
                }
                else{
                    this->ease_pen(this->pw_up, this->pw_down);
                }

                strcpy(this->position, "down");
            }

        }

        void up(){
            if(strcmp(this->position,"down") == 0){

                if(this->virtualPlotter){
                    this->virtual_pw = this->pw_up;
                }
                else{
                    this->ease_pen(this->pw_down, this->pw_up);
                }
                
                // if(this->bg->turtle){
                //     this->bg->turtle->up();
                // }
                strcpy(this->position, "up");
            }
        }

        void ease_pen(double start, double end){
            double diff = end - start;
            double angle = start;
            double length_of_step = diff / abs(diff);
            gpioSetMode(this->pin, PI_OUTPUT);
            for(int i = 0; i < abs(diff); i++){
                angle += length_of_step;
                gpioServo(this->pin, angle);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }

        void pw(double pulse_width){

            if(this->virtualPlotter){
                this->virtual_pw = pulse_width;
            }
            else{
                gpioSetMode(this->pin, PI_OUTPUT);
                gpioServo(this->pin, pulse_width);
            }
        }

        double get_pw(){
                if(this->virtualPlotter){
                    return this->virtual_pw;
                }
                else{
                    return gpioGetServoPulsewidth(this->pin);
                }
            }
};

class Plotter{

private:
    bool virtualPlotter;
    bool turtle;
    double turtle_coarseness;
    std::array<int, 4> bounds;
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
    std::chrono::steady_clock::time_point last_moved;
    double angle_1;
    double angle_2;
    double (*angles_to_pw_1)(double);
    double (*angles_to_pw_2)(double);
    double previous_pw_1;
    double previous_pw_2;
    double active_hysteresis_correction_1;
    double active_hysteresis_correction_2;
    std::set<double> angles_used_1;
    std::set<double> angles_used_2;
    std::set<double> pulse_widths_used_1;
    std::set<double> pulse_widths_used_2;
    double virtual_pw_1;
    double virtual_pw_2;
    double x;
    double y;
    Pen* pen;

public:

    //Silebilirsin
    static double static_angles_to_pw_1_wrapper(Plotter* plotter, double angle){
        return plotter->naive_angles_to_pulse_widths_1(angle);
    }

    double naive_angles_to_pulse_widths_1(double angle){
        return (angle - this->servo_1_parked_angle) * this->servo_1_degree_ms + this->servo_1_parked_pw;
    }

    double naive_angles_to_pulse_widths_2(double angle){
        return (angle - this->servo_2_parked_angle) * this->servo_2_degree_ms + this->servo_2_parked_pw;
    }

    void reset_report(){
        this->angle_1 = this->angle_2 = 9999; // 9999 = None in Python
    }

    void virtualise(){
        std::cout << "Initialising virtual BrachioGraph" << std::endl;
        this->virtual_pw_1 = this->angles_to_pw_1(-90);
        this->virtual_pw_2 = this->angles_to_pw_2(90);
        this->virtualPlotter = true;
    }
    
    /*Return the servo angles required to reach any x/y position. This is a dummy method in
        the base class; it needs to be overridden in a sub-class implementation.*/
    std::pair<double,double> xy_to_angles(double x = 0, double y = 0){
        return std::make_pair(0,0);
    }
        /*Return the servo angles required to reach any x/y position. This is a dummy method in
        the base class; it needs to be overridden in a sub-class implementation.*/

    std::pair<double,double> angles_to_xy(double angle_1, double angle_2){
        return std::make_pair(0,0);
    }

    void set_pulse_widths(double pw_1=9999, double pw_2=9999){

        if(this->virtualPlotter){

            if(pw_1 != 9999){
                if(pw_1 > 500 && pw_1 < 2500){
                    this->virtual_pw_1 = pw_1;
                }
                else{
                    throw std::invalid_argument("ValueError");
                }
            }

            if(pw_2 != 9999){
                if(pw_2 > 500 && pw_2 < 2500){
                    this->virtual_pw_2 = pw_2;
                }
                else{
                    throw std::invalid_argument("ValueError");
                }
            }

        }
        else{

            if(pw_1 != 9999){
                gpioSetMode(14, PI_OUTPUT);
                gpioServo(14, pw_1); // Example pulse width value (1500 microseconds)
            }
            if(pw_2 != 9999){
                gpioSetMode(15, PI_OUTPUT);
                gpioServo(15, pw_2); // Example pulse width value (1500 microseconds)
            }

        }

    }

    void set_angles(double angle_1 = 9999, double angle_2 = 9999){
        double pw_1 = 9999;
        double pw_2 = 9999;

        if(angle_1 != 9999){
            pw_1 = this->angles_to_pw_1(angle_1);

            if(pw_1 > this->previous_pw_1){
                this->active_hysteresis_correction_1 = this->hysteresis_correction_1;
            }
            else if(pw_1 < this->previous_pw_1){
                this->active_hysteresis_correction_1 = -1 * this->hysteresis_correction_1;
            }

            this->previous_pw_1 = pw_1;

            pw_1 = pw_1 + this->active_hysteresis_correction_1;

            this->angle_1 = angle_1;
            this->angles_used_1.insert(angle_1);
            this->pulse_widths_used_1.insert(pw_1);
        }

        if(angle_2 != 9999){
            pw_2 = this->angles_to_pw_2(angle_2);

            if(pw_2 > this->previous_pw_2){
                this->active_hysteresis_correction_2 = this->hysteresis_correction_2;
            }
            else if(pw_2 < this->previous_pw_2){
                this->active_hysteresis_correction_2 = -1 * this->hysteresis_correction_2;
            }

            this->previous_pw_2 = pw_2;

            pw_2 = pw_2 + this->active_hysteresis_correction_2;

            this->angle_2 = angle_2;
            this->angles_used_2.insert(angle_2);
            this->pulse_widths_used_2.insert(pw_2);
        }

        std::pair<double,double> x_and_y = this->angles_to_xy(this->angle_1, this->angle_2);
        this->x = x_and_y.first;
        this->y = x_and_y.second;

        // if(this->turtle){
        // }

        this->set_pulse_widths(pw_1, pw_2);

    }

    void move_angles(double angle_1 = 9999, double angle_2 = 9999, double angular_step = 9999, double wait = 9999, bool draw = false){
        if(wait != 9999){
            wait = wait;
        }
        else{
            wait = this->wait;
        }
    
        if(angular_step != 9999){
            angular_step = angular_step;
        }
        else{
            angular_step = this->angular_step;
        }
    
        if(draw){
            this->pen->down();
        }
        else{
            this->pen->up();
        }

        double diff_1 = 0, diff_2 = 0;

        if(angle_1 != 9999){
            diff_1 = angle_1 - this->angle_1;
        }
        if(angle_2 != 9999){
            diff_2 = angle_2 - this->angle_2;
        }
        
        double abs_diff_1 = std::abs(diff_1 / angular_step);
        double abs_diff_2 = std::abs(diff_2 / angular_step);
        int no_of_steps = static_cast<int>(std::max(abs_diff_1, abs_diff_2));
        no_of_steps = (no_of_steps != 0) ? no_of_steps : 1;

        bool disable_tqdm;
        if(no_of_steps < 100){
            disable_tqdm = true;
        }
        else{
            disable_tqdm = false;
        }

        double length_of_step_1 = diff_1/no_of_steps;
        double length_of_step_2 = diff_2/no_of_steps;

        //Tqdm ekle



    }

    void park(){
        if(this->virtualPlotter){
            std::cout << "Parking" << std::endl;
        }

        this->pen->up();
        this->move_angles(this->servo_1_parked_angle, this->servo_2_parked_angle);
    }

    void xy(double x = 9999, double y = 9999, double angular_step = 9999, double wait = 9999, double resolution = 9999, bool draw = false){

        wait = (wait != 9999) ? wait : this->wait;
        resolution = (resolution != 9999) ? resolution : this->resolution;

        x = (x != 9999) ? x : this->x;
        y = (y != 9999) ? y : this->y;

        std::pair<double, double> angles = this->xy_to_angles(x , y);
        double angle_1 = angles.first;
        double angle_2 = angles.second;

        if(draw){

            double x_length = x - this->x;
            double y_length = y - this->y;

            double length = std::sqrt(x_length * x_length + y_length * y_length);
            
            int no_of_steps = static_cast<int>(std::round(length / resolution));
            no_of_steps = (no_of_steps != 0) ? no_of_steps : 1;

            bool disable_tqdm;
            if(no_of_steps < 100){
                disable_tqdm = true;
            }
            else{
                disable_tqdm = false;
            }

            double length_of_step_x = x_length / no_of_steps;
            double length_of_step_y = y_length / no_of_steps;

            for(int step = 0; step < no_of_steps; step++){
                this->x = this->x + length_of_step_x;
                this->y = this->y + length_of_step_y;

                std::pair<double,double> angles = this->xy_to_angles(this->x , this-> y);
                double angle_1 = angles.first;
                double angle_2 = angles.second;

                this->move_angles(angle_1, angle_2, angular_step, wait, draw);
            }
        }
        else{
            this->move_angles(angle_1, angle_2, angular_step, wait, draw);
        }

    }

    void draw_line(std::pair<double,double> start = std::make_pair(0,0), std::pair<double,double> end = std::make_pair(0,0), double angular_step = 9999, double wait = 9999, double resolution = 9999, bool both = false){
        double start_x = start.first;
        double start_y = start.second;
        double end_x = end.first;
        double end_y = end.second;


        this->xy(start_x, start_y, angular_step, wait, resolution);
        this->xy(end_x, end_y, angular_step, wait, resolution, true);

        if(both){
            this->xy(start_x, start_y, angular_step, wait, resolution, true);
        }
    }

    void plot_lines(){

    }

    int left(){
        return this->bounds[0];
    }

    int bottom(){
        return this->bounds[1];
    }

    int right(){
        return this->bounds[2];
    }

    int top(){
        return this->bounds[3];
    }

    std::pair<double,double> get_pulse_widths(){
        double actual_pulse_width_1, actual_pulse_width_2;
        if(this->virtualPlotter){
            actual_pulse_width_1 = this->virtual_pw_1;
            actual_pulse_width_2 = this->virtual_pw_2;
        }
        else{
            actual_pulse_width_1 = gpioGetServoPulsewidth(14);
            actual_pulse_width_2 = gpioGetServoPulsewidth(15);
        }

        return std::make_pair(actual_pulse_width_1, actual_pulse_width_2);
    }

    void status(){
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "                      | Servo 1 | Servo 2 " << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        
        std::pair<double,double> pulse_widths = this->get_pulse_widths();

        double pw_1 = pulse_widths.first;
        double pw_2 = pulse_widths.second;

        std::cout << std::setw(23) << std::right << "pulse-width |"
            << std::setw(7) << std::right << std::fixed << std::setprecision(0) << pw_1 << " |"
            << std::setw(7) << std::right << std::fixed << std::setprecision(0) << pw_2 << std::endl;

        double angle_1 = this->angle_1;
        double angle_2 = this->angle_2;

        std::cout << std::setw(23) << std::right << "angle |"
            << std::setw(7) << std::right << std::fixed << std::setprecision(0) << angle_1 << " |"
            << std::setw(7) << std::right << std::fixed << std::setprecision(0) << angle_2 << std::endl;

        double h1 = this->hysteresis_correction_1;
        double h2 = this->hysteresis_correction_2;

        std::cout << std::setw(23) << std::right << "hysteresis correction |"
            << std::setw(7) << std::right << std::fixed << std::setprecision(0) << h1 << " |"
            << std::setw(7) << std::right << std::fixed << std::setprecision(0) << h2 << std::endl;
        
        std::cout << "------------------------------------------" << std::endl;
        
        std::cout << std::setw(23) << std::right << "x/y location |"
            << std::setw(7) << std::right << std::fixed << std::setprecision(0) << x << " |"
            << std::setw(7) << std::right << std::fixed << std::setprecision(0) << y << std::endl;

        std::cout << "------------------------------------------" << std::endl;

        std::cout << "pen:" << this->pen->getPosition() << std::endl;
        std::cout << "------------------------------------------" << std::endl;

        std::cout << "left: " << this->left() << ", right: " << this->right()
              << ", top: " << this->top() << ", bottom: " << this->bottom() << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "wait: " << this->wait << " seconds" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "resolution: " << this->resolution << " cm" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "angular step: " << this->angular_step << "˚" << std::endl;
        std::cout << "------------------------------------------" << std::endl;

    }

    Plotter(
        bool virtualPlotter = false,
        bool turtle = false,
        double turtle_coarseness = 9999,
        std::array<int, 4> bounds = {-10, 5, 10, 15},
        double servo_1_parked_pw = 1500,
        double servo_2_parked_pw = 1500,
        double servo_1_degree_ms = -10,
        double servo_2_degree_ms = 10,
        double servo_1_parked_angle = 0,
        double servo_2_parked_angle = 0,
        double hysteresis_correction_1 = 0,
        double hysteresis_correction_2 = 0,
        std::vector<std::pair<double,double>> servo_1_angle_pws = std::vector<std::pair<double,double>>(),
        std::vector<std::pair<double,double>> servo_2_angle_pws = std::vector<std::pair<double,double>>(),
        std::map<double, std::map<std::string, double>> servo_1_angle_pws_bidi = std::map<double, std::map<std::string, double>>(),
        std::map<double, std::map<std::string, double>> servo_2_angle_pws_bidi = std::map<double, std::map<std::string, double>>(),
        double pw_up = 9999,
        double pw_down = 9999,
        double angular_step = 9999,
        double wait = 9999,
        double resolution = 9999
    ){
        this->last_moved = std::chrono::steady_clock::now();
        this->virtualPlotter = virtualPlotter;
        this->angle_1 = servo_1_parked_angle;
        this->angle_2 = servo_2_parked_angle;

        // if(turtle){
        //     try{

        //     }
        //     catch(){

        //     }
        // }
        // else
        this->turtle = false; // You can add port of Turtle to C++ maybe?
        this->bounds = bounds;

        this->servo_1_parked_pw = servo_1_parked_pw;
        this->servo_1_degree_ms = servo_1_degree_ms;
        this->servo_1_parked_angle = servo_1_parked_angle;
        this->hysteresis_correction_1 = hysteresis_correction_1;

        if(!servo_1_angle_pws_bidi.empty()){
            servo_1_angle_pws.clear();
            std::vector<double> differences;
            for (const auto& angle_pws_pair : servo_1_angle_pws_bidi) {
                double angle = angle_pws_pair.first;
                const auto& pws = angle_pws_pair.second;
                double acw = pws.at("acw");
                double cw = pws.at("cw");            
                
                double pw = (acw + cw) / 2;

                servo_1_angle_pws.push_back(std::make_pair(angle, pw));
                differences.push_back((acw - cw) / 2);
            }
            this->hysteresis_correction_1 = xt::mean(differences);
        }

        if(!servo_1_angle_pws.empty()){
            std::vector<double> first_elements;
            std::vector<double> second_elements;
            for (const auto& pair : servo_1_angle_pws) {
                first_elements.push_back(pair.first);
                second_elements.push_back(pair.second);
            }

            this->angles_to_pw_1 = utils::Polynomial::getFunctionPtr(
                utils::polyfit(first_elements, second_elements, 3)
                );

        }
        else{

            //#include <functional>
            //    std::function<double(double)> angles_to_pw_1;
            //    std::function<double(double)> angles_to_pw_2;
        //            angles_to_pw_1 = [this](double angle) { return naive_angles_to_pulse_widths_1(angle); };
         //            angles_to_pw_2 = [this](double angle) { return naive_angles_to_pulse_widths_2(angle); };
           
            this->angles_to_pw_1 = &Plotter::static_angles_to_pw_1_wrapper;
        }

        this->servo_2_parked_pw = servo_2_parked_pw;
        this->servo_2_degree_ms = servo_2_degree_ms;
        this->servo_2_parked_angle = servo_2_parked_angle;
        this->hysteresis_correction_2 = hysteresis_correction_2;
            
        
        if(!servo_2_angle_pws_bidi.empty()){
            servo_2_angle_pws.clear();
            std::vector<double> differences;
            for (const auto& angle_pws_pair : servo_2_angle_pws_bidi) {
                double angle = angle_pws_pair.first;
                const auto& pws = angle_pws_pair.second;
                double acw = pws.at("acw");
                double cw = pws.at("cw");            
                
                double pw = (acw + cw) / 2;

                servo_2_angle_pws.push_back(std::make_pair(angle, pw));
                differences.push_back((acw - cw) / 2);
            }
            this->hysteresis_correction_2 = xt::mean(differences);
        }

        if(!servo_2_angle_pws.empty()){
            std::vector<double> first_elements;
            std::vector<double> second_elements;
            for (const auto& pair : servo_2_angle_pws) {
                first_elements.push_back(pair.first);
                second_elements.push_back(pair.second);
            }

            this->angles_to_pw_2 = utils::Polynomial::getFunctionPtr(
                utils::polyfit(first_elements, second_elements, 3)
                );

        }
        else{
            this->angles_to_pw_2 = &this->naive_angles_to_pulse_widths_2;
        }

        this->previous_pw_1 = this->previous_pw_2 = 0;
        this->active_hysteresis_correction_1 = this->active_hysteresis_correction_2 = 0;
        this->reset_report();

        if(this->virtualPlotter){
            if(wait != 9999)
                this->wait = wait;
            else this->wait = 0;
            this->virtualise();
        }
        else{
            try{
                gpioCfgSetInternals(1);

                gpioInitialise();

                gpioSetPWMfrequency(14, 50);
                gpioSetPWMfrequency(15, 50);

                gpioCfgSetInternals(0); 
                this->virtualPlotter = false;
                // by default we use a wait factor of 0.01 seconds for better control
                if(wait != 9999)
                    this->wait = wait;
                else    
                    this->wait = 0.01;
            }
            catch(...){
                std::cout << "pigpio daemon is not available; running in virtual mode" << std::endl;
                this->virtualise();
                if(wait != 9999)
                    this->wait = wait;
                else    
                    this->wait = 0;
            }
        }

        if(pw_up != 9999)
            this->pw_up = pw_up;
        else    
            this->pw_up = 1400;
        
        if(pw_down != 9999)
            this->pw_down = pw_down;
        else    
            this->pw_down = 1600;


        this->pen = new Pen(this,pw_up,pw_down,this->virtualPlotter);

        if(angular_step != 9999)
            this->angular_step = angular_step;
        else    
            this->angular_step = 0.1;

        if(resolution != 9999)
            this->resolution = resolution;
        else    
            this->resolution = 0.1;

        this->set_angles(this->servo_1_parked_angle, this->servo_2_parked_angle);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        this->status();
    }   


};

