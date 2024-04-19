// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_BASE64_H
#define NKMM_BASE64_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

namespace nkmm {
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
}  // namespace nkmm

//#ifdef	__cplusplus
//}
//#endif

#endif  //NKMM_BASE64_H
