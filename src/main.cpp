#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"

// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  PID pid;
  /**
   * TODO: Initialize the pid variable.
   *
   */
    
    pid.Init(0.8, 0.0001, 8);
    pid.Init_p({0.278,0.0001,1.29147});
   

  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
      
    static double err_sum = 0.0;
    static int run_counter = 0;
    static bool off_road = false;
    static const bool run_twiddle = true;
      
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());
          double steer_value;
          /**
           * TODO: Calculate steering value here, remember the steering value is
           *   [-1, 1].
           * NOTE: Feel free to play around with the throttle and speed.
           *   Maybe use another PID controller to control the speed!
           */
            
          
            
            pid.UpdateError(cte);
            steer_value = pid.TotalError();
//            std::cout << "CTE: " << cte << " Steering Value: " << steer_value
//            << std::endl;
            if (steer_value > 1) steer_value = 1;
            else if (steer_value < -1) steer_value = -1;
            
            // make err positive and reward < 1 cte but power by 2
            // DEBUG
//            std::cout << "CTE: " << cte << " Steering Value: " << steer_value
//            << std::endl;
            if (run_twiddle){
                // skip initial 100 cte, meaning only get only it is steablize
                if (run_counter> 100){
                    err_sum += cte*cte;
                    if (cte > 5) {
                        off_road = true;
                    }
                }
            
                run_counter += 1;
            
              // Run twiddle if the it finish running or run outside of the lane
                if (run_counter > 1500 || off_road){
                    double average_err = err_sum/(run_counter - 100);
                    if (off_road){
                        average_err += 1000;
                    }
                    
                    pid.twiddle(average_err);
                    pid.print_output(average_err);
                    
                    run_counter = 0;
                    err_sum = 0.0;
                    off_road = false;
                    //Resetting simulator
                    std::string msg("42[\"reset\", {}]");
                    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
                }
            }
            
            
            
          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
//          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
    
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}
