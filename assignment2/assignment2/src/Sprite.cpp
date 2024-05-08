#include "Sprite.hpp"

Sprite::Sprite(
	const Mesh& mesh, 
	Shader* shader, 
	const Texture& spriteSheet, 
	double xOffset, 
	double yOffset, 
	int columns, 
	int rows, 
	float interval, 
	int baseRow)
: Object(mesh, shader, spriteSheet) {
	mXOffset = xOffset; 
	mYOffset = yOffset; 
	mColumns = columns;
	mRows = rows;
	mInterval = interval;
	mBaseRow = baseRow;
	mCurrentRow = mBaseRow - 1; // sub 1 because current row starts at 0 and base row starts at 1

	double right = mXOffset + mXOffset * mCurrentColumn;
	double left = 0.0 + mXOffset * mCurrentColumn;

	double up = 1.0 - mYOffset * mCurrentRow;
	double down = 1.0 - (mYOffset + mYOffset * mCurrentRow);

	mMesh.vertices[0].texCoords = { right, up };
	mMesh.vertices[1].texCoords = { right, down };
	mMesh.vertices[2].texCoords = { left, down };
	mMesh.vertices[3].texCoords = { left, up };

	setupBuffers();
}

void Sprite::update(const double deltaTime, const double elapsedTime) {
	mFrameTimer += deltaTime;

	if (mFrameTimer < mInterval)
		return;

	std::cout << "Update Sprite State\n";

	// Time to update frame

	mCurrentColumn += 1;
	mCurrentColumn %= mColumns;

	if (mCurrentColumn == 0) {
		mCurrentRow += 1;
		mCurrentRow %= mRows;

		if (mCurrentRow == 0)
			mCurrentRow += mBaseRow - 1;
	}

	double right = mXOffset + mXOffset * mCurrentColumn;
	double left = 0.0 + mXOffset * mCurrentColumn;

	double up = 1.0 - mYOffset * mCurrentRow;
	double down = 1.0 - (mYOffset + mYOffset * mCurrentRow);

	mMesh.vertices[0].texCoords = { right, up};
	mMesh.vertices[1].texCoords = { right, down };
	mMesh.vertices[2].texCoords = { left, down };
	mMesh.vertices[3].texCoords = { left, up };

	mFrameTimer = 0;
}


void Sprite::draw(const glm::mat4& view, const glm::mat4& proj) {
	updateMatrix();

	mShader->use();
	mShader->setMat4("transform", mTransMat);
	mShader->setMat4("view", view);
	mShader->setMat4("projection", proj);

	glBindVertexArray(mVAO);
	glBindTexture(GL_TEXTURE_2D, mActiveTexture.id);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mMesh.vertices.size() * sizeof(Vertex), mMesh.vertices.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, mMesh.indices.size() * sizeof(GLuint), mMesh.indices.data());

	glDrawElements(GL_TRIANGLES, mMesh.indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	mShader->disable();
}

void Sprite::setupBuffers() {
	std::cout << "Sprite setup buffer.\n";

	// glGenVertexArrays(1, &mVAO);
	// glGenBuffers(1, &mVBO);
	// glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}



