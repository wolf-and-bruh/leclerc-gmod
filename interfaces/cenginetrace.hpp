#pragma once
#include "../includes.hpp"

#define CONTENTS_EMPTY 0
#define CONTENTS_SOLID 0x1
#define CONTENTS_WINDOW 0x2
#define CONTENTS_AUX 0x4
#define CONTENTS_GRATE 0x8
#define CONTENTS_SLIME 0x10
#define CONTENTS_WATER 0x20
#define CONTENTS_BLOCKLOS 0x40
#define CONTENTS_OPAQUE 0x80
#define CONTENTS_TESTFOGVOLUM 0x100
#define CONTENTS_UNUSED 0x200
#define CONTENTS_BLOCKLIGHT 0x400
#define CONTENTS_TEAM1 0x800
#define CONTENTS_TEAM2 0x1000
#define CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define CONTENTS_MOVEABLE 0x4000
#define CONTENTS_AREAPORTAL 0x8000
#define CONTENTS_PLAYERCLIP 0x10000
#define CONTENTS_MONSTERCLIP 0x20000
#define CONTENTS_CURRENT_0 0x40000
#define CONTENTS_CURRENT_90 0x80000
#define CONTENTS_CURRENT_180 0x100000
#define CONTENTS_CURRENT_270 0x200000
#define CONTENTS_CURRENT_UP 0x400000
#define CONTENTS_CURRENT_DOWN 0x800000
#define CONTENTS_ORIGIN 0x1000000
#define CONTENTS_MONSTER 0x2000000
#define CONTENTS_DEBRIS 0x4000000
#define CONTENTS_DETAIL 0x8000000
#define CONTENTS_TRANSLUCENT 0x10000000
#define CONTENTS_LADDER 0x20000000
#define CONTENTS_HITBOX 0x40000000
#define LAST_VISIBLE_CONTENTS CONTENTS_OPAQUE
#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define SURF_LIGHT 0x0001
#define SURF_SKY2D 0x0002
#define SURF_SKY 0x0004
#define SURF_WARP 0x0008
#define SURF_TRANS 0x0010
#define SURF_NOPORTAL 0x0020
#define SURF_TRIGGER 0x0040
#define SURF_NODRAW 0x0080
#define SURF_HINT 0x0100
#define SURF_SKIP 0x0200
#define SURF_NOLIGHT 0x0400
#define SURF_BUMPLIGHT 0x0800
#define SURF_NOSHADOWS 0x1000
#define SURF_NODECALS 0x2000
#define SURF_NOCHOP 0x4000
#define SURF_HITBOX 0x8000

#define MASK_ALL (0xFFFFFFFF)
#define MASK_SOLID (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_PLAYERSOLID (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_NPCSOLID (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define MASK_NPCFLUID (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_WATER (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define MASK_OPAQUE (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define MASK_OPAQUE_AND_NPCS (MASK_OPAQUE|CONTENTS_MONSTER)
#define MASK_BLOCKLOS (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define MASK_BLOCKLOS_AND_NPCS (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define MASK_VISIBLE (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_VISIBLE_AND_NPCS (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_SHOT (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define MASK_SHOT_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define MASK_SHOT_HULL (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define MASK_SHOT_PORTAL (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_SOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define MASK_PLAYERSOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define MASK_NPCSOLID_BRUSHONLY (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC_FLUID (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define MASK_SPLITAREAPORTAL (CONTENTS_WATER|CONTENTS_SLIME)
#define MASK_CURRENT (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define MASK_DEADSOLID (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

#define  Assert( _exp ) ((void)0)

struct Ray_t {
public:
	VectorAligned m_start;
	VectorAligned m_delta;
	VectorAligned m_start_offset;
	VectorAligned m_extents;
	const Matrix3x4* m_world_axis_transform;
	bool m_is_ray;
	bool m_is_swept;

	Ray_t() : m_world_axis_transform(nullptr), m_is_ray{ false }, m_is_swept{ false } {}

	Ray_t(Vector const& start, Vector const& end) {
		Init(start, end);
	}

	Ray_t(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs) {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.Length() != 0;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.LengthSqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}

	void Init(Vector const& start, Vector const& end) {
		m_delta = end - start;

		m_is_swept = m_delta.LengthSqr() != 0;

		m_extents = { 0.f,0.f,0.f };

		m_world_axis_transform = nullptr;
		m_is_ray = true;

		m_start_offset = { 0.f,0.f,0.f };
		m_start = start;
	}

	void init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs) {
		m_delta = end - start;

		m_world_axis_transform = nullptr;
		m_is_swept = m_delta.Length() != 0;

		m_extents = maxs - mins;
		m_extents *= 0.5f;
		m_is_ray = m_extents.LengthSqr() < 1e-6;

		m_start_offset = maxs + mins;
		m_start_offset *= 0.5f;
		m_start = start + m_start_offset;
		m_start_offset *= -1.0f;
	}
};


struct cplane_t
{
	Vector	normal;
	float	dist;
	unsigned char type;			// for fast side tests
	unsigned char signbits;		// signx + (signy<<1) + (signz<<1)
	unsigned char pad[2];

	cplane_t() {}

private:
	// No copy constructors allowed if we're in optimal mode
	cplane_t(const cplane_t& vOther);
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

class CBaseTrace {
public:
	Vector			startpos;
	Vector			endpos;
	cplane_t		plane;

	float			fraction;

	int				contents;
	unsigned short	dispFlags;

	bool			allsolid;
	bool			startsolid;

	CBaseTrace() {}
};

class trace_t : public CBaseTrace {
public:
	float                   fractionleftsolid;
	csurface_t              surface;
	int                     hitgroup;
	short                   physicsbone;
	unsigned short          worldSurfaceIndex;
	C_BasePlayer* m_pEnt;
	int                     hitbox;

	trace_t() {}

private:
	trace_t(const trace_t& vOther);
};

class CTrace
{
public:
	Vector startPos;
	Vector endPos;
	cplane_t plane;

	float fraction;

	int contents;
	std::uint16_t dispFlags;

	bool allSolid;
	bool startSolid;

	float fractionLeftSolid;
	csurface_t surface;

	int hitgroup;

	short physicsBone;
	std::uint16_t worldSurfaceIndex;

	C_BasePlayer* entity;
	int hitbox;
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter {
public:
	virtual bool ShouldHitEntity(C_BasePlayer* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType_t	GetTraceType() {
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};

class IHandleEntity;
class CTraceListData;
class CPhysCollide;

class IEntityEnumerator
{
public:
	// This gets called with each handle
	virtual bool EnumEntity(IHandleEntity* pHandleEntity) = 0;
};

class TraceFilterSimple {
	void* entity;
	int collision_group;

public:
	TraceFilterSimple(void* ent) {
		entity = ent;
		collision_group = 0;
	}

	virtual bool should_hit_entity(void* entity, int contents_mask) {
		return entity != this->entity;
	}

	virtual int get_trace_type() const {
		return TRACE_EVERYTHING;
	}

	virtual void set_entity(void* ent) {
		entity = ent;
	}

	virtual void set_collision_group(int group) {
		collision_group = group;
	}
};

class IEngineTrace
{
public:
	virtual void unk0() = 0;
	virtual void unk1() = 0;
	virtual void unk2() = 0;
	virtual void unk3() = 0;
	virtual void TraceRay(const Ray_t& ray, unsigned int fMask, TraceFilterSimple* pTraceFilter, CTrace* pTrace) = 0;
};