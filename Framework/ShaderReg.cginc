
#ifndef _SHADER_REG
#define _SHADER_REG

#define SHADER_REG_PS_CM_DIRLIGHT register(b0)
#define SHADER_REG_PS_CB_POINTLIGHT register(b1)
#define SHADER_REG_PS_CB_SPOTLIGHT register(b2)
#define SHADER_REG_PS_CB_EYE register(b3)
#define SHADER_REG_PS_CB_MATERIAL register(b4)
#define SHADER_REG_PS_CB_LIGHTVP register(b5)

#define SHADER_REG_PS_SRV_CM register(t0)
#define SHADER_REG_PS_SRV_DIFFUSE register(t1)
#define SHADER_REG_PS_SRV_NORMAL register(t2)
#define SHADER_REG_PS_SRV_SHADOW register(t3)
#define SHADER_REG_PS_SRV_SSAO register(t4)

#define SHADER_REG_PS_SAMP_CM register(s0)
#define SHADER_REG_PS_SAMP_TEX register(s1)
#define SHADER_REG_PS_SAMP_SHADOW register(s2)

#endif