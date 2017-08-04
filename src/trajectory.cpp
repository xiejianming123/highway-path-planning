#include "trajectory.h"

Trajectory::Trajectory(Vehicle& car, const BehaviorType behavior){

  // get target states based on behavior s component
  double target_s = car.saved_state_s.p + TRAVERSE_TIME * car.saved_state_s.v;
  double target_v = car.saved_state_s.v;

  if (behavior == BehaviorType::KEEPLANE) {

    bool safe = (car.front_v > SPEED_LIMIT) || (car.front_gap > FRONT_BUFFER);
    target_v =  safe ? SPEED_LIMIT : (car.front_v - SPEED_BUFFER);
    target_v = target_v > MIN_SPEED ? target_v : MIN_SPEED;
    target_s =  car.saved_state_s.p + TRAVERSE_TIME * 0.5 * (car.saved_state_s.v + target_v);
  }

  this->targetState_s = {target_s, target_v, 0.0};

  // get target states based on behavior d component
  this->targetState_d = {car.get_target_d(behavior), 0.0, 0.0};

  // generate JMTs
  JMT jmt_s(car.saved_state_s, targetState_s, TRAVERSE_TIME);
  JMT jmt_d(car.saved_state_d, targetState_d, TRAVERSE_TIME);
  this->jmtPair.emplace_back(jmt_s);
  this->jmtPair.emplace_back(jmt_d);
}

JMT Trajectory::get_jmt_s() const {
  return jmtPair[0];
}

JMT Trajectory::get_jmt_d() const {
  return jmtPair[1];
}
