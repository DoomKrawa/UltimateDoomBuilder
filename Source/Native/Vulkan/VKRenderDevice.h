/*
**  BuilderNative Renderer
**  Copyright (c) 2019 Magnus Norddahl
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include "../Backend.h"
#include "System/VulkanDevice.h"
#include <list>

class VKSharedVertexBuffer;
class VKShader;
class VKShaderManager;
class VKVertexBuffer;
class VKIndexBuffer;
class VKTexture;

class VKRenderDevice : public RenderDevice
{
public:
	VKRenderDevice(void* disp, void* window);
	~VKRenderDevice();

	void DeclareUniform(UniformName name, const char* glslname, UniformType type) override;
	void DeclareShader(ShaderName index, const char* name, const char* vertexshader, const char* fragmentshader) override;
	void SetShader(ShaderName name) override;
	void SetUniform(UniformName name, const void* values, int count, int bytesize) override;
	void SetVertexBuffer(VertexBuffer* buffer) override;
	void SetIndexBuffer(IndexBuffer* buffer) override;
	void SetAlphaBlendEnable(bool value) override;
	void SetAlphaTestEnable(bool value) override;
	void SetCullMode(Cull mode) override;
	void SetBlendOperation(BlendOperation op) override;
	void SetSourceBlend(Blend blend) override;
	void SetDestinationBlend(Blend blend) override;
	void SetFillMode(FillMode mode) override;
	void SetMultisampleAntialias(bool value) override;
	void SetZEnable(bool value) override;
	void SetZWriteEnable(bool value) override;
	void SetTexture(Texture* texture) override;
	void SetSamplerFilter(TextureFilter minfilter, TextureFilter magfilter, MipmapFilter mipfilter, float maxanisotropy) override;
	void SetSamplerState(TextureAddress address) override;
	bool Draw(PrimitiveType type, int startIndex, int primitiveCount) override;
	bool DrawIndexed(PrimitiveType type, int startIndex, int primitiveCount) override;
	bool DrawData(PrimitiveType type, int startIndex, int primitiveCount, const void* data) override;
	bool StartRendering(bool clear, int backcolor, Texture* target, bool usedepthbuffer) override;
	bool FinishRendering() override;
	bool Present() override;
	bool ClearTexture(int backcolor, Texture* texture) override;
	bool CopyTexture(Texture* dst, CubeMapFace face) override;

	bool SetVertexBufferData(VertexBuffer* buffer, void* data, int64_t size, VertexFormat format) override;
	bool SetVertexBufferSubdata(VertexBuffer* buffer, int64_t destOffset, void* data, int64_t size) override;
	bool SetIndexBufferData(IndexBuffer* buffer, void* data, int64_t size) override;

	bool SetPixels(Texture* texture, const void* data) override;
	bool SetCubePixels(Texture* texture, CubeMapFace face, const void* data) override;
	void* MapPBO(Texture* texture) override;
	bool UnmapPBO(Texture* texture) override;

	static std::mutex& GetMutex();
	static void DeleteObject(VKVertexBuffer* buffer);
	static void DeleteObject(VKIndexBuffer* buffer);
	static void DeleteObject(VKTexture* texture);

	void ProcessDeleteList(bool finalize = false);

	std::unique_ptr<VKSharedVertexBuffer> mSharedVertexBuffers[2];

	std::list<VKTexture*> mTextures;
	std::list<VKIndexBuffer*> mIndexBuffers;

	std::unique_ptr<VulkanDevice> Device;
	std::unique_ptr<Win32Window> Window;

private:
	struct DeleteList
	{
		std::vector<VKVertexBuffer*> VertexBuffers;
		std::vector<VKIndexBuffer*> IndexBuffers;
		std::vector<VKTexture*> Textures;
	} mDeleteList;
};
