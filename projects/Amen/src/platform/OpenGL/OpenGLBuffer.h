#pragma once
#include <core/Render/Buffer.h>

namespace Hazel
{
	class OpenGLVertexBuffer: public VertexBuffer
	{
		public:
			OpenGLVertexBuffer(const float *data, unsigned int size);

		public:
			virtual ~OpenGLVertexBuffer();
			virtual void Bind() override;
			virtual void UnBind() override;

		private:
			unsigned int m_id;
	};



	class OpenGLIndexBuffer: public IndexBuffer
	{
		public:
			OpenGLIndexBuffer(unsigned int *data, unsigned int count);

		public:
			virtual ~OpenGLIndexBuffer();
			virtual void Bind() override;
			virtual void UnBind() override;

		private:
			unsigned int m_id;
	};



	class OpenGLArrayBuffer: public ArrayBuffer
	{
		public:
			OpenGLArrayBuffer(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer);

		public:
			virtual ~OpenGLArrayBuffer();
			virtual void Bind() override;
			virtual void UnBind() override;

		private:
			unsigned int m_id;
	};
}
