//========= Copyright � 1996-2006, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef C_OBJ_SENTRYGUN_H
#define C_OBJ_SENTRYGUN_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseobject.h"
#include "ObjectControlPanel.h"
#include "c_tf_projectile_rocket.h"
#include "c_tf_player.h"

class C_MuzzleFlashModel;

//-----------------------------------------------------------------------------
// Purpose: Sentry object
//-----------------------------------------------------------------------------
class C_ObjectSentrygun : public C_BaseObject
{
	DECLARE_CLASS( C_ObjectSentrygun, C_BaseObject );
public:
	DECLARE_CLIENTCLASS();

	C_ObjectSentrygun();
	~C_ObjectSentrygun();

	void GetAmmoCount( int &iShells, int &iMaxShells, int &iRockets, int & iMaxRockets );

	void GetStatusText( wchar_t *pStatus, int iMaxStatusLen );

	virtual bool	IsUpgrading( void ) const;

	virtual void GetTargetIDString( wchar_t *sIDString, int iMaxLenInBytes );

	virtual BuildingHudAlert_t GetBuildingAlertLevel( void );

	virtual const char *GetHudStatusIcon( void );

	int GetKills( void ) { return m_iKills; }
	int GetAssists( void ) { return m_iAssists; }
	int GetState( void ) { return m_iState; }


	virtual void GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType );

	virtual CStudioHdr *OnNewModel( void );
	virtual void UpdateDamageEffects( BuildingDamageLevel_t damageLevel );

	virtual void OnPlacementStateChanged( bool bValidPlacement );

	void DebugDamageParticles();

	virtual void	OnPreDataChanged( DataUpdateType_t updateType );
	virtual void	OnDataChanged( DataUpdateType_t updateType );


	// Laser methods
	CNewParticleEffect	*CreateLaserBeam( void ) 
	{
		SetNextClientThink( gpGlobals->curtime + 0.25f );

		// Don't bother creating a new shield if one already exists
		if ( !m_pShield )
		{
			m_pShield = new C_BaseAnimating();
			m_pShield->SetModel( "models/buildables/sentry_shield.mdl");
		
			if ( m_iUpgradeLevel == 3)
			{
				// Slight offset for lvl 3 sentry
				m_pShield->SetAbsOrigin( GetAbsOrigin() + Vector( 0, 0, 4 ) );
			}
			else
			{
				m_pShield->SetAbsOrigin( GetAbsOrigin() );
			}
		}

		switch ( GetTeamNumber() )
		{
			case TF_TEAM_RED:
				m_pShield->m_nSkin = 0;
				break;
			case TF_TEAM_BLUE:
				m_pShield->m_nSkin = 1;
				break;
		}

		return m_pLaserBeam = ParticleProp()->Create( "laser_sight_beam", PATTACH_POINT_FOLLOW, "laser_origin" ); 
	}

	void DestroyLaserBeam( void ) 
	{
		ParticleProp()->StopEmissionAndDestroyImmediately( m_pLaserBeam );
		m_pLaserBeam = NULL;

		// set time for shield removal
		SetNextClientThink( gpGlobals->curtime + 3.0f );
	}

	void DestroyShield( void )
	{
		m_pShield->Remove();
		m_pShield = NULL;
	}

	virtual void ClientThink( void ) 
	{
		if ( !GetBuilder()->IsAlive() )
		{
			DestroyLaserBeam();
			return;
		}

		if ( !m_pLaserBeam && m_pShield )
		{
			DestroyShield();
			SetNextClientThink( CLIENT_THINK_NEVER );
			return;
		}

		// Check for player death four times every second
		SetNextClientThink( gpGlobals->curtime + 0.25f );
	}


	// ITargetIDProvidesHint
public:
	virtual void	DisplayHintTo( C_BasePlayer *pPlayer );

private:

	void UpgradeLevelChanged();

private:
	int m_iState;

	int m_iAmmoShells;
	int m_iMaxAmmoShells;
	int m_iAmmoRockets;

	int m_iKills;
	int m_iAssists;

	C_BaseAnimating	   *m_pShield;
	CNewParticleEffect *m_pDamageEffects;
	CNewParticleEffect *m_pLaserBeam;

	int m_iPlacementBodygroup;

	int m_iOldBodygroups;

private:
	C_ObjectSentrygun( const C_ObjectSentrygun & ); // not defined, not accessible
};

class C_TFProjectile_SentryRocket : public C_TFProjectile_Rocket
{
	DECLARE_CLASS( C_TFProjectile_SentryRocket, C_TFProjectile_Rocket );
public:
	DECLARE_CLIENTCLASS();

	virtual void CreateRocketTrails( void ) {}
};

#endif	//C_OBJ_SENTRYGUN_H