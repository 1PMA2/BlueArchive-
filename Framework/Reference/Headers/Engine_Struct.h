#pragma once

namespace Engine
{
	enum MOUSEBUTTON { MBS_LBUTTON, MBS_RBUTTON, MBS_WHEEL, MBS_X, MBS_END };
	enum MOUSEMOVE { MMS_X, MMS_Y, MMS_WHEEL, MMS_END };

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vPosition;
		float			fTime;
	} KEYFRAME;

	typedef struct tagLightDesc
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE			eType = TYPE_END;

		XMFLOAT4		vDirection;

		XMFLOAT4		vPosition;
		float			fRange;

		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;

	}LIGHTDESC;

	typedef struct tagMaterialDesc
	{
		XMFLOAT4		vDiffuse;
		XMFLOAT4		vAmbient;
		XMFLOAT4		vSpecular;
	}MTRLDESC;

	typedef struct tagLineIndices32
	{
		unsigned long _1, _2;
	}LINEINDICES32;

	typedef struct tagLineIndices16
	{
		unsigned short _1, _2;
	}LINEINDICES16;

	typedef struct tagFaceIndices32
	{
		unsigned long _1, _2, _3;
	}FACEINDICES32;

	typedef struct tagFaceIndices16
	{
		unsigned short _1, _2, _3;
	}FACEINDICES16;

	typedef struct tagModelMaterial
	{
		class CTexture*		pTextures[AI_TEXTURE_TYPE_MAX];
	}MODEL_MATERIAL;

	typedef struct tagVertex_Normal_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
	}VTXNORTEX;

	typedef struct ENGINE_DLL tagVertex_Normal_Texture_Declaration
	{
		static const unsigned int		iNumElements = 3;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXNORTEX_DECLARATION;


	typedef struct tagVertex_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vTexUV;
	}VTXTEX;

	typedef struct ENGINE_DLL tagVertex_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_DECLARATION;

	typedef struct tagVertex_Point
	{
		XMFLOAT3		vPosition;
		XMFLOAT2		vPSize;
	}VTXPOINT;

	typedef struct tagVertex_Instance
	{
		XMFLOAT4			vRight;
		XMFLOAT4			vUp;
		XMFLOAT4			vLook;
		XMFLOAT4			vTranslation;
	}VTXINSTANCE;

	typedef struct ENGINE_DLL tagVertex_Point_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	}VTXPOINT_INSTANCE_DECLARATION;

	typedef struct ENGINE_DLL tagVertex_Texture_Instance_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXTEX_INSTANCE_DECLARATION;



	typedef struct tagVertex_Position
	{
		XMFLOAT3		vPosition;
	}VTXPOS;

	typedef struct ENGINE_DLL tagVertex_Position_Declaration
	{
		static const unsigned int		iNumElements = 1;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXPOS_DECLARATION;




	typedef struct tagVertex_Model
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
	}VTXMODEL;

	typedef struct ENGINE_DLL tagtagVertex_Model_Declaration
	{
		static const unsigned int		iNumElements = 4;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXMODEL_DECLARATION;

	typedef struct tagVertex_AnimModel
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexUV;
		XMFLOAT3		vTangent;
		XMUINT4			vBlendIndex;
		XMFLOAT4		vBlendWeight;
	}VTXANIM;

	typedef struct ENGINE_DLL tagVertex_AnimModel_Declaration
	{
		static const unsigned int		iNumElements = 6;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXANIM_DECLARATION;



	typedef struct tagVertex_Cube_Texture
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vTexUV;
	}VTXCUBETEX;

	typedef struct ENGINE_DLL tagVertex_Cube_Texture_Declaration
	{
		static const unsigned int		iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC	Element[iNumElements];
	} VTXCUBETEX_DECLARATION;

	typedef struct tagGraphicDesc
	{
		enum WINMODE { MODE_FULL, MODE_WIN, MODE_END };

		unsigned int iWinCX, iWinCY;
		HWND	hWnd;
		WINMODE	isWindowMode;
	}GRAPHICDESC;
}

enum CHANNELID
{
	BGM,
	CH_PLAYER_0,
	CH_PLAYER_1,
	CH_PLAYER_2,
	CH_PLAYER_3,
	CH_PLAYER_4,
	CH6,
	CH7,
	CH8,
	CH9,
	CH10,
	CH11,
	CH12,
	CH13,
	CH14,
	CH15,
	CH16,
	CH17,
	CH18,
	CH19,
	CH20,
	CH21,
	CH22,
	CH23,
	CH24,
	CH25,
	CH26,
	CH27,
	CH28,
	CH29,
	CH30,
	CH_END,
};
