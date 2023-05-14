#include "Common.h"

namespace engine
{
    //loads in a whole file, leaves user responsible for it afterwards. including delete[]ing it
    //returns 0 if the file cannot be loaded
    void* LoadFile(const std::string& filename, int* pOutFileSize)
    {
        FILE* fp = fopen(filename.c_str(),"rb");
        if(!fp)
            return 0;
        
        fseek(fp,0,SEEK_END);
        size_t len = ftell(fp);
        fseek(fp,0,SEEK_SET);

        if(pOutFileSize)
            *pOutFileSize = len;

        char* pRet = new char[len+1];
        fread(pRet, sizeof(char),len,fp);
        pRet[len] = 0;
        fclose(fp);

        return pRet;
    }

    GLuint CompileShader(GLuint* pShader, GLenum shaderType, const std::string& fname)
    {
        GLint iStatus;
        const GLchar* pSource = nullptr;
         pSource = (GLchar*)LoadFile(fname);
         
        if(pSource == nullptr)
        {
            printf("failed to load shader\n");
            return 0;
        }

        const GLchar* source[]={pSource};

        *pShader = glCreateShader(shaderType);
        glShaderSource(*pShader,1,source,NULL);
        glCompileShader(*pShader);

        GLint iLogLength;
        glGetShaderiv(*pShader, GL_INFO_LOG_LENGTH, &iLogLength);
        if(iLogLength > 0)
        {
            GLchar* pLog = (GLchar*)malloc(iLogLength);
            glGetShaderInfoLog(*pShader,iLogLength,&iLogLength, pLog);
            printf("Shader compile log: \n %s \n",pLog);
            free(pLog);
        }

        glGetShaderiv(*pShader, GL_COMPILE_STATUS, &iStatus);
        if(iStatus == 0)
        {
            glDeleteShader(*pShader);
            return 0;
        }
        return 1;
    }

    GLuint LoadShaders(const std::string& vsh, const std::string& fsh)
    {
        GLuint uiVS, uiFS;
        //compile both shaders, if either fails, return out deleting anything made
        if(!CompileShader(&uiVS, GL_VERTEX_SHADER, vsh))
        {
            printf("Failed to compile Vertext shader \n");
            return 0;
        }

        if(!CompileShader(&uiFS,GL_FRAGMENT_SHADER,fsh))
        {
            printf("failed to compile Fragment shader \n");
            glDeleteShader(uiVS);
            return 0;
        }

        //create program to return and attach shaders
        GLuint uiProgram = glCreateProgram();
        glAttachShader(uiProgram,uiVS);
        glAttachShader(uiProgram,uiFS);
        glLinkProgram(uiProgram);

        GLint iStatus, iLogLength;
        glGetProgramiv(uiProgram,GL_INFO_LOG_LENGTH,&iLogLength);
        if(iLogLength > 0)
        {
            GLchar* pLog = (GLchar*)malloc(iLogLength);
            glGetProgramInfoLog(uiProgram,iLogLength,&iLogLength,pLog);
            printf("program linking log: \n %s\n",pLog);
            free(pLog);
        }

        glGetProgramiv(uiProgram,GL_LINK_STATUS,&iStatus);
        if(iStatus == 0)
        {
            printf("linking failed: %d \n", uiProgram);
            if(uiVS)
            {
                glDeleteShader(uiVS);
            }
            if(uiFS)
            {
                glDeleteShader(uiFS);
            }
            if(uiProgram)
            {
                glDeleteProgram(uiProgram);
            }

            return 0;
        }

        //free the vertex and fragment shaders
        glDeleteShader(uiVS);
        glDeleteShader(uiFS);
        return uiProgram;
    }
}