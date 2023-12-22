#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <iomanip>
#include <map>
#include <set>
#include <unistd.h>
#include <cmath>
#include <functional>
#include <fstream>
#include "utils.cpp"
#include <pigpio.h>
#include "rapidjson/document.h"

//namespace ct = cturtle;
class Plotter;

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
            std::cout << "Inside PEN constructor: " << this->pin << std::endl;
            std::cout << "Inside PEN constructor: " << pin << std::endl;
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
            std::cout << "In down\n";
            if(strcmp(this->position, "up") == 0){
                if(this->virtualPlotter){
                    this->virtual_pw = this->pw_down;
                }
                else{
                    std::cout << "Ease pen called from down\n";
                    this->ease_pen(this->pw_up, this->pw_down);
                }

                strcpy(this->position, "down");
            }

        }

        void up(){
            std::cout << "In up\n";
            if(strcmp(this->position,"down") == 0){

                if(this->virtualPlotter){
                    this->virtual_pw = this->pw_up;
                }
                else{
                    std::cout << "Ease pen called from up\n";
                    this->ease_pen(this->pw_down, this->pw_up);
                }
                
                strcpy(this->position, "up");
            }
        }

        void ease_pen(double start, double end){
            double diff = end - start;
            double angle = start;
            double length_of_step = diff / abs(diff);
            std::cout << "inside ease_pen\n";
            std::cout << "PIN: " << this->pin << std::endl;
            std::cout << "diff: " << diff << std::endl;
            std::cout << "abs(diff):" << abs(diff) << std::endl;
            gpioSetMode(this->pin, PI_OUTPUT);
            for(int i = 0; i < abs(diff); i++){
                angle += length_of_step;
                gpioServo(this->pin, angle);
                std::cout << "angle:" << angle << std::endl;
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
    //std::chrono::steady_clock::time_point last_moved;
    double last_moved;
    double angle_1;
    double angle_2;
    //double (*angles_to_pw_1)(double);
    //double (*angles_to_pw_2)(double);
    std::function<double(double)> angles_to_pw_1;
    std::function<double(double)> angles_to_pw_2;
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
    

public:

    Pen* pen;

    double get_angle_1(){
        return this->angle_1;
    }

    double get_angle_2(){
        return this->angle_2;
    }

    std::set<double> get_angle_used_1(){
        return this->angles_used_1;
    }

    std::set<double> get_angle_used_2(){
        return this->angles_used_2;
    }

    std::set<double> get_pulse_widths_used_1(){
        return this->pulse_widths_used_1;
    }

    std::set<double> get_pulse_widths_used_2(){
        return this->pulse_widths_used_2;
    }

    //Silebilirsin
    //static double static_angles_to_pw_1_wrapper(Plotter* plotter, double angle){
    //    return plotter->naive_angles_to_pulse_widths_1(angle);
    //}

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
    virtual std::pair<double,double> xy_to_angles(double x = 0, double y = 0){
        return std::make_pair(0,0);
    }
        /*Return the servo angles required to reach any x/y position. This is a dummy method in
        the base class; it needs to be overridden in a sub-class implementation.*/

    virtual std::pair<double,double> angles_to_xy(double angle_1, double angle_2){
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
        
        int no_of_steps = std::max(std::abs(diff_1 / angular_step), std::abs(diff_2 / angular_step));
        no_of_steps = std::max(no_of_steps, 1);

        bool disable_tqdm;
        if(no_of_steps < 100){
            disable_tqdm = true;
        }
        else{
            disable_tqdm = false;
        }

        double length_of_step_1 = diff_1/no_of_steps;
        double length_of_step_2 = diff_2/no_of_steps;
        
        for(int i = 0; i < no_of_steps; i++){
            this->angle_1 = this->angle_1 + length_of_step_1;
            this->angle_2 = this->angle_2 + length_of_step_2;
            double time_since_last_moved = this->monotonic() - this->last_moved;
            if(time_since_last_moved < wait){
                std::chrono::microseconds duration(static_cast<long long>((wait * 1e6) - time_since_last_moved));
                std::this_thread::sleep_for(duration);
            }

            this->set_angles(this->angle_1, this->angle_2);

            //this->last_moved = std::chrono::steady_clock::now();
            this->last_moved = this->monotonic();
            }
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
                std::cout << "no_of_steps : " << no_of_steps << " , length_of_step_x : " << length_of_step_x << " , length_of_step_y : " << length_of_step_y << std::endl;
                std::cout << "ANGLE 1 : " << angle_1 << " , ANGLE 2 : " << angle_2 << std::endl;
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
    
    std::tuple<bool, double, double, double, double, double> analyse_lines(std::vector<std::vector<std::vector<double>>> lines, bool rotate = false, std::array<int, 4> bounds = {9999,9999,9999,9999}){
       
        if(bounds[0] == bounds[1] && bounds[1] == bounds[2] && bounds[2] == bounds[3] && bounds[3] == 9999){
            bounds = this->bounds;
        }
    
        std::set<double> x_values_in_lines;
        std::set<double> y_values_in_lines;
    
        for (const auto& line : lines) {
            for (const auto& point : line) {
                x_values_in_lines.insert(point[0]);
                y_values_in_lines.insert(point[1]);
            }
        }

        int min_x = *std::min_element(x_values_in_lines.begin(), x_values_in_lines.end());
        int max_x = *std::max_element(x_values_in_lines.begin(), x_values_in_lines.end());
        int min_y = *std::min_element(y_values_in_lines.begin(), y_values_in_lines.end());
        int max_y = *std::max_element(y_values_in_lines.begin(), y_values_in_lines.end());

        int x_range = max_x - min_x;
        int y_range = max_y - min_y;
        int box_x_range = bounds[2] - bounds[0];
        int box_y_range = bounds[3] - bounds[1];

        double x_mid_point = (max_x + min_x) / 2.0;
        double y_mid_point = (max_y + min_y) / 2.0;
        double box_x_mid_point = (bounds[0] + bounds[2]) / 2.0;
        double box_y_mid_point = (bounds[1] + bounds[3]) / 2.0;

        double divider, temp;
        
        if ((x_range >= y_range && box_x_range >= box_y_range) ||
            (x_range <= y_range && box_x_range <= box_y_range)) {
            
            divider = std::max((double)x_range / box_x_range, (double)y_range / box_y_range);
            rotate = false;
        } else {
            temp = std::max((double)x_range / box_y_range, (double)y_range / box_x_range);
            divider = temp;
            rotate = true;
            std::swap(x_mid_point, y_mid_point);
        }

        return std::make_tuple(rotate, x_mid_point, y_mid_point, box_x_mid_point, box_y_mid_point, divider);
    }

    std::vector<std::vector<std::vector<double>>> rotate_and_scale_lines(std::vector<std::vector<std::vector<double>>> lines, bool rotate = false, bool flip = false, std::array<int, 4> bounds = {9999,9999,9999,9999})
    {
        auto analysed_lines = this->analyse_lines(lines, rotate, bounds);

        rotate = std::get<0>(analysed_lines);
        double x_mid_point = std::get<1>(analysed_lines);
        double y_mid_point = std::get<2>(analysed_lines);
        double box_x_mid_point = std::get<3>(analysed_lines);
        double box_y_mid_point = std::get<4>(analysed_lines);
        double divider = std::get<5>(analysed_lines);

        for (auto& line : lines) {
            for (auto& point : line) {
                if (rotate) {
                    std::swap(point[0], point[1]);
                }

                double x = point[0];
                x = x - x_mid_point;
                x = x / divider;

                if (flip ^ rotate) {
                    x = -x;
                }

                x = x + box_x_mid_point;

                double y = point[1];
                y = y - y_mid_point;
                y = y / divider;
                y = y + box_y_mid_point;

                point[0] = x;
                point[1] = y;
            }
        }

        return lines;
    }

    void plot_file(char * filename = "",std::array<int, 4> bounds = {9999,9999,9999,9999}, double angular_step = 9999, double wait = 9999, double resolution = 9999){

        if(bounds[0] == bounds[1] && bounds[1] == bounds[2] && bounds[2] == bounds[3] && bounds[3] == 9999){
            bounds = this->bounds;
        }

        std::ifstream file(filename);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();

            std::string json_str = buffer.str();

            rapidjson::Document doc;
            doc.Parse(json_str.c_str());

            if (!doc.IsArray()) {
                std::cerr << "Invalid JSON format!" << std::endl;
                return;
            }

            std::vector<std::vector<std::vector<double>>> lines;

            for (rapidjson::SizeType i = 0; i < doc.Size(); ++i) {
                if (!doc[i].IsArray()) {
                    std::cerr << "Invalid line format!" << std::endl;
                    return;
                }

                std::vector<std::vector<double>> line;

                for (rapidjson::SizeType j = 0; j < doc[i].Size(); ++j) {
                    if (!doc[i][j].IsArray() || doc[i][j].Size() != 2 || !doc[i][j][0].IsInt() || !doc[i][j][1].IsInt()) {
                        std::cerr << "Invalid point format in line!" << std::endl;
                        return;
                    }

                    double x = doc[i][j][0].GetDouble();
                    double y = doc[i][j][1].GetDouble();

                    line.push_back({x, y});
                }

                lines.push_back(line);
            }

            this->plot_lines(lines, bounds, angular_step, wait, resolution, true);
        } else {
            std::cerr << "Unable to open file" << std::endl;
        }
    }

    double round(double number, int digits) {
        double multiplier = std::pow(10.0, digits);
        return std::round(number * multiplier) / multiplier;
    }
    
    void box(std::array<int, 4> bounds = {9999,9999,9999,9999}, double angular_step = 9999, double wait = 9999, double resolution = 9999, int repeat = 1, bool reverse = false){
        std::array<int, 4> innerBounds = bounds[0] != 9999 ? bounds: this->bounds;

        this->xy(bounds[0],bounds[1], angular_step, wait, resolution);

        if(!reverse){
            this->xy(bounds.at(2),bounds.at(1), angular_step, wait, resolution,true) ;
            this->xy(bounds.at(2),bounds.at(3), angular_step, wait, resolution,true) ;
            this->xy(bounds.at(0),bounds.at(3), angular_step, wait, resolution,true) ;
            this->xy(bounds.at(0),bounds.at(1), angular_step, wait, resolution,true) ;
        }
        else{
            this->xy(bounds.at(0),bounds.at(3), angular_step, wait, resolution,true) ;
            this->xy(bounds.at(2),bounds.at(3), angular_step, wait, resolution,true) ;
            this->xy(bounds.at(2),bounds.at(1), angular_step, wait, resolution,true) ;
            this->xy(bounds.at(0),bounds.at(1), angular_step, wait, resolution,true) ;
        }
        this->park();
    }

    void plot_lines(std::vector<std::vector<std::vector<double>>> lines, std::array<int, 4> bounds = {9999,9999,9999,9999}, double angular_step = 9999 
        ,double wait = 9999, double resolution = 9999, bool flip = false, bool rotate = false)
    {
        if(bounds[0] == bounds[1] && bounds[1] == bounds[2] && bounds[2] == bounds[3] && bounds[3] == 9999){
            bounds = this->bounds;
        }
        

        //Çizilen lineları desktopa gönderirken bu fonksiyon öncesini gönder.
        lines = this->rotate_and_scale_lines(lines, false, true, bounds);

        for(const auto& line : lines){
            double x, y;
            x = line[0][0];
            y = line[0][1];

            if (round(this->x,1) != round(x,1) || round(this->y,1) != round(y,1)) {
                this->xy(x, y, angular_step, wait, resolution);
            }

            for (size_t i = 1; i < line.size(); ++i) {
                x = line[i][0];
                y = line[i][1];
                this->xy(x, y,angular_step,wait, resolution, true);
            }

            std::cout << "Çizilen Line: ";
            for (const auto& point : line) {
                std::cout << "(" << point[0] << ", " << point[1] << ") ";
            }
            std::cout << std::endl;        
        }

        this->park();

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

    double monotonic(){
        auto currentTime = std::chrono::steady_clock::now();
        auto duration = currentTime.time_since_epoch();
        double seconds = std::chrono::duration<double>(duration).count();
        return seconds;
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
        //this->last_moved = std::chrono::steady_clock::now();
        this->last_moved = monotonic();
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
            //this->hysteresis_correction_1 = xt::mean(differences);
            this->hysteresis_correction_1 = utils::mean(differences);
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
            this->angles_to_pw_1 = [this](double angle) {return this->naive_angles_to_pulse_widths_1(angle);};
            //this->angles_to_pw_1 = &Plotter::naive_angles_to_pulse_widths_1;
            //this->angles_to_pw_1 = std::bind(&Plotter::naive_angles_to_pulse_widths_1, this, std::placeholders::_1);
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
            //this->hysteresis_correction_2 = xt::mean(differences);
            this->hysteresis_correction_2 = utils::mean(differences);
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
            this->angles_to_pw_2 = [this](double angle) {return this->naive_angles_to_pulse_widths_2(angle);};
            //this->angles_to_pw_2 = &this->naive_angles_to_pulse_widths_2;
            //this->angles_to_pw_2 = std::bind(&Plotter::angles_to_pw_2, this, std::placeholders::_1);
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


        this->pen = new Pen(this,this->pw_up,this->pw_down,18,0.25,this->virtualPlotter);

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

