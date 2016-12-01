//-----------------------------------------------------------------------------
/*!
* @file		mx_scramble.h
* @brief	Scramble
* @author	koma
* @version	$Id:$
*
* @note
* ÅEçXêVóöó  
*/
//-----------------------------------------------------------------------------
#ifndef MX_SCRAMBLE_H
#define MX_SCRAMBLE_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

namespace mx {
namespace scramble {

void Encode(char *data, int size, const char *sugar);
void Decode(char *data, int size, const char *sugar);

}  //scramble
}  //mx

//#ifdef	__cplusplus
//}
//#endif

#endif  //MX_SCRAMBLE_H
