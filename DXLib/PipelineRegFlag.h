#pragma once

#define KEY_NONE ""

enum class PIP_REG
{
	/*
	set vertex shader
	/datas		
	0.path of vertex shader to register
	this can be key after
	*/
	TRANSFORM_SHADER_VERTEX,
	/*
	set Hull shader
	/datas
	0.path of Hull shader to register
	this can be key after
	*/
	TRANSFORM_SHADER_HULL,
	/*
	set domain shader
	/datas
	0.path of domain shader to register
	this can be key after
	*/
	TRANSFORM_SHADER_DOMAIN,
	/*
	set geometry shader
	/datas
	0.path of geom shader to register
	this can be key after
	*/
	TRANSFORM_SHADER_GEOMETRY,

	/*
	set vertice & indice to render
	/datas
	0.key for either register or applying data
	1.vertice array
	2.vertex count
	3.Indice array
	4.Index count
	*/
	TRANSFORM_MESH,
	/*
	set world matrix
	/datas
	0.address of world matrix
	*/
	TRANSFORM_WORLD,
	/*
	set View matrix
	/datas
	0.address of view matrix
	*/
	TRANSFORM_VIEW,
	/*
	set Projection matrix
	/datas
	0.address of project matrix
	*/
	TRANSFORM_PROJECT,
	TRANSFORM_ANIM,
	VIEWPORT_TOPLEFT,
	VIEWPORT_SIZE,

	/*
	set Depth activate flag
	/datas
	1.On/Off of BOOLEAN
	*/
	DEPTH_ENABLE,

	/*
	set pixel shader
	/datas
	0.path of pixel shader to register
	this can be key after
	*/
	PIXEL_SHADER_PIXEL,

	/*
	set directional light info
	/datas
	1.address of dir light info
	2.byte size of pData
	*/
	PIXEL_LIGHT_DIRECTIONAL,
	/*
	set point light info
	/datas
	1.address of point light info
	2.byte size of pData
	*/
	PIXEL_LIGHT_POINT,
	/*
	set spot light info
	/datas
	1.address of spot light info
	2.byte size of pData
	*/
	PIXEL_LIGHT_SPOT,
	/*
	set diffuse(main) texture
	/datas = 
	array of texture key for either registering or applying texture.
	maximum count of array element = 10
	*/
	PIXEL_TEX,

	/*
	set main camera's position
	/datas
	1.x of eye position
	2.y of eye position
	3.z of eye position
	*/
	PIXEL_EYE,

	/*
	set blending set for opaque
	/datas		= nullptr
	*/
	BLENDING_OPAQUE,
	/*
	set blending set for transparent
	/datas		= nullptr
	*/
	BLENDING_TRANSPARENT,
};

enum class PIP_RENDER
{
	STD,
	SHADOW_DEPTH,
	SSAO
};
