#include"dx9hook.h"
DWORD_PTR* pVTable = NULL;
typedef HRESULT(__stdcall* f_DrawIndexedPrimitive)(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
f_DrawIndexedPrimitive Original_DrawIndexedPrimitive;
HRESULT __stdcall  DrawIndexedPrimitiveHook(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	IDirect3DVertexBuffer9* pStreamData = NULL;
	UINT ioffsetInBytes, iStride;
	if (pDevice->GetStreamSource(0, &pStreamData, &ioffsetInBytes, &iStride) == D3D_OK)
	{
		pStreamData->Release();//NumVerticesd
		DWORD Value = 0;
			if (iStride == 36 || iStride == 40 ||
				iStride == 44 || iStride == 48 ||
				iStride == 68 || iStride == 72)
			{
				pDevice->GetRenderState(D3DRS_ZENABLE, &Value);
				pDevice->SetRenderState((D3DRENDERSTATETYPE)7, 0);
				pDevice->SetRenderState((D3DRENDERSTATETYPE)137, 2);
				pDevice->SetRenderState((D3DRENDERSTATETYPE)20, 2);
				pDevice->SetRenderState((D3DRENDERSTATETYPE)27, 3);
			}
	}
	return Original_DrawIndexedPrimitive(pDevice, type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

bool dx9_Initialize() {
	auto Window = FindWindowA("CrossFire", NULL);
	// 初始化D3D
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d9 == NULL) {
		MessageBox(NULL, L"创建D3D失败", L"", MB_OK);
		return false;
	}
	D3DPRESENT_PARAMETERS pp = {};
	pp.Windowed = TRUE;
	pp.SwapEffect = D3DSWAPEFFECT_COPY;
	IDirect3DDevice9* device;
	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &device))) {
		MessageBox(NULL, L"创建设备失败", L"", MB_OK);
		d3d9->Release();
		return false;
	}
	pVTable = (DWORD_PTR*)device;
	pVTable = (DWORD_PTR*)pVTable[0];
	auto DrawIndexedPrimitive = (f_DrawIndexedPrimitive)pVTable[82];
	MH_Initialize();
	MH_CreateHook(DrawIndexedPrimitive, DrawIndexedPrimitiveHook, (PVOID*)(&Original_DrawIndexedPrimitive));
	MH_EnableHook(MH_ALL_HOOKS);
	// 释放
	d3d9->Release();
	device->Release();
	return true;
}
