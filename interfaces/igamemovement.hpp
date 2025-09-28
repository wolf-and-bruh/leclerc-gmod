#pragma once
class CGameMovement
{
public:
	virtual			~CGameMovement(void) {}

	// Process the current movement command
	virtual void	ProcessMovement(C_BasePlayer* pPlayer, CMoveData* pMove) = 0;
	virtual void	StartTrackPredictionErrors(C_BasePlayer* pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(C_BasePlayer* pPlayer) = 0;
	virtual void	DiffPrint(char const* fmt, ...) = 0;

	// Allows other parts of the engine to find out the normal and ducked player bbox sizes
	virtual Vector	GetPlayerMins(bool ducked) const = 0;
	virtual Vector	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector  GetPlayerViewOffset(bool ducked) const = 0;
	virtual void	TracePlayerBBox(const Vector& start, const Vector& end, unsigned int fMask, int collisionGroup, trace_t& pm);
	virtual void	TryTouchGround(const Vector& start, const Vector& end, const Vector& mins, const Vector& maxs, unsigned int fMask, int collisionGroup, trace_t& pm);


	CMoveData* mv;
	int				m_nOldWaterLevel;
	float			m_flWaterEntryTime;
	int				m_nOnLadder;
	Vector			m_vecForward;
	Vector			m_vecRight;
	Vector			m_vecUp;

	// Does most of the player movement logic.
// Returns with origin, angles, and velocity modified in place.
// were contacted during the move.
	virtual void	UNK0(void);
	virtual void	UNK1(void);
	virtual void	UNK2(void);
	virtual void	PlayerMove(void);
	virtual float	CalcRoll(const Angle& angles, const Vector& velocity, float rollangle, float rollspeed);

	virtual	void	DecayPunchAngle(void);

	virtual void	CheckWaterJump(void);

	virtual void	WaterMove(void);

	virtual void	AirAccelerate(Vector& wishdir, float wishspeed, float accel);

	virtual void	AirMove(void);
	virtual float	GetAirSpeedCap(void) { return 30.f; }

	virtual bool	CanAccelerate();
	virtual void	Accelerate(Vector& wishdir, float wishspeed, float accel);

	// Only used by players.  Moves along the ground when player is a MOVETYPE_WALK.
	virtual void	WalkMove(void);

	// Handle MOVETYPE_WALK.
	virtual void	FullWalkMove();


};