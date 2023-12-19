// #include <cmath>
// #include <iostream>
// #include "CTurtle.hpp"
// #include "pantograph.cpp"

// namespace ct = cturtle;


// /*

// 1- Plotter için mutex cond ve messageQueue asla set edilmiyor bunu düzelt
// 2- PantoGraph eksik onu tamamla

// */


// class BaseTurtle : public ct::Turtle{
//     private:

//         int window_size;
//         double reach;
//         PantoGraph* machine;
//         int coarseness;
//         double angle_1;
//         double angle_2;
//         double draw_reach;

//     public:

//         ct::InteractiveTurtleScreen* screen;
//         double multiplier;

//         BaseTurtle(int window_size = 800, double reach = 16, int speed = 0, PantoGraph* machine = nullptr, int coarseness = 0
//         ){
            
//             this->window_size = window_size;
//             this->reach = reach;
//             this->machine = machine;
//             this->coarseness = coarseness;

//             if(this->machine){
//                 this->angle_1 = this->machine->get_first_angle();
//                 this->angle_2 = this->machine->get_second_angle();
//             }

//             double grid_size = (this->window_size / 1.05);

//             this->multiplier = grid_size / 2 / this->reach;

//             this->draw_reach = (this->reach * this->multiplier * 1.05);

//             this->screen = new ct::TurtleScreen(window_size, window_size);
//             this->screen->mode(ct::ScreenMode::SM_LOGO);
//             this->speed(0);
//             this->screen->tracer(speed, 0);
//         }    

//         void draw_grid(){
//             this->draw_grid_lines(1, "#bbb", 1, false);
//             this->draw_grid_lines(5, "#black", 1, true);
//         }

//         void draw_grid_lines(int draw_every = 1, std::string color = "gray", int width = 1, bool include_numbers = false){
//             this->pencolor(ct::Color(color));
//             this->width(width);

//             for(int i = 0; i < (-1 * this->reach); i += (this->reach + 1)){
//                 if(!(i % draw_every)){
//                     int draw_i = i * this->multiplier;
//                     this->penup();
//                     this->goTo(draw_i, -1 * this->draw_reach);
//                     this->pendown();
//                     this->goTo(draw_i, this->draw_reach);
//                     this->penup();
//                     this->goTo(-1 * this->draw_reach, draw_i);
//                     this->pendown();
//                     this->goTo(this->draw_reach, draw_i);

//                     if(include_numbers){
//                         this->penup();
//                         this->goTo(i * this->multiplier, -1 * this->multiplier);
//                         this->write(" " + std::to_string(i), "Helvetica");
//                         this->goTo(-1 * this->reach * this->multiplier, i * this->multiplier);
//                         this->write(std::to_string(i), "Helvetica");
//                     }
//                 }
//             }
//         }

//         void set_angles(double angle_1, double angle_2){
//             if(this->coarseness){
//                 double coarsened_angle_1 = this->coarsen_angle(angle_1);
//                 double coarsened_angle_2 = this->coarsen_angle(angle_2);

//                 double diff_1 = coarsened_angle_1 - this->angle_1;
//                 double diff_2 = coarsened_angle_2 - this->angle_2;
//                 double length = std::sqrt(diff_1 * diff_1 + diff_2 * diff_2);
//                 int no_of_steps = length * 10;

//                 if(no_of_steps){
//                     double length_of_step_1 = diff_1 / no_of_steps;
//                     double length_of_step_2 = diff_2 / no_of_steps;

//                     for(int step = 0; step < no_of_steps; step++){
//                         this->angle_1 = this->angle_1 + length_of_step_1;
//                         this->angle_2 = this->angle_2 + length_of_step_2;

//                         std::pair<double,double> x_and_y = this->machine->angles_to_xy_pantograph(this->angle_1, this->angle_2);
//                         double x = x_and_y.first;
//                         double y = x_and_y.second;
//                         this->setpos(x * this->multiplier, y * this->multiplier);
//                     }
//                 }
                
