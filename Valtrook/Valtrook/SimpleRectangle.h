#pragma once

#include "Glyph.h"
#include "AABB.h"
#include "UV.h"
#include "GLBlendMode.h"
#include "Texture.h"
#include <array>

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class SimpleRectangle {
	public:
		SimpleRectangle(const TextureResource& texture);
		SimpleRectangle(const TextureResource& texture, float x, float y, float depth, float halfWidth, float halfHeight);
		SimpleRectangle(const TextureResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize);
		SimpleRectangle(const TextureResource& texture, float x, float y, float depth, float halfWidth, float halfHeight, Colour colour, GLBlendMode* blendMode = &GLBlendMode::Blend_Default);
		SimpleRectangle(const TextureResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, Colour colour, GLBlendMode* blendMode = &GLBlendMode::Blend_Default);
		~SimpleRectangle();

		void setTexture(const TextureResource& texture);
		void setX(float x);
		void setY(float y);
		void setCenter(float x, float y);
		void setCenter(std::array<float, 2> center);
		void setDepth(float depth);
		void setHalfWidth(float h_width);
		void setHalfHeight(float h_height);
		void setHalfSize(const std::array<float, 2>& halfSize);
		void setHalfSize(float h_width, float h_height);
		void setColour(const Colour& colour);
		void setUV(const UV& uv);
		void setBlendMode(GLBlendMode* blendMode);
		//Honestly, don't ask. It's really used to GUI rendering, setting the center/height/width and the uv dependant on the culling aabb size&position
		void setCullSurface(AABB<float> cullAABB);

		TextureResource getTexture() const;
		float getX() const;
		float getY() const;
		std::array<float, 2> getCenter() const;
		float getDepth() const;
		float getHalfWidth() const;
		float getHalfHeight() const;
		std::array<float, 2> getHalfSize() const;
		Colour getColour() const;
		UV getUV() const;
		GLBlendMode* getBlendMode() const;
		AABB<float> getCullSurface() const;

		std::array<TriangleGlyph, 2> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine);
		void sendRenderInformation(VBOBatcher* batcher);


	protected:
		std::array<float, 2> center;
		float depth;
		std::array<float, 2> halfSize;
		TextureResource texture;
		GLBlendMode* blendMode;
		UV uvBounds;
		Colour colour;

		AABB<float> cullAABB;
		bool bHasCullSurface;

		std::array<TriangleGlyph, 2> Glyph;

		bool needsReconstructed;
		void recalculateVertexes();

	};
}