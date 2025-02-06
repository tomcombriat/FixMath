#ifndef FIXMATH_MAPPER_H
#define FIXMATH_MAPPER_H

#include "FixMath.h"


/**
   Abstract class to help refractoring the code of the different Mappers
*/
template<typename in_type, typename out_type>
class FixMathMapper
{
public:
  /** Constructor
   */
  FixMathMapper() {;}

  /**
     Constructor
     @param _in_min the lower bound of the input
     @param _in_max the higher bound of the input
     @param _out_min the lower bound of the output
     @param _out_max the higher bound of the output
     @note should work if they are not in increasing order but more testing needed to confirm
     @note _in_min will be mapped to _out_min and _in_max to _out_max
  */
  FixMathMapper(in_type _in_min, in_type _in_max, out_type _out_min, out_type _out_max) {
    setBounds(_in_min,_in_max,_out_min,_out_max);    
  }

  /**
     Set the bounds of the mapper
     @param _in_min the lower bound of the input
     @param _in_max the higher bound of the input
     @param _out_min the lower bound of the output
     @param _out_max the higher bound of the output
     @note should work if they are not in increasing order but more testing needed to confirm
     @note _in_min will be mapped to _out_min and _in_max to _out_max
  */
  void setBounds(in_type _in_min, in_type _in_max, out_type _out_min, out_type _out_max)
  {
    in_min = _in_min;
    in_max = _in_max;
    out_min = _out_min;
    out_max = _out_max;
    compute_coef();
  }

protected:
  in_type in_min, in_max;
  out_type out_min, out_max;
  virtual void compute_coef() = 0;
};



/**
   A mapper, intended to work as Arduino's map, but faster as division is involved only when setBounds() is called, as custom-made for FixMath's types.
   This version uses invFull() to perform the calculations. The biggest precision available, at the cost of some speed (platform and use-case dependent)
*/
template<typename in_type, typename out_type>
class FixMathMapperFull: public FixMathMapper<in_type, out_type>
{

public:
  /** Constructor
   */
  FixMathMapperFull() {;}
  /**
     Constructor
     @param _in_min the lower bound of the input
     @param _in_max the higher bound of the input
     @param _out_min the lower bound of the output
     @param _out_max the higher bound of the output
     @note should work if they are not in increasing order but more testing needed to confirm
     @note _in_min will be mapped to _out_min and _in_max to _out_max
  */
   FixMathMapperFull(in_type _in_min, in_type _in_max, out_type _out_min, out_type _out_max) {
    setBounds(_in_min,_in_max,_out_min,_out_max);    
  }

  /**
     Map an input to the output range
     @param in the input
  */
  out_type map(in_type in) const
  {
    return ((in -this->in_min) * coef) + this->out_min;
  }

private:
  decltype((out_type(1)-out_type(0)) * (in_type(1)-in_type(0)).invFull()) coef=0;
  void compute_coef()  { coef = (this->out_max-this->out_min)*(this->in_max-this->in_min).invFull();} 
};


/**
   A mapper, intended to work as Arduino's map, but faster as division is involved only when setBounds() is called, as custom-made for FixMath's types.
   This version uses invAccurate() to perform the calculations. The nearly biggest precision available, at the cost of some speed (platform and use-case dependent)
*/
template<typename in_type, typename out_type>
class FixMathMapperAccurate: public FixMathMapper<in_type, out_type>
{

public:
  /** Constructor
   */
  FixMathMapperAccurate() {;}
  /**
     Constructor
     @param _in_min the lower bound of the input
     @param _in_max the higher bound of the input
     @param _out_min the lower bound of the output
     @param _out_max the higher bound of the output
     @note should work if they are not in increasing order but more testing needed to confirm
     @note _in_min will be mapped to _out_min and _in_max to _out_max
  */
   FixMathMapperAccurate(in_type _in_min, in_type _in_max, out_type _out_min, out_type _out_max) {
    setBounds(_in_min,_in_max,_out_min,_out_max);    
  }

  /**
     Map an input to the output range
     @param in the input
  */
  out_type map(in_type in) const
  {
    return ((in -this->in_min) * coef) + this->out_min;
  }

private:
  decltype((out_type(1)-out_type(0)) * (in_type(1)-in_type(0)).invFull()) coef=0;
  void compute_coef()  { coef = (this->out_max-this->out_min)*(this->in_max-this->in_min).invAccurate();} 
};


/**
   A mapper, intended to work as Arduino's map, but faster as division is involved only when setBounds() is called, as custom-made for FixMath's types.
   This version uses invFast() to perform the calculations. The biggest speed available to represent the whole target range, at the cost of some precision
*/
template<typename in_type, typename out_type>
class FixMathMapperFast: public FixMathMapper<in_type, out_type>
{

public:
  /** Constructor
   */
  FixMathMapperFast() {;}
  /**
     Constructor
     @param _in_min the lower bound of the input
     @param _in_max the higher bound of the input
     @param _out_min the lower bound of the output
     @param _out_max the higher bound of the output
     @note should work if they are not in increasing order but more testing needed to confirm
     @note _in_min will be mapped to _out_min and _in_max to _out_max
  */
   FixMathMapperFast(in_type _in_min, in_type _in_max, out_type _out_min, out_type _out_max) {
    setBounds(_in_min,_in_max,_out_min,_out_max);    
  }

  /**
     Map an input to the output range
     @param in the input
  */
  out_type map(in_type in) const
  {
    return ((in -this->in_min) * coef) + this->out_min;
  }

private:
  decltype((out_type(1)-out_type(0)) * (in_type(1)-in_type(0)).invFull()) coef=0;
  void compute_coef()  { coef = (this->out_max-this->out_min)*(this->in_max-this->in_min).invFast();} 
};





#endif
