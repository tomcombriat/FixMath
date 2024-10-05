#ifndef FIXMATH_MAPPER_H
#define FIXMATH_MAPPER_H

#include "FixMath.h"

//enum precision { FAST, ACCURATE, FULL };

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


  /**
Map an input to the output range
@param in the input
  */
 out_type map(in_type in) const
  {
    return /*out_type*/(/*in_type*/(in - in_min) * coef) + out_min;
     //return out_type(0);
  }


 private:
 
  decltype((out_type(1)-out_type(0)) * (in_type(1)-in_type(0)).invFull()) coef=0;
  in_type in_min, in_max;
  out_type out_min, out_max;

  void compute_coef()
  {
    coef = (out_max-out_min)*(in_max-in_min).invFull();
  }
  
};





#endif