//             }
//             else{
//                 std::pair<double,double> x_and_y = this->machine->angles_to_xy_pantograph(this->angle_1, this->angle_2);
//                 double x = x_and_y.first;
//                 double y = x_and_y.second;
//                 this->setpos(x * this->multiplier, y * this->multiplier);
//             }
//         }

//         double coarsen_angle(double angle){
//             return std::round(angle / this->coarseness) * this->coarseness;
//         }

// };

// class BrachioGraphTurtle: public BaseTurtle{

//     private:

//     double inner_arm;
//     double outer_arm;
//     double shoulder_centre_angle;
//     double shoulder_sweep;
//     double elbow_centre_angle;
//     double elbow_sweep;

//     public:

//         BrachioGraphTurtle(
//             double inner_arm = 8,
//             double outer_arm = 8,
//             int window_size = 800,
//             int speed = 0,
//             double shoulder_centre_angle = 0,
//             double elbow_centre_angle = 90,
//             double shoulder_sweep = 180,
//             double elbow_sweep = 180,
//             PantoGraph* machine,
//             int coarseness = 0
//         )
//         :BaseTurtle(
//             window_size,
//             inner_arm + outer_arm,
//             speed,
//             machine,
//             coarseness
//         ){
//             this->inner_arm = inner_arm;
//             this->outer_arm = outer_arm;
//             this->shoulder_centre_angle = shoulder_centre_angle;
//             this->shoulder_sweep = shoulder_sweep;
//             this->elbow_centre_angle = elbow_centre_angle;
//             this->elbow_sweep = elbow_sweep;

//         }

//         void draw_pen_arc(int width = 1, std::string color = "black"){
//             this->penup();
//             this->rt(180);
//             this->fd(this->outer_arm * this->multiplier);
//             this->rt(-90);

//             this->circle(this->outer_arm * this->multiplier, (360- this->elbow_sweep) / 2, color);

//             this->pencolor(color);
//             this->pendown();
//             this->width(width);
//             this->circle(this->outer_arm * this->multiplier, this->elbow_sweep, color);
//         }

//         double radians(double degrees) {
//             return degrees * (M_PI / 180.0);
//         }   
//         double degrees(double radians) {
//             return radians * (180.0 / M_PI);
//         }

//         void draw_arms_arc(double elbow_centre_angle, int width, std::string color = "black", bool reverse = false){
//             double reach = std::sqrt(
//                 this->inner_arm * this->inner_arm
//                 + this->outer_arm * this->outer_arm
//                 - 2
//                 * this->inner_arm
//                 * this->outer_arm
//                 * std::cos(
//                     radians(180 - elbow_centre_angle)
//                 )
//             );

//             double a;
//             if(reach == 0){
//                 a = 0;
//             }
//             else if((this->inner_arm * this->inner_arm + reach * reach - this->outer_arm * this->outer_arm)
//             / (2 * this->inner_arm * reach) > 1){
//                 a = 0;
//             }
//             else{
//                 a = std::acos(
//                     (this->inner_arm * this->inner_arm + reach * reach - this->outer_arm * this->outer_arm)
//                     / (2 * this->inner_arm * reach)
//                 );
//             }

//             double heading = this->shoulder_centre_angle + this->shoulder_sweep / 2 + degrees(a);
//             double sweep;
//             if(reverse){
//                 sweep = this->shoulder_sweep * -1;
//                 heading = heading - this->shoulder_sweep;
//             }
//             else{
//                 sweep = this->shoulder_sweep;
//             }

//             this->draw_arc_around_origin(heading,reach,sweep,width,color);

//         }

//         void draw_arc_around_origin(double heading, double reach, double sweep, int width, std::string color){
//             this->penup();
//             this->home();
//             this->rt(heading);
//             this->fd(reach * this->multiplier);
//             this->setheading(heading - 90);
//             this->pendown();
//             this->width(width);
//             this->pencolor(color);
//             this->circle(reach * this->multiplier, sweep, color);
//         }

//         void draw_outline(int width = 4, std::string color, int lightness = 1){
//             double outer_arm_angle = this->elbow_centre_angle - this->elbow_sweep / 2;
//             this->draw_arms_arc(outer_arm_angle, width, color.empty() ? "blue" : color);

//             this->penup();
//             this->home();
//             this->rt(this->shoulder_centre_angle - this->shoulder_sweep / 2);
//             this->fd(this->inner_arm * this->multiplier);
//             this->rt(this->elbow_centre_angle);
//             this->draw_pen_arc(width,color.empty() ? "red" : color);

//             outer_arm_angle = this->elbow_centre_angle + this->elbow_sweep / 2;
//             this->draw_arms_arc(outer_arm_angle, width, color.empty() ? "purple4" : color, true);
        

//             this->penup();
//             this->home();
//             this->rt(this->shoulder_centre_angle + this->shoulder_sweep / 2);
//             this->fd(this->inner_arm * this->multiplier);
//             this->rt(this->elbow_centre_angle);
//             this->draw_pen_arc(width, color.empty() ? "orange" : color);

//             this->screen->update(false, false);
        
//         }

//         void draw_arcs(int every = 2, std::string color = "orange"){
//             for(int angle = (this->shoulder_centre_angle + this->shoulder_sweep / 2); angle < (this->shoulder_centre_angle - this->shoulder_sweep / 2 - 1); angle -= every){

//                 this->penup();
//                 this->home();

//                 this->rt(angle);
//                 this->fd(this->inner_arm * this->multiplier);

//                 this->rt(this->elbow_centre_angle);

//                 this->draw_pen_arc(1, color);

//             }
//         }

//         void draw_arms(int every = 60){
//             for(int angle = (this->shoulder_centre_angle + this->shoulder_sweep / 2); angle < (this->shoulder_centre_angle - this->shoulder_sweep / 2 - 1); angle -= every){

//                 this->penup();
//                 this->home();
//                 this->width(6);

//                 this->pendown();

//                 this->pencolor(std::string("blue"));
//                 this->rt(angle);
//                 this->fd(this->inner_arm * this->multiplier);

//                 this->rt(this->elbow_centre_angle);
//                 this->pencolor(std::string("red"));
//                 this->fd(this->outer_arm * this->multiplier);

//             }
//             this->screen->update(false,false);
//         }

// };

// class PantoGraphTurtle : public BaseTurtle{

//     private:
//         int driver;
//         int follower;
//         double motor_1_pos;
//         double motor_2_pos;
//         int window_size;
//         int speed;
//         double motor_1_centre_angle;
//         double motor_2_centre_angle;
//         int motor_1_sweep;
//         int motor_2_sweep;
//         PantoGraph* machine;
//         int coarseness;
//     public:

//         PantoGraphTurtle(
//             int driver = 8,
//             int follower = 8,
//             double motor_1_pos = -1.5,
//             double motor_2_pos = 1.5,
//             int window_size = 800,
//             int speed = 0,
//             double motor_1_centre_angle = 0,
//             double motor_2_centre_angle = 0,
//             int motor_1_sweep = 180,
//             int motor_2_sweep = 180,
//             PantoGraph* machine,
//             int coarseness = 0
//         ):BaseTurtle(
//             window_size,
//             driver + follower,
//             speed,
//             machine,
//             coarseness
//         ){
//             this->driver = driver;
//             this->follower = follower;
//             this->motor_1_pos = motor_1_pos;
//             this->motor_2_pos = motor_2_pos;
//             this->motor_1_centre_angle = motor_1_centre_angle;
//             this->motor_2_centre_angle = motor_2_centre_angle;
//             this->motor_1_sweep = motor_1_sweep;
//             this->motor_2_sweep = motor_2_sweep;
//         }



// };

