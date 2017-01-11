// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  rei_base64.h
//! @brief BASE64
//!
//! @author
//====================================================================
#ifndef REI_BASE64_H
#define REI_BASE64_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

namespace rei {
namespace base64 {

//------------------------------------------------------------------
//! デコード
//------------------------------------------------------------------
unsigned char* Decode(unsigned char* src, unsigned char* dest, int size);

//------------------------------------------------------------------
//! エンコード
//------------------------------------------------------------------
unsigned char* Encode(unsigned char* src, unsigned char* dest, int size);

}  // namespace base64
}  // namespace rei

//#ifdef	__cplusplus
//}
//#endif

#endif  //REI_BASE64_H
