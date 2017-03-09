#ifndef SAMPLERS_FXH
#define SAMPLERS_FXH

Texture2D T0Texture: register( t0 );	// Diffuse
Texture2D T1Texture: register( t1 );	// Bump
Texture2D T2Texture: register( t2 );	// LightMap
Texture2D T3Texture: register( t3 );	// Specular

SamplerState S0Sampler: register( s0 );
SamplerState S1Sampler: register( s1 );
SamplerState S2Sampler: register( s2 );
SamplerState S3Sampler: register( s3 );

TextureCube T0TextureCube: register( t0 );	// Diffuse
TextureCube T1TextureCube: register( t1 );	// Bump
TextureCube T2TextureCube: register( t2 );	// LightMap
TextureCube T3TextureCube: register( t3 );	// Specular


#endif //SAMPLERS_FXH