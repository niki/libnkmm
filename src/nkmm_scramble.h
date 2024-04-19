// -*- mode:c++; coding:utf-8-ws -*-
#ifndef NKMM_SCRAMBLE_H
#define NKMM_SCRAMBLE_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

namespace nkmm {
namespace scramble {

void Encode(char *data, int size, const char *sugar);
void Decode(char *data, int size, const char *sugar);

}  //scramble
}  //nkmm

//#ifdef	__cplusplus
//}
//#endif

#endif  //NKMM_SCRAMBLE_H
