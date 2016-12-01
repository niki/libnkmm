// -*- mode:c++; coding:utf-8-ws -*-
//====================================================================
//! @file  cs_base64.h
//! @brief BASE64
//!
//! @author 
//====================================================================
#ifndef MX_BASE64_H
#define MX_BASE64_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

namespace mx {
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
}  // namespace mx

//#ifdef	__cplusplus
//}
//#endif

#endif  //MX_BASE64_H
