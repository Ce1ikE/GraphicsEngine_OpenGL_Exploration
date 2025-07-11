#include "ResourceClasses/ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;

void checkFileExists(std::string);

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader * ResourceManager::GetShader(std::string name)
{
    return &Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D * ResourceManager::GetTexture(std::string name)
{
    return &Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
    {
        glDeleteProgram(iter.second.ID);
    }
    // (properly) delete all textures
    for (auto iter : Textures)
    {
        glDeleteTextures(1, &iter.second.ID);
    }
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    Shader shader;

    std::string vShaderFilePath{ SHADER_SOURCE_DIR + std::string(vShaderFile) };
    std::string fShaderFilePath{ SHADER_SOURCE_DIR + std::string(fShaderFile) };
    std::string gShaderFilePath;
    if (gShaderFile != nullptr)
    {
        gShaderFilePath = SHADER_SOURCE_DIR + std::string(gShaderFile);
    }

    try
    {
        checkFileExists(vShaderFilePath);
        shader.setVertexSource(vShaderFile);
        checkFileExists(fShaderFilePath);
        shader.setFragmentSource(fShaderFile);
        if (gShaderFile != nullptr)
        {
            checkFileExists(gShaderFilePath);
            shader.setGeometrySource(gShaderFile);
        }
        else
        {
            shader.setGeometrySource(nullptr);
        }

        // open files
        std::ifstream vertexShaderFile(vShaderFilePath,std::ios::in) , fragmentShaderFile(fShaderFilePath, std::ios::in);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFilePath);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        const char* gShaderCode = geometryCode.c_str();
        // 2. now we create a shader object from source code
        shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    }
    catch (const std::exception e)
    {
        std::string errorMsg(e.what());
        Logger::error(
            MESSAGE("SHADER: Failed to read shader files " + errorMsg)
        );
    }
    catch (std::ifstream::failure& e) {
        std::string errorMsg(e.what());
        Logger::error(
            MESSAGE("SHADER: Failed to open files")
        );
    }
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

std::vector<std::string> ResourceManager::showResources()
{
    
    std::vector<std::string> resourceNames;
    
    resourceNames.reserve(Shaders.size() + Textures.size());
    for (auto iter : Shaders)
    {
       resourceNames.push_back(iter.first.c_str());
    }
    for (auto iter : Textures)
    {
        resourceNames.push_back(iter.first.c_str());
    }
    return resourceNames;
};

void checkFileExists(std::string pathToFile)
{
    std::ifstream file(pathToFile);
    if (!file.good())
    {
        throw std::exception(std::string("No file exists : " + pathToFile).c_str());
    }
    else
    {
        Logger::succes(
            MESSAGE(std::string("file exists : ") + pathToFile)
        );
    }
};
