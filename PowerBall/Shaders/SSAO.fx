//------------------------------------------------------------------------------------------------------
//	Post-process effect: Screen space ambient occlusion (SSAO).
//	Approximates ambient occlusion by sampling the depth of neighboring pixels with 3D-offset vectors.
//	Requirements:	Normal in view space and depth in normalized device coordinates for each pixel.
//					Note that normals should be face normals to avoid false occlusion.
//------------------------------------------------------------------------------------------------------

#include "stdafx.fx"

//------------------------------------------------------------------------------------------------------
//	Global variables (non-numeric values cannot be added to constant buffers)
//------------------------------------------------------------------------------------------------------
Texture1D<float3> rndTex;	//used for sampling. contains randomized 3D-vectors x,y,z[-1,1] = length[0,3]
float DEPTH_EPSILON = 0.001f;
static const uint		nrOfSamples = 8;	//temp**

//------------------------------------------------------------------------------------------------------
//	Constant buffers
//------------------------------------------------------------------------------------------------------
cbuffer PerFrame
{
	float		angleBias;				//angle in radian from plane of pixel to ignore samples
	float4x4	viewInverseTranspose;	//transposed inverse of of camera view-matrix
	float4x4	projMatrix;				//projection matrix of camera
	float4x4    invProjMatrix;			//inverse projection matrix
	float4x4	viewToTex;	//projmatrix * texture

	//**
	// Coordinates given in view space.
	float   gOcclusionRadius    = 0.5f;
	float   gOcclusionFadeStart = 0.2f;
	float   gOcclusionFadeEnd   = 2.0f;
	float   gSurfaceEpsilon     = 0.05f;
	float3	gOffsetVectors[14];
	//**

	//**blur
	float gWeights[11] = 
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f
	};
	//**
};
//**ssao
Texture2D gRandomVecMap;
//**blur
static const int gBlurRadius = 5;

float OcclusionFunction(float distZ)
{
	//
	// If depth(q) is "behind" depth(p), then q cannot occlude p.  Moreover, if 
	// depth(q) and depth(p) are sufficiently close, then we also assume q cannot
	// occlude p because q needs to be in front of p by Epsilon to occlude p.
	//
	// We use the following function to determine the occlusion.  
	// 
	//
	//       1.0     -------------\
	//               |           |  \
	//               |           |    \
	//               |           |      \ 
	//               |           |        \
	//               |           |          \
	//               |           |            \
	//  ------|------|-----------|-------------|---------|--> zv
	//        0     Eps          z0            z1        
	//
	
	float occlusion = 0.0f;
	if(distZ > gSurfaceEpsilon)
	{
		float fadeLength = gOcclusionFadeEnd - gOcclusionFadeStart;
		
		// Linearly decrease occlusion from 1 to 0 as distZ goes 
		// from gOcclusionFadeStart to gOcclusionFadeEnd.	
		occlusion = saturate( (gOcclusionFadeEnd-distZ)/fadeLength );
	}
	
	return occlusion;	
}
float4 Blur(float2 texCoord, Texture2D Texture, Texture2D normalAndDepth, uniform bool horizontalBlur)
{
	float2 texOffset;
	if(horizontalBlur)
	{
		texOffset = float2(1.0f / width, 0.0f);
	}
	else
	{
		texOffset = float2(0.0f, 1.0f / height);
	}

	// The center value always contributes to the sum.
	float4 color      = gWeights[5]*Texture.SampleLevel(PointClampSampler, texCoord, 0.0);
	float totalWeight = gWeights[5];
	 
	float4 centerNormalDepth = normalAndDepth.SampleLevel(PointClampSampler, texCoord, 0.0f);

	for(float i = -gBlurRadius; i <=gBlurRadius; ++i)
	{
		// We already added in the center weight.
		if( i == 0 )
			continue;

		float2 tex = texCoord + i*texOffset;

		float4 neighborNormalDepth = normalAndDepth.SampleLevel(PointClampSampler, tex, 0.0f);

		//
		// If the center value and neighbor values differ too much (either in 
		// normal or depth), then we assume we are sampling across a discontinuity.
		// We discard such samples from the blur.
		//
	
		if( dot(neighborNormalDepth.xyz, centerNormalDepth.xyz) >= 0.8f &&
		    abs(neighborNormalDepth.a - centerNormalDepth.a) <= 0.2f )
		{
			float weight = gWeights[i+gBlurRadius];

			// Add neighbor pixel to blur.
			color += weight*Texture.SampleLevel(PointClampSampler, tex, 0.0);
		
			totalWeight += weight;
		}
	}

	// Compensate for discarded samples by making total weights sum to 1.
	color.w = 1.0f;
	return color / totalWeight;

}
//------------------------------------------------------------------------------------------------------
//	SSAO Shader
//------------------------------------------------------------------------------------------------------
float4 SSAO(float2 texCoord, Texture2D normalAndDepthMap, Texture2D Texture)
{
	/*
	///********
	float fovy = PI * 0.45f;
	float farZ = 200.0f;
	float aspect = (float)900 / (float)1440;
	
	float halfHeight = farZ * tan(0.5f * fovy);
	float halfWidth  = aspect * halfHeight;

	float4 FrustumFarCorner0 = float4(-halfWidth, -halfHeight, farZ, 0.0f);
	float4 FrustumFarCorner1 = float4(-halfWidth, +halfHeight, farZ, 0.0f);
	float4 FrustumFarCorner2 = float4(+halfWidth, +halfHeight, farZ, 0.0f);
	float4 FrustumFarCorner3 = float4(+halfWidth, -halfHeight, farZ, 0.0f);
	///**********

	// p -- the point we are computing the ambient occlusion for.
	// n -- normal vector at p.
	// q -- a random offset from p.
	// r -- a potential occluder that might occlude p.
	
	// Get viewspace normal and z-coord of this pixel.  The tex-coords for
	// the fullscreen quad we drew are already in uv-space.
	float4 normAndDepth = normalAndDepthMap.SampleLevel(PointBorderSampler, texCoord, 0.0f); //sample normal (world space) and depth of the pixel (normalized device coordinates [0,1])
	
	float3 n = normalize(mul(float4(normAndDepth.xyz, 1.0f), viewInverseTranspose).xyz);
	float pz = normAndDepth.w;
	
	//********
	//
	// Reconstruct full view space position (x,y,z).
	// Find t such that p = t*pin.ToFarPlane.
	// p.z = t*pin.ToFarPlane.z
	// t = p.z / pin.ToFarPlane.z
	//
	float4 pixelPosV = float4(texCoord.x, texCoord.y, normAndDepth.w, 1.0f); //texture space x[0,1], y[0,1], z[0,1]
	//convert to normalized device coordinates [-1,1]
	pixelPosV.y = 1.0f - pixelPosV.y; //first invert direction of y-axis
	pixelPosV.xy = pixelPosV.xy * 2.0f - 1.0f;
	//transform by the inverse projection matrix; 
	pixelPosV = mul(pixelPosV, invProjMatrix);
	//convert to view-space by dividing by w
	pixelPosV.xyz /= pixelPosV.w;
	float3 p = pixelPosV.xyz;
	//********

	// Extract random vector and map from [0,1] --> [-1, +1].
	float3 randVec = gRandomVecMap.SampleLevel(PointWrapSampler, texCoord, 0.0f).xyz;

	float occlusionSum = 0.0f;
	
	[unroll]
	// Sample neighboring points about p in the hemisphere oriented by n.
	for(uint i = 0; i < nrOfSamples; ++i)
	{
		// Our offset vectors are fixed and uniformly distributed (so that our offset vectors
		// do not clump in the same direction).  If we reflect them about a random vector
		// then we get a random uniform distribution of offset vectors.
		float3 offset = reflect(gOffsetVectors[i].xyz, randVec);
	
		// Flip offset vector if it is behind the plane defined by (p, n).
		float flip = sign( dot(offset, n) );
		
		// Sample a point near p within the occlusion radius.
		float3 q = p + flip * gOcclusionRadius * offset;
		
		// Project q and generate projective tex-coords.  
		float4 projQ = mul(float4(q, 1.0f), viewToTex);
		projQ /= projQ.w;
	
		// Find the nearest depth value along the ray from the eye to q (this is not
		// the depth of q, as q is just an arbitrary point near p and might
		// occupy empty space).  To find the nearest depth we look it up in the depthmap.

		float rz = normalAndDepthMap.SampleLevel(PointBorderSampler, projQ.xy, 0.0f).a;

		// Reconstruct full view space position r = (rx,ry,rz).  We know r
		// lies on the ray of q, so there exists a t such that r = t*q.
		// r.z = t*q.z ==> t = r.z / q.z

		float3 r = (rz / q.z) * q;
		
		//
		// Test whether r occludes p.
		//   * The product dot(n, normalize(r - p)) measures how much in front
		//     of the plane(p,n) the occluder point r is.  The more in front it is, the
		//     more occlusion weight we give it.  This also prevents self shadowing where 
		//     a point r on an angled plane (p,n) could give a false occlusion since they
		//     have different depth values with respect to the eye.
		//   * The weight of the occlusion is scaled based on how far the occluder is from
		//     the point we are computing the occlusion of.  If the occluder r is far away
		//     from p, then it does not occlude it.
		// 
		
		float distZ = p.z - r.z;
		float dp = max(dot(n, normalize(r - p)), 0.0f);
		float occlusion = dp * OcclusionFunction(distZ);
		
		occlusionSum += occlusion;
	}
	
	occlusionSum /= nrOfSamples;
	
	float access = 1.0f - occlusionSum;
	// Sharpen the contrast of the SSAO map to make the SSAO affect more dramatic.
	float t = saturate(pow(access, 4.0f));
	float4 color = float4(1,1,1,1) - float4(t,t,t,0);
	return color;
	
	//float4 colorT = saturate(pow(access, 4.0f));
	//colorT = Blur(texCoord, Texture, normalAndDepthMap, true);
	//return colorT;

	*/
	
	
	float4 normAndDepth = normalAndDepthMap.Sample(PointBorderSampler, texCoord); //sample normal (world space) and depth of the pixel (normalized device coordinates [0,1])
	//transform normal of pixel from world space to view space
	normAndDepth.xyz = normalize(mul(float4(normAndDepth.xyz, 0.0f), viewInverseTranspose).xyz); 

	float4 debugColor = float4(1,1,1,0);
	float occlusionFactor = 0.0f;
	if(normAndDepth.w > -DEPTH_EPSILON && normAndDepth.w < 1.0f + DEPTH_EPSILON) //exclude pixels with no depth data (-1.0f) and skybox ( > 1.0f)
	{
		debugColor.w = 1;

		//1. get view position of the pixel
		float4 pixelPosV = float4(texCoord.x, texCoord.y, normAndDepth.w, 1.0f); //texture space x[0,1], y[0,1], & NDC z[0,1]
		//convert to normalized device coordinates [-1,1]
		pixelPosV.y = 1.0f - pixelPosV.y; //first invert direction of y-axis
		pixelPosV.xy = pixelPosV.xy * 2.0f - 1.0f;
		//transform by the inverse projection matrix; 
		pixelPosV = mul(pixelPosV, invProjMatrix);
		//convert to view-space by dividing by w
		pixelPosV.xyz /= pixelPosV.w;

		float dist = 0.0f;
		float4 offsetPos[nrOfSamples];
		for(uint i = 0; i < nrOfSamples; i++) 
		{
			//2. add 3D-vectors to the view position of the pixel
			//sample offset vector
			float3 offsetVector = rndTex.Sample(PointWrapSampler, i); 
			//check if offset vector is above surface of the pixel and within angle bias, if not, flip and hope for the best
			if(dot(offsetVector, normAndDepth.xyz) > angleBias)
			{
				offsetVector = -offsetVector;
			}
			offsetPos[i] = float4(pixelPosV.xyz + offsetVector, 1.0f); 

			//3. convert offset xy-positions to texture space and z to normalized device coordinates [0,1]
			//convert to clip space xy[-w,w], z[0,w]
			offsetPos[i] = mul(offsetPos[i], projMatrix); 
			//convert to normalized device coordinates xy[-1,1], z[0,1]
			offsetPos[i].xyz /= offsetPos[i].w; 
			//convert to texture space [0,1]
			offsetPos[i].xy = (offsetPos[i].xy + 1) * 0.5f;
			offsetPos[i].y = 1.0f - offsetPos[i].y; //invert direction of y-axis

			//4. get distance between the depth of the sample (offset position) and the depth sampled at that point
			dist = offsetPos[i].z - normalAndDepthMap.Sample(PointBorderSampler, offsetPos[i].xy).w;  
			
			//if distance is positive, then the offset position is behind the pixel sampled at that xy-location and therefore occlude the pixel being computed to some degree
			if(dist > DEPTH_EPSILON) //use epsilon to avoid using samples behind the pixel
			{
				//5. todo: occlusion function - quadratic?
				//
				//	negative depth deltas give zero occlusion
				//	smaller depth deltas give higher occlusion 
				//	the occlusion value falls to zero again beyond a certain depth delta value
				//
				occlusionFactor += (1.0f / nrOfSamples);
				debugColor -= (1.0f / nrOfSamples) * float4(1,1,1,0); 

				//6. todo: blur (bilateral)
			}
		}
	}
	float access = 1.0f - occlusionFactor;

	//return saturate(pow(access, 4.0f));
	return debugColor;
	
}

