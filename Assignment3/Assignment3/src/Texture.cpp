#include "Texture.hpp"

Texture Texture::create(const std::string& path) {
	Texture t;

	glGenTextures(1, &t.id);

	const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path.c_str());

	if (format == FIF_UNKNOWN) {
		std::cout << "Unknown file type for texture " << path << "\n";

		glDeleteTextures(1, &t.id);
		t.id = 0;

		return t;
	}

	FIBITMAP* bm1 = FreeImage_Load(format, path.c_str());

	if (!bm1) {
		std::cout << "Failed to load image " << path << "\n";

		FreeImage_Unload(bm1);
		glDeleteTextures(1, &t.id);
		t.id = 0;

		return t;
	}

	FIBITMAP* bitmap = FreeImage_ConvertTo32Bits(bm1);
	FreeImage_Unload(bm1);
	void* data = FreeImage_GetBits(bitmap);

	if (!data) {
		std::cout << "Failed to get data from bitmap " << path << std::endl;

		FreeImage_Unload(bitmap);
		glDeleteTextures(1, &t.id);
		t.id = 0;

		return t;
	}

	const int width = FreeImage_GetWidth(bitmap);
	const int height = FreeImage_GetHeight(bitmap);

	std::cout << "Texture loaded! " << path << ", w: " << width << ", h: " << height << std::endl;

	glBindTexture(GL_TEXTURE_2D, t.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(bitmap);

	return t;
}

Texture::~Texture() {
	// glDeleteTextures(1, &id);
}
