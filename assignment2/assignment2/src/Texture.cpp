#include "Texture.hpp"

Texture::Texture(const std::string& path) {
	glGenTextures(1, &id);

	const FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path.c_str());

	if (format == FIF_UNKNOWN) {
		std::cout << "Unknown file type for texture " << path << std::endl;
		return;
	}

	FIBITMAP* bm1 = FreeImage_Load(format, path.c_str());

	if (!bm1) {
		std::cout << "Failed to load image " << path << std::endl;
		return;
	}

	FIBITMAP* bitmap = FreeImage_ConvertTo32Bits(bm1);
	FreeImage_Unload(bm1);
	void* data = FreeImage_GetBits(bitmap);

	if (!data) {
		std::cout << "Failed to get data from bitmap " << path << std::endl;
		return;
	}

	const int width = FreeImage_GetWidth(bitmap);
	const int height = FreeImage_GetHeight(bitmap);

	std::cout << "Texture loaded! " << path << ", w: " << width << ", h: " << height << std::endl;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture(GL_TEXTURE_2D, 0);
}
