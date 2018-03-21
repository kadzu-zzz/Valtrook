#include "SimpleTextRectangle.h"

#include "TextRenderer.h"
#include "Logger.h"
#include "Conversion.h"
#include "RenderingEngine.h"
#include "VBOBatcher.h"

namespace Val {
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture) : SimpleTextRectangle(texture, { 0.0f, 0.0f }, 0.5f, { 1.0f, 1.0f }, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture, float x, float y, float depth, float halfWidth, float halfHeight) : SimpleTextRectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize) : SimpleTextRectangle(texture, center, depth, halfSize, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, Colour colour, const GLBlendMode & blendMode) : SimpleTextRectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, colour, blendMode) {
	}
	SimpleTextRectangle::SimpleTextRectangle(const TextResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, Colour colour, const GLBlendMode & blendMode) :
		textResource(texture), center(center), depth(depth), halfSize(halfSize), renderColour(colour), blendMode(blendMode), needsReconstructed(true) {
	}
	SimpleTextRectangle::~SimpleTextRectangle() {
	}

	void SimpleTextRectangle::setTextResource(const TextResource & text) {
		textResource = text;
		needsReconstructed = true;
	}

	void SimpleTextRectangle::setText(const std::string & contents) {
		textResource = TextResource(textResource.getFont(), contents);
		needsReconstructed = true;
	}

	void SimpleTextRectangle::setFont(FontAsset * newFont) {
		textResource = TextResource(newFont, textResource.getTextString());
		needsReconstructed = true;
	}

	void SimpleTextRectangle::setX(float x) {
		this->center[0] = x;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setY(float y) {
		this->center[1] = y;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setCenter(const std::array<float, 2>& center) {
		this->center = center;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setCenter(float x, float y) {
		this->center[0] = x;
		this->center[1] = y;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setHalfWidth(float h_width) {
		this->halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setHalfHeight(float h_height) {
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setHalfSize(float h_width, float h_height) {
		this->halfSize[0] = h_width;
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setWidth(float width) {
		this->halfSize[0] = width / 2.0f;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setHeight(float height) {
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setSize(float width, float height) {
		this->halfSize[0] = width / 2.0f;
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setColour(const Colour & colour) {
		this->renderColour = colour;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setBlendMode(const GLBlendMode & blendMode) {
		this->blendMode = blendMode;
		needsReconstructed = true;
	}
	void SimpleTextRectangle::setScaleTextToHeight(bool scaleTextToHeight) {
		this->scaleTextToHeight = scaleTextToHeight;
		needsReconstructed = true;
	}
	TextResource SimpleTextRectangle::getTextResource() const {
		return textResource;
	}
	std::string SimpleTextRectangle::getText() const {
		return textResource.getTextString();
	}
	FontAsset * SimpleTextRectangle::getFont() const {
		return textResource.getFont();
	}
	float SimpleTextRectangle::getX() const {
		return center[0];
	}
	float SimpleTextRectangle::getY() const {
		return center[1];
	}
	float SimpleTextRectangle::getDepth() const {
		return depth;
	}
	std::array<float, 2> SimpleTextRectangle::getCenter() const {
		return center;
	}
	float SimpleTextRectangle::getHalfWidth() const {
		return halfSize[0];
	}
	float SimpleTextRectangle::getHalfHeight() const {
		return halfSize[1];
	}
	std::array<float, 2> SimpleTextRectangle::getHalfSize() const {
		return halfSize;
	}
	Colour SimpleTextRectangle::getColour() const {
		return renderColour;
	}
	GLBlendMode SimpleTextRectangle::getBlendMode() const {
		return blendMode;
	}
	bool SimpleTextRectangle::doesScaleTextToHeight() const {
		return scaleTextToHeight;
	}
	void SimpleTextRectangle::reconstruct() {
		if (needsReconstructed) {
			recalculateVertexes();
		}
	}
	std::array<TriangleGlyph, 2> SimpleTextRectangle::getRenderGlyphs() {
		if (needsReconstructed)
			recalculateVertexes();
		return Glyph;
	}
	void SimpleTextRectangle::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			recalculateVertexes();

		engine->getRenderer()->add(Glyph);
	}
	void SimpleTextRectangle::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			recalculateVertexes();

		batcher->add(Glyph);
	}
	void SimpleTextRectangle::recalculateVertexes() {
		needsReconstructed = false;

		auto texture = TextRenderer::getTexture(textResource);
		Colour renderColour = this->renderColour;

		if (texture.getTexture() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Simple Text Rectangle");
			texture = Texture::errorTexture;
			renderColour = Colour(255, 255, 255, this->renderColour.a);
		}

		auto uvBounds = texture.getBounds();

		if (scaleTextToHeight) {
			auto texSize = texture.getTextureSizeInPixel();
			halfSize[0] = (halfSize[1] / texSize[1]) * texSize[0];
		}


		std::array<float, 2> centerUPixel = { WorldToUnalignedPixel<float>(center[0]), WorldToUnalignedPixel<float>(center[1]) };
		std::array<float, 2> halfSizeUPixel = { WorldToUnalignedPixel<float>(halfSize[0]), WorldToUnalignedPixel<float>(halfSize[1]) };

		Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>({

			Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v + uvBounds.vHeight, renderColour),
			Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v + uvBounds.vHeight, renderColour)

		}), &blendMode).dispose();
	}
}