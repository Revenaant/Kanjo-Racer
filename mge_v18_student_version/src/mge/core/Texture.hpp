#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <map>
#include <string>
#include <GL/glew.h>

class Texture
{
	public:
	    Texture();
		virtual ~Texture();
		static Texture* load(const std::string& pTexturePath);

		GLuint getId();

	protected:

        static Texture* _loadFromFile(const std::string pTexturePath);

	    //OpenGL id for texture buffer
		GLuint _id;
};

#endif // TEXTURE_HPP
