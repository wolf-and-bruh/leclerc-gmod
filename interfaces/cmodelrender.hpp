#pragma once
struct model_t
{
	int pad;
	char* name;
};

struct ModelRenderInfo_t
{
	Vector origin;
	Angle angles;
	void* pRenderable;
	const model_t* pModel;
	const Matrix3x4* pModelToWorld;
	const Matrix3x4* pLightingOffset;
	const Vector* pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	void* instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};
typedef unsigned short ModelInstanceHandle_t;

struct DrawModelState_t
{
	void* m_pStudioHdr;
	void* m_pStudioHWData;
	void* m_pRenderable;
	const Matrix3x4* m_pModelToWorld;
	void* m_decals;
	int						m_drawFlags;
	int						m_lod;
};

class CModelRender
{
public:
	/*0*/	virtual int		DrawModel(int flags, void* pRenderable, ModelInstanceHandle_t instance, int entity_index, const void* model, Vector const& origin, Angle const& angles, int skin, int body, int hitboxset, const Matrix3x4* modelToWorld = NULL, const Matrix3x4* pLightingOffset = NULL) = 0;
	/*1*/	virtual void	ForcedMaterialOverride(IMaterial* newMaterial, int nOverrideType) = 0;
	/*2*/	virtual void	SetViewTarget(const void* pStudioHdr, int nBodyIndex, const Vector& target) = 0;
	/*3*/	virtual ModelInstanceHandle_t CreateInstance(void* pRenderable, void* pCache = NULL) = 0;
	/*4*/	virtual void DestroyInstance(ModelInstanceHandle_t handle) = 0;
	/*5*/	virtual void SetStaticLighting(ModelInstanceHandle_t handle, void* pHandle) = 0;
	/*6*/	virtual void* GetStaticLighting(ModelInstanceHandle_t handle) = 0;
	/*7*/	virtual bool ChangeInstance(ModelInstanceHandle_t handle, void* pRenderable) = 0;
	/*8*/	virtual void AddDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru = false, int maxLODToDecal = 0) = 0;
	/*9*/	virtual void GMODAddDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, bool noPokeThru = false, int maxLODToDecal = 0) = 0;
	/*1*/	virtual void RemoveAllDecals(ModelInstanceHandle_t handle) = 0;
	/*11*/	virtual void RemoveAllDecalsFromAllModels() = 0;
	/*12*/	virtual Matrix3x4* DrawModelShadowSetup(void* pRenderable, int body, int skin, void* pInfo, Matrix3x4* pCustomBoneToWorld = NULL) = 0;
	/*13*/	virtual void DrawModelShadow(void* pRenderable, const void* info, Matrix3x4* pCustomBoneToWorld = NULL) = 0;
	/*14*/	virtual bool RecomputeStaticLighting(ModelInstanceHandle_t handle) = 0;
	/*15*/	virtual void ReleaseAllStaticPropColorData(void) = 0;
	/*16*/	virtual void RestoreAllStaticPropColorData(void) = 0;
	/*17*/	virtual int	DrawModelEx(ModelRenderInfo_t& pInfo) = 0;
	/*18*/	virtual int	DrawModelExStaticProp(ModelRenderInfo_t& pInfo) = 0;
	/*19*/	virtual bool DrawModelSetup(ModelRenderInfo_t& pInfo, DrawModelState_t* pState, Matrix3x4* pCustomBoneToWorld, Matrix3x4** ppBoneToWorldOut) = 0;
	/*20*/	virtual void DrawModelExecute(const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, Matrix3x4* pCustomBoneToWorld = NULL) = 0;
	/*21*/	virtual void SetupLighting(const Vector& vecCenter) = 0;
	/*22*/	virtual int DrawStaticPropArrayFast(void* pProps, int count, bool bShadowDepth) = 0;
	/*23*/	virtual void SuppressEngineLighting(bool bSuppress) = 0;
	/*24*/	virtual void SetupColorMeshes(int nTotalVerts) = 0;
	/*25*/	virtual void AddColoredDecal(ModelInstanceHandle_t handle, Ray_t const& ray, Vector const& decalUp, int decalIndex, int body, void* cColor, bool noPokeThru = false, int maxLODToDecal = 0) = 0;
};