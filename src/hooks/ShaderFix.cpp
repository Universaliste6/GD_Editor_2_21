#include "ShaderFix.h"
#include <map>
#include <vector>
#include <bits/stdc++.h>
#include "hooking.h"
#include "cocos2d.h"

#include "CCDirector.h"
#include "CCGLProgram.h"
#include "ccGLStateCache.h"

using namespace cocos2d;

#define FUNCTIONHOOK(returntype, name, ...) \
returntype (*name##O)(__VA_ARGS__);			\
returntype name##H(__VA_ARGS__)

#define RESET1F(program, location) program->setUniformLocationWith1f(location, 0)
#define RESET2F(program, location) program->setUniformLocationWith2f(location, 0, 0)
#define RESET3F(program, location) program->setUniformLocationWith3f(location, 0, 0, 0)
#define RESET4F(program, location) program->setUniformLocationWith4f(location, 0, 0, 0, 0)
/*
// color change
FUNCTIONHOOK(void, ShaderLayer_preColorChangeShader, CCLayer* self) 
{
	ShaderLayer_preColorChangeShaderO(self);

	if
    (
	    MBO(float, self, 0x404) == 1.0
		&& MBO(float, self, 0x408) == 1.0
		&& MBO(float, self, 0x40C) == 1.0
		&& MBO(float, self, 0x410) == 0.0
		&& MBO(float, self, 0x414) == 0.0
		&& MBO(float, self, 0x418) == 0.0
	) 
    {
		auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();
        // reset brightness ig
		RESET3F(program, MBO(int, self, 0x420));
	}
} 

// shockline
FUNCTIONHOOK(void, ShaderLayer_preShockLineShader, CCLayer* self) 
{
    ShaderLayer_preShockLineShaderO(self);

    if(!(MBO(double, self, 0x210) > 0.0)) 
    {
        auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();

        // reset all 1f
        for(uintptr_t i = 0x240; i <= 0x260; i += 4) 
            RESET1F(program, MBO(int, self, i));

        // reset 2f
        RESET2F(program, MBO(int, self, 0x264));
        RESET2F(program, MBO(int, self, 0x268));
    }
}

// shockwave
FUNCTIONHOOK(void, ShaderLayer_preShockWaveShader, CCLayer* self) 
{
    ShaderLayer_preShockWaveShaderO(self);

    if(!(MBO(double, self, 0x1A0) > 0.0)) 
    {
        auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();

        // reset all 1f
        for(uintptr_t i = 0x1E4; i <= 0x208; i += 4) 
            RESET1F(program, MBO(int, self, i));
    }
}

// glitch
FUNCTIONHOOK(void, ShaderLayer_preGlitchShader, CCLayer* self) 
{
    ShaderLayer_preGlitchShaderO(self);

    if(!MBO(bool, self, 0x26C)) 
    {
        auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();

        // reset all 1f
        for(uintptr_t i = 0x284; i <= 0x294; i += 4) 
            if(i != 0x290) RESET1F(program, MBO(int, self, i));

        // reset all 2f
        RESET2F(program, MBO(int, self, 0x290));
    }
}

// chromatic glitch
FUNCTIONHOOK(void, ShaderLayer_preChromaticGlitchShader, CCLayer* self) 
{
    ShaderLayer_preChromaticGlitchShaderO(self);

    if
    (
        !MBO(bool, self, 0x2C0) 
        && MBO(float, self, 0x2AC) == 0
        && MBO(float, self, 0x2B0) == 0
        && MBO(float, self, 0x2BC) == 0
    ) 
    {
        auto spr = MBO(CCSprite*, self, 0x154);
		auto program = spr->getShaderProgram();

        // reset all 1f
        for(uintptr_t i = 0x2C4; i <= 0x2DC; i += 4)
            RESET1F(program, MBO(int, self, i));
    }
}
*/

CCGLProgram* getGLProgram(CCLayer* shaderLayer)
{
    auto spr = MBO(CCSprite*, shaderLayer, 0x154);
	return spr->getShaderProgram();
}
FUNCTIONHOOK(void, ShaderLayer_performCalculations, CCLayer* self)
{
    ShaderLayer_performCalculationsO(self);
    CCGLProgram* glProgram = getGLProgram(self);

    
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_shockWaveOn"), (MBO(double, self, 0x1A0) > 0.0));
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_shockLineOn"), (MBO(double, self, 0x210) > 0.0));
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_glitchOn"), (MBO(bool, self, 0x26C)));
    // fuj chromatic. always on.
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_cGOn"), !(!MBO(bool, self, 0x2C0) || MBO(float, self, 0x2AC) == 0 && MBO(float, self, 0x2B0) == 0 && MBO(float, self, 0x2BC) == 0));
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_lensCircleOn"), (MBO(float, self, 0x2F4) > 0.0f));
    // fuj blurs. always on.
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_bulgeOn"), (MBO(float, self, 0x37C) > 0.0f));
    // fuj pinch. always on.
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_grayscaleOn"), (MBO(float, self, 0x3BC) > 0.0f));
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_sepiaOn"), (MBO(float, self, 0x3D8) > 0.0f));
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_invertColorOn"), (MBO(float, self, 0x3E0) > 0.0f));
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_hueShiftOn"), (MBO(float, self, 0x3F8) != 0.0f));
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_colorChangeOn"), !(MBO(float, self, 0x404) == 1.0
		&& MBO(float, self, 0x408) == 1.0
		&& MBO(float, self, 0x40C) == 1.0
		&& MBO(float, self, 0x410) == 0.0
		&& MBO(float, self, 0x414) == 0.0
		&& MBO(float, self, 0x418) == 0.0));
    glProgram->setUniformLocationWith1i(glProgram->getUniformLocationForName("_splitScreenOn"), (MBO(float, self, 0x424) != 0.0f || MBO(float, self, 0x428) != 0.0f));
}

