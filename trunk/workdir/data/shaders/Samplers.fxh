#ifndef SAMPLERS_FXH
#define SAMPLERS_FXH

Texture2D T0Texture: register( t0 );	// Diffuse
Texture2D T1Texture: register( t1 );	// LightMap
Texture2D T2Texture: register( t2 );	// NormalMap
Texture2D T3Texture: register( t3 );	// Specular
Texture2D T4Texture: register( t4 );	// ShadowMap
Texture2D T5Texture: register( t5 );	// ShadowMask
Texture2D T6Texture: register( t6 );	
TextureCube T7Texture: register (t7);	// Skybox cube texture

SamplerState S0Sampler: register( s0 );
SamplerState S1Sampler: register( s1 );
SamplerState S2Sampler: register( s2 );
SamplerState S3Sampler: register( s3 );
SamplerState S4Sampler: register( s4 );
SamplerState S5Sampler: register( s5 );
SamplerState S6Sampler: register( s6 );
SamplerState S7Sampler: register( s7 );		// Skybox cube texture	

TextureCube T0TextureCube: register( t0 );	// Diffuse
TextureCube T1TextureCube: register( t1 );	// Bump
TextureCube T2TextureCube: register( t2 );	// LightMap
TextureCube T3TextureCube: register( t3 );	// Specular

#endif //SAMPLERS_FXH