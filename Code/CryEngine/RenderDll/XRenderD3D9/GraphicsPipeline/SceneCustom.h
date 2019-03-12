// Copyright 2001-2018 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include "Common/GraphicsPipelineStage.h"
#include "Common/GraphicsPipelineStateSet.h"
#include "Common/SceneRenderPass.h"
#include "Common/FullscreenPass.h"
#include "SceneGBuffer.h"

struct SGraphicsPipelineStateDescription;

class CSceneCustomStage : public CGraphicsPipelineStage
{
	enum EPerPassTexture
	{
		ePerPassTexture_PerlinNoiseMap   = CSceneGBufferStage::ePerPassTexture_PerlinNoiseMap,
		ePerPassTexture_TerrainElevMap   = CSceneGBufferStage::ePerPassTexture_TerrainElevMap,
		ePerPassTexture_WindGrid         = CSceneGBufferStage::ePerPassTexture_WindGrid,
		ePerPassTexture_TerrainNormMap   = CSceneGBufferStage::ePerPassTexture_TerrainNormMap,
		ePerPassTexture_TerrainBaseMap   = CSceneGBufferStage::ePerPassTexture_TerrainBaseMap,
		ePerPassTexture_NormalsFitting   = CSceneGBufferStage::ePerPassTexture_NormalsFitting,
		ePerPassTexture_DissolveNoise    = CSceneGBufferStage::ePerPassTexture_DissolveNoise,
		ePerPassTexture_SceneLinearDepth = CSceneGBufferStage::ePerPassTexture_SceneLinearDepth,

		ePerPassTexture_PaletteTexelsPerMeter = 33,
	};
	
public:
	enum EPass
	{
		ePass_DebugViewSolid = 0,
		ePass_DebugViewWireframe,
		ePass_DebugViewDrawModes,
		ePass_SelectionIDs, // draw highlighted objects from editor
		ePass_Silhouette,
	};

public:
	CSceneCustomStage();

	static bool DoDebugRendering();
	static bool DoDebugOverlay();

	void Init() final;
	void Update() final;
	void Prepare();

	bool IsStageActive(EShaderRenderingFlags flags) const final
	{
		if (flags & EShaderRenderingFlags::SHDF_FORWARD_MINIMAL)
			return false;

		return true;
	}

	void Execute();
	void ExecuteSilhouettePass();
	void ExecuteHelpers();
	void ExecuteDebugger();
	void ExecuteDebugOverlay();
	void ExecuteSelectionHighlight();

	bool CreatePipelineStates(DevicePipelineStatesArray* pStateArray, const SGraphicsPipelineStateDescription& stateDesc, CGraphicsPipelineStateLocalCache* pStateCache);
	bool CreatePipelineState(const SGraphicsPipelineStateDescription& desc, EPass passID, CDeviceGraphicsPSOPtr& outPSO);

private:
	bool SetAndBuildPerPassResources(bool bOnInit);

private:
	CDeviceResourceSetDesc   m_perPassResources;
	CDeviceResourceSetPtr    m_pPerPassResourceSet;
	CDeviceResourceLayoutPtr m_pResourceLayout;
	CConstantBufferPtr       m_pPerPassConstantBuffer;
	
	CSceneRenderPass         m_debugViewPass;
	CSceneRenderPass         m_selectionIDPass;
	CFullscreenPass          m_highlightPass;

	CSceneRenderPass         m_silhouetteMaskPass;
};
