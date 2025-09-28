#pragma once

enum ImageFormat
{
	IMAGE_FORMAT_UNKNOWN = -1,
	IMAGE_FORMAT_RGBA8888 = 0,
	IMAGE_FORMAT_ABGR8888,
	IMAGE_FORMAT_RGB888,
	IMAGE_FORMAT_BGR888,
	IMAGE_FORMAT_RGB565,
	IMAGE_FORMAT_I8,
	IMAGE_FORMAT_IA88,
	IMAGE_FORMAT_P8,
	IMAGE_FORMAT_A8,
	IMAGE_FORMAT_RGB888_BLUESCREEN,
	IMAGE_FORMAT_BGR888_BLUESCREEN,
	IMAGE_FORMAT_ARGB8888,
	IMAGE_FORMAT_BGRA8888,
	IMAGE_FORMAT_DXT1,
	IMAGE_FORMAT_DXT3,
	IMAGE_FORMAT_DXT5,
	IMAGE_FORMAT_BGRX8888,
	IMAGE_FORMAT_BGR565,
	IMAGE_FORMAT_BGRX5551,
	IMAGE_FORMAT_BGRA4444,
	IMAGE_FORMAT_DXT1_ONEBITALPHA,
	IMAGE_FORMAT_BGRA5551,
	IMAGE_FORMAT_UV88,
	IMAGE_FORMAT_UVWQ8888,
	IMAGE_FORMAT_RGBA16161616F,
	IMAGE_FORMAT_RGBA16161616,
	IMAGE_FORMAT_UVLX8888,
	IMAGE_FORMAT_R32F,			// Single-channel 32-bit floating point
	IMAGE_FORMAT_RGB323232F,
	IMAGE_FORMAT_RGBA32323232F,

	// Depth-stencil texture formats for shadow depth mapping
	IMAGE_FORMAT_NV_DST16,		// 
	IMAGE_FORMAT_NV_DST24,		//
	IMAGE_FORMAT_NV_INTZ,		// Vendor-specific depth-stencil texture
	IMAGE_FORMAT_NV_RAWZ,		// formats for shadow depth mapping 
	IMAGE_FORMAT_ATI_DST16,		// 
	IMAGE_FORMAT_ATI_DST24,		//
	IMAGE_FORMAT_NV_NULL,		// Dummy format which takes no video memory

	// Compressed normal map formats
	IMAGE_FORMAT_ATI2N,			// One-surface ATI2N / DXN format
	IMAGE_FORMAT_ATI1N,			// Two-surface ATI1N format

#if defined( _X360 )
	// Depth-stencil texture formats
	IMAGE_FORMAT_X360_DST16,
	IMAGE_FORMAT_X360_DST24,
	IMAGE_FORMAT_X360_DST24F,
	// supporting these specific formats as non-tiled for procedural cpu access
	IMAGE_FORMAT_LINEAR_BGRX8888,
	IMAGE_FORMAT_LINEAR_RGBA8888,
	IMAGE_FORMAT_LINEAR_ABGR8888,
	IMAGE_FORMAT_LINEAR_ARGB8888,
	IMAGE_FORMAT_LINEAR_BGRA8888,
	IMAGE_FORMAT_LINEAR_RGB888,
	IMAGE_FORMAT_LINEAR_BGR888,
	IMAGE_FORMAT_LINEAR_BGRX5551,
	IMAGE_FORMAT_LINEAR_I8,
	IMAGE_FORMAT_LINEAR_RGBA16161616,

	IMAGE_FORMAT_LE_BGRX8888,
	IMAGE_FORMAT_LE_BGRA8888,
#endif

	IMAGE_FORMAT_DXT1_RUNTIME,
	IMAGE_FORMAT_DXT5_RUNTIME,

	NUM_IMAGE_FORMATS
};

#if defined( POSIX  ) || defined( DX_TO_GL_ABSTRACTION )
typedef enum _D3DFORMAT
{
	D3DFMT_INDEX16,
	D3DFMT_D16,
	D3DFMT_D24S8,
	D3DFMT_A8R8G8B8,
	D3DFMT_A4R4G4B4,
	D3DFMT_X8R8G8B8,
	D3DFMT_R5G6R5,
	D3DFMT_X1R5G5B5,
	D3DFMT_A1R5G5B5,
	D3DFMT_L8,
	D3DFMT_A8L8,
	D3DFMT_A,
	D3DFMT_DXT1,
	D3DFMT_DXT3,
	D3DFMT_DXT5,
	D3DFMT_V8U8,
	D3DFMT_Q8W8V8U8,
	D3DFMT_X8L8V8U8,
	D3DFMT_A16B16G16R16F,
	D3DFMT_A16B16G16R16,
	D3DFMT_R32F,
	D3DFMT_A32B32G32R32F,
	D3DFMT_R8G8B8,
	D3DFMT_D24X4S4,
	D3DFMT_A8,
	D3DFMT_R5G6B5,
	D3DFMT_D15S1,
	D3DFMT_D24X8,
	D3DFMT_VERTEXDATA,
	D3DFMT_INDEX32,

	// adding fake D3D format names for the vendor specific ones (eases debugging/logging)

	// NV shadow depth tex
	D3DFMT_NV_INTZ = 0x5a544e49,	// MAKEFOURCC('I','N','T','Z')
	D3DFMT_NV_RAWZ = 0x5a574152,	// MAKEFOURCC('R','A','W','Z')

	// NV null tex
	D3DFMT_NV_NULL = 0x4c4c554e,	// MAKEFOURCC('N','U','L','L')

	// ATI shadow depth tex
	D3DFMT_ATI_D16 = 0x36314644,	// MAKEFOURCC('D','F','1','6')
	D3DFMT_ATI_D24S8 = 0x34324644,	// MAKEFOURCC('D','F','2','4')

	// ATI 1N and 2N compressed tex
	D3DFMT_ATI_2N = 0x32495441,	// MAKEFOURCC('A', 'T', 'I', '2')
	D3DFMT_ATI_1N = 0x31495441,	// MAKEFOURCC('A', 'T', 'I', '1')

	D3DFMT_UNKNOWN
} D3DFORMAT;
#endif

class IMatRenderContext
{
public:
	virtual void				BeginRender() = 0;
	virtual void				EndRender() = 0;

	virtual void				Flush(bool flushHardware = false) = 0;

	virtual void				BindLocalCubemap(void* pTexture) = 0;

	// pass in an ITexture (that is build with "rendertarget" "1") or
	// pass in NULL for the regular backbuffer.
	virtual void				SetRenderTarget(void* pTexture) = 0;
	virtual void* GetRenderTarget(void) = 0;

	virtual void				GetRenderTargetDimensions(int& width, int& height) const = 0;

	// Bind a material is current for rendering.
	virtual void				Bind(void* material, void* proxyData = 0) = 0;
	// Bind a lightmap page current for rendering.  You only have to 
	// do this for materials that require lightmaps.
	virtual void				BindLightmapPage(int lightmapPageID) = 0;

	// inputs are between 0 and 1
	virtual void				DepthRange(float zNear, float zFar) = 0;

	virtual void				ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0;

	// read to a unsigned char rgb image.
	virtual void				ReadPixels(int x, int y, int width, int height, unsigned char* data, void* dstFormat) = 0;
};