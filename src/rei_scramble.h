// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_scramble.h
//! @brief Scramble
//!
//! @author
//====================================================================
#ifndef REI_SCRAMBLE_H
#define REI_SCRAMBLE_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

namespace rei {
namespace scramble {

void Encode(char *data, int size, const char *sugar);
void Decode(char *data, int size, const char *sugar);

}  //scramble
}  //rei

//#ifdef	__cplusplus
//}
//#endif

#endif  //REI_SCRAMBLE_H
