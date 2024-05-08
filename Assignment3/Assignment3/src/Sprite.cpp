#include "Object.hpp"
#include "Time.hpp"

Sprite::Sprite(const std::string& name, int columns, int baseRow, double xOffset, double yOffset, double frameTime, const std::vector<Texture>& textures)
	: Object(name, Square, textures, true), mColumns(columns), mBaseRow(baseRow), mXOffset(xOffset), mYOffset(yOffset), mFrame(0), mFrameTimer(0), mInterval(frameTime) {
	mCurrentRow = mBaseRow - 1;
	type = ObjectType::SPRITE;
}

Sprite Sprite::create(const std::string& name, int columns, int baseRow, double xOffset, double yOffset, double frameTime, const std::vector<Texture>& textures) {
	Sprite s(name, columns, baseRow, xOffset, yOffset, frameTime, textures);

	return s;
}

void Sprite::update() {
	mFrameTimer += Time::deltaTime;

	if (mFrameTimer < mInterval)
		return;

	std::cout << "Update Sprite State\n";

	// Time to update frame

	mCurrentColumn += 1;
	mCurrentColumn %= mColumns;

	double right = mXOffset + mXOffset * mCurrentColumn;
	double left = 0.0 + mXOffset * mCurrentColumn;

	double up = 1.0 - mYOffset * mCurrentRow;
	double down = 1.0 - (mYOffset + mYOffset * mCurrentRow);

	mMesh.vertices[0].texCoords = { right, up };
	mMesh.vertices[1].texCoords = { right, down };
	mMesh.vertices[2].texCoords = { left, down };
	mMesh.vertices[3].texCoords = { left, up };

	mFrameTimer = 0;
}