// compileShader func from latest cocos2dx v2 lmao
FUNCTIONHOOK(bool, CCGLProgram_compileShader, CCGLProgram *self,  GLuint * shader, GLenum type, const GLchar* source)
{
    GLint status;
 
    if (!source)
    {
        return false;
    }
    
    const GLchar *sources[] = {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_LINUX && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#if CC_TARGET_PLATFORM == CC_PLATFORM_NACL
        "precision highp float;\n"
#else
        (type == GL_VERTEX_SHADER ? "precision highp float;\n" : "precision mediump float;\n"),
#endif
#endif
        "uniform mat4 CC_PMatrix;\n"
        "uniform mat4 CC_MVMatrix;\n"
        "uniform mat4 CC_MVPMatrix;\n"
        "uniform vec4 CC_Time;\n"
        "uniform vec4 CC_SinTime;\n"
        "uniform vec4 CC_CosTime;\n"
        "uniform vec4 CC_Random01;\n"
        "//CC INCLUDES END\n\n",
        source,
    };

    *shader = glCreateShader(type);
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (! status)
    {
        GLsizei length;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
		
		glGetShaderSource(*shader, length, NULL, src);
		CCLog("cocos2d: ERROR: Failed to compile shader:\n%s", src);
        
        if (type == GL_VERTEX_SHADER)
        {
            CCLog("cocos2d: %s", self->vertexShaderLog());
        }
        else
        {
            CCLog("cocos2d: %s", self->fragmentShaderLog());
        }
        free(src);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        return false;
#else
		abort();
#endif
    }
    return (status == GL_TRUE);
}

void ShaderFix::ApplyHooks() 
{
    /*HOOK("_ZN11ShaderLayer20preColorChangeShaderEv", ShaderLayer_preColorChangeShaderH, ShaderLayer_preColorChangeShaderO);
    HOOK("_ZN11ShaderLayer18preShockLineShaderEv", ShaderLayer_preShockLineShaderH, ShaderLayer_preShockLineShaderO);
    HOOK("_ZN11ShaderLayer18preShockWaveShaderEv", ShaderLayer_preShockWaveShaderH, ShaderLayer_preShockWaveShaderO);
    HOOK("_ZN11ShaderLayer15preGlitchShaderEv", ShaderLayer_preGlitchShaderH, ShaderLayer_preGlitchShaderO);
    HOOK("_ZN11ShaderLayer24preChromaticGlitchShaderEv", ShaderLayer_preChromaticGlitchShaderH, ShaderLayer_preChromaticGlitchShaderO);
*/
    HOOK("_ZN7cocos2d11CCGLProgram13compileShaderEPjjPKc", CCGLProgram_compileShaderH, CCGLProgram_compileShaderO);

    HOOK("_ZN11ShaderLayer19performCalculationsEv", ShaderLayer_performCalculationsH, ShaderLayer_performCalculationsO);
}