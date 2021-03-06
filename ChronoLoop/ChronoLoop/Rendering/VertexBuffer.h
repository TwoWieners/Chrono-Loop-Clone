#pragma once
#include <map>
#include <string>
#include <d3d11.h>
#include "renderer.h"
#include <wrl/client.h>
#include "../Common/Common.h"

namespace Epoch {
	template<typename T>
	class VertexBuffer {
	private:
		std::map<std::string, unsigned int> mOffsets;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
	public:
		VertexBuffer();
		~VertexBuffer();
		unsigned int AddVerts(std::string _name, const T *_verts, unsigned int _numVerts);
		Microsoft::WRL::ComPtr<ID3D11Buffer> const GetBuffer();
	};

	template<typename T>
	VertexBuffer<T>::VertexBuffer() {
		mVertexBuffer = nullptr;
	}

	template<typename T>
	VertexBuffer<T>::~VertexBuffer() {}

	template<typename T>
	unsigned int VertexBuffer<T>::AddVerts(std::string _name, const T * _verts, unsigned int _numVerts) {
		Renderer::Instance()->GetRendererLock().lock();
		unsigned int offset = 0;
		if (mOffsets.count(_name) > 0)
			offset = mOffsets.at(_name);
		else {
			if (mVertexBuffer) {
				D3D11_BUFFER_DESC desc;
				mVertexBuffer->GetDesc(&desc);
				D3D11_SUBRESOURCE_DATA initData;
				unsigned int oldSize = desc.ByteWidth / sizeof(T);
				unsigned int newSize = oldSize + _numVerts;
				offset = oldSize;
				initData.pSysMem = new T[newSize];
				memcpy((char *)(initData.pSysMem) + desc.ByteWidth, _verts, sizeof(T) * _numVerts);
				desc.ByteWidth += sizeof(T) * _numVerts;
				ID3D11Buffer *newBuffer;
				Renderer::Instance()->GetDevice()->CreateBuffer(&desc, &initData, &newBuffer);
				Renderer::Instance()->GetContext()->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, mVertexBuffer.Get(), 0, 0);
				mVertexBuffer.Attach(newBuffer);
				std::string name = "The Vertex Buffer, mk" + std::to_string(mOffsets.size());
				SetD3DName(mVertexBuffer.Get(), name.c_str());
				delete[] initData.pSysMem;
			} else {
				D3D11_BUFFER_DESC desc;
				desc.Usage = D3D11_USAGE_DEFAULT;
				desc.ByteWidth = sizeof(T) * _numVerts;
				desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				desc.CPUAccessFlags = 0;
				desc.MiscFlags = 0;
				desc.StructureByteStride = 0;

				D3D11_SUBRESOURCE_DATA initData;
				initData.pSysMem = _verts;
				Renderer::Instance()->GetDevice()->CreateBuffer(&desc, &initData, mVertexBuffer.GetAddressOf());
				std::string name = "The Vertex Buffer";
				SetD3DName(mVertexBuffer.Get(), name.c_str());
			}
			mOffsets[_name] = offset;
		}
		Renderer::Instance()->GetRendererLock().unlock();
		return offset;
	}
	template<typename T>
	Microsoft::WRL::ComPtr<ID3D11Buffer> const VertexBuffer<T>::GetBuffer() {
		return mVertexBuffer;
	}
}
