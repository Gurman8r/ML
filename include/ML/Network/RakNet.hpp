#ifndef _ML_RAKNET_HPP_
#define _ML_RAKNET_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <RakNetTypes.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define ML_PEER		static_cast<RakNet::RakPeerInterface *>
#define ML_PACKET	static_cast<RakNet::Packet *>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RAKNET_HPP_