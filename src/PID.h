#ifndef PID_H
#define PID_H
#include <vector>
#include <iostream>
using std::vector;

enum TwiddleState {INIT, INCREMENT,DECREMENT};

class PID {
 public:
  /**
   * Constructor
   */
  PID();

  /**
   * Destructor.
   */
  virtual ~PID();

  /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  void Init(double Kp_, double Ki_, double Kd_);

  /**
   * Update the PID error variables given cross track error.
   * @param cte The current cross track error
   */
  void UpdateError(double cte);

  /**
   * Calculate the total PID error.
   * @output The total PID error
   */
  double TotalError();
    
    
  void twiddle(double current_err);
  void Init_p(vector<double> input_dp);
  void move_index();
  void print_output(double current_err);

 private:
  /**
   * PID Errors
   */
  double p_error;
  double i_error;
  double d_error;

  /**
   * PID Coefficients
   */ 
  double Kp;
  double Ki;
  double Kd;
    
  vector<double> dp;
// Tolerance
  double tol;
  double best_err;
  TwiddleState state;
    
  vector<double> p;
    
    int index = 0;
    int iter = 0;
  
};

#endif  // PID_H
