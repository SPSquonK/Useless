#pragma once

#include "DPMng.h"
#include "rtmessenger.h"

class CCachePlayer;
class CDPCoreClient : public CDPMng,
	public DPMngFeatures::PacketHandler<CDPCoreClient, DPID>
{
public:
//	Constructions
	CDPCoreClient();

//	Overrides
	virtual	void SysMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );
	virtual void UserMessageHandler( LPDPMSG_GENERIC lpMsg, DWORD dwMsgSize, DPID idFrom );
//	Operations
	void	SendToServer( DPID dpidUser, LPVOID lpMsg, DWORD dwMsgSize );
	void	SendAddPlayer(const CCachePlayer & pPlayer, CRTMessenger & rtmessenger );
	void	SendDestroyPlayer(const CCachePlayer & pPlayer );
// Handlers
private:
	void	OnProcServerList( CAr & ar, DPID );
	void	OnProcServer( CAr & ar, DPID );
	void	OnLeave( CAr & ar, DPID dpid );
	void	OnDestroyAllPlayers( CAr & ar, DPID dpid );
	void	OnJoin( CAr & ar, DPID dpid );
//	Operator commands
	void	OnSay( CAr & ar, DPID dpid );
	void	OnKillPlayer( CAr & ar, DPID dpid );
};

extern CDPCoreClient g_DPCoreClient;
