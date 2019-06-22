#ifndef _ML_RAKNET_HPP_
#define _ML_RAKNET_HPP_

/* * * * * * * * * * * * * * * * * * * * */

#include <RakNet/RakPeerInterface.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/BitStream.h>
#include <RakNet/RakNetTypes.h>

/* * * * * * * * * * * * * * * * * * * * */

#define ML_PEER		static_cast<RakNet::RakPeerInterface *>
#define ML_PACKET	static_cast<RakNet::Packet *>

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_RAKNET_HPP_