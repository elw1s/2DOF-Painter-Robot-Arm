// #include "plotter.cpp"

// double hypotenuse(double side1, double side2){
//     return std::sqrt(side1 * side1 + side2 * side2);
// }

// class PantoGraph : Plotter{
//     private:
//         std::queue<std::string>* messageQueue;
//         pthread_cond_t* condition;
//         pthread_mutex_t* mutex;
//         int driver;
//         int follower;
//         double motor_1_pos;
//         double motor_2_pos;
//         std::array<int, 4> bounds;
//         int angle_multiplier;
//         double correction_1;
//         double correction_2;
//         int centre_1;
//         double multiplier_1;
//         int centre_2;
//         double multiplier_2;
//         double servo_1_parked_pw;
//         double servo_2_parked_pw;
//         double servo_1_degree_ms;
//         double servo_2_degree_ms;
//         double servo_1_parked_angle;
//         double servo_2_parked_angle;
//         double hysteresis_correction_1;
//         double hysteresis_correction_2;
//         std::vector<std::pair<double,double>> servo_1_angle_pws;
//         std::vector<std::pair<double,double>> servo_2_angle_pws;
//         std::map<double, std::map<std::string, double>> servo_1_angle_pws_bidi;
//         std::map<double, std::map<std::string, double>> servo_2_angle_pws_bidi;
//         double pw_up;
//         double pw_down;
//         double wait;
//         double resolution;
//         bool virtualPlotter;
//         bool turtle;
//         double x;
//         double y;
//     public:

//         std::pair<double, double> angles_to_xy_pantograph(double angle_1 , double angle_2){
//             return this->angles_to_xy(angle_1, angle_2);
//         }

//         double get_first_angle(){
//             return this->get_angle_1();
//         }
        
//         double get_second_angle(){
//             return this->get_angle_2();
//         }

//         void setMessageQueue(std::queue<std::string>* newMessageQueue) {
//             messageQueue = newMessageQueue;
//         }

//         void setCondition(pthread_cond_t* newCondition) {
//             condition = newCondition;
//         }

//         void setMutex(pthread_mutex_t* newMutex) {
//             mutex = newMutex;
//         }

//         PantoGraph(
//             std::queue<std::string>* messageQueue = nullptr, 
//             pthread_cond_t* condition = nullptr, 
//             pthread_mutex_t* mutex = nullptr,
//             int driver = 8,
//             int follower = 8,
//             double motor_1_pos = -1.5,
//             double motor_2_pos = 1.5,
//             std::array<int, 4> bounds = {-10, 5, 10, 15},
//             int angle_multiplier = 1,
//             double correction_1 = 0,
//             double correction_2 = 0,
//             int centre_1 = 1500,
//             double multiplier_1 = 425/45,
//             int centre_2 = 1500,
//             double multiplier_2 = 415/45,
//             double servo_1_parked_pw = 1500,
//             double servo_2_parked_pw = 1500,
//             double servo_1_degree_ms = -10,
//             double servo_2_degree_ms = 10,
//             double servo_1_parked_angle = -45,
//             double servo_2_parked_angle = 45,
//             double hysteresis_correction_1 = 0,
//             double hysteresis_correction_2 = 0,
//             std::vector<std::pair<double,double>> servo_1_angle_pws = std::vector<std::pair<double,double>>(),
//             std::vector<std::pair<double,double>> servo_2_angle_pws = std::vector<std::pair<double,double>>(),
//             std::map<double, std::map<std::string, double>> servo_1_angle_pws_bidi = std::map<double, std::map<std::string, double>>(),
//             std::map<double, std::map<std::string, double>> servo_2_angle_pws_bidi = std::map<double, std::map<std::string, double>>(),
//             double pw_up = 1500,
//             double pw_down = 1100,
//             double wait = 9999,
//             double resolution = 9999,
//             bool virtualPlotter = false,
//             bool turtle = false
//         ): Plotter(
//         messageQueue, 
//         condition, 
//         mutex,
//         virtualPlotter,
//         turtle,
//         9999,
//         bounds,
//         servo_1_parked_pw,
//         servo_2_parked_pw,
//         servo_1_degree_ms,
//         servo_2_degree_ms,
//         servo_1_parked_angle,
//         servo_2_parked_angle,
//         hysteresis_correction_1,
//         hysteresis_correction_2,
//         servo_1_angle_pws,
//         servo_2_angle_pws,
//         servo_1_angle_pws_bidi,
//         servo_2_angle_pws_bidi,
//         pw_up,
//         pw_down,
//         9999,
//         wait,
//         resolution
//     ){
//         this->driver = driver;
//         this->follower = follower;
//         this->motor_1_pos = motor_1_pos;
//         this->motor_2_pos = motor_2_pos;

//         this->angle_multiplier = angle_multiplier;

//         this->correction_1 = correction_1;
//         this->correction_2 = correction_2;

//         this->centre_1 = centre_1;
//         this->centre_2 = centre_2;

//         this->multiplier_1 = multiplier_1;
//         this->multiplier_2 = multiplier_2;

//         this->set_angles(0,0);
//         std::pair<double,double> x_and_y = this->angles_to_xy(0,0);
//         this->x = x_and_y.first;
//         this->y = x_and_y.second;

//         this->quiet();
//     }

// };