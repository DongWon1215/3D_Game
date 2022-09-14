
matrix		g_matWorld, g_matView, g_matProj;

vector		g_vLightDir; // 명암.
vector		g_vLightDiffuse; // 난반사광원에 의한 물체의  색상을 결정하기위해.
vector		g_vLightAmbient; // 환경광원에 의한 물체의 색상을 결정하기위해.
vector		g_vLightSpecular; // 정반사광원에 의한 물체의  색상을 결정하기위해.

vector		g_vCamPosition;
float		g_fRange;
vector		g_vLightPosition;

vector		g_vBrushPosition;
float		g_fBrushRange;


texture		g_SourTexture; // 난반사광원에 의한 물체의  색상을 결정하기위해.

sampler	SourSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_SourTexture;
};

texture		g_DestTexture; // 난반사광원에 의한 물체의  색상을 결정하기위해.

sampler	DestSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_DestTexture;
};

texture		g_FilterTexture; // 난반사광원에 의한 물체의  색상을 결정하기위해.

sampler	FilterSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_FilterTexture;
};

texture		g_BrushTexture; // 난반사광원에 의한 물체의  색상을 결정하기위해.

sampler	BrushSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;

	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_BrushTexture;
};





vector		g_vMtrlAmbient = (vector)1; // 환경광원에 의한 물체의 색상을 결정하기위해.
vector		g_vMtrlSpecular = (vector)1; // 정반사광원에 의한 물체의  색상을 결정하기위해.
float		g_fPower = 30.0f;



struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;	
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;		

	vector		vPosition;

	vPosition = mul(vector(In.vPosition, 1.f), g_matWorld);
	vPosition = mul(vPosition, g_matView);
	vPosition = mul(vPosition, g_matProj);

	Out.vPosition = vPosition;
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_matWorld);
	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);

	return Out;	
}

struct PS_IN
{
	float4	vPosition : POSITION;
	float4	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4	vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector  vColor : COLOR0;	
};

PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vSourDiffuse = g_vLightDiffuse * tex2D(SourSampler, In.vTexUV * 10.f);
	vector		vDestDiffuse = g_vLightDiffuse * tex2D(DestSampler, In.vTexUV * 10.f);

	
	vector		vFilter = tex2D(FilterSampler, In.vTexUV);
	vector		vBrush = 0.0f;


	if (g_vBrushPosition.x - g_fBrushRange < In.vWorldPos.x && In.vWorldPos.x < g_vBrushPosition.x + g_fBrushRange &&
		g_vBrushPosition.z - g_fBrushRange < In.vWorldPos.z && In.vWorldPos.z < g_vBrushPosition.z + g_fBrushRange)
	{
		float2		vUV;

		vUV.x = ((g_vBrushPosition.x - g_fBrushRange) - In.vWorldPos.x) / (2.f * g_fBrushRange);
		vUV.y = ((g_vBrushPosition.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

		vBrush = tex2D(BrushSampler, vUV);
	}
	
	vector		vDiffuse = vSourDiffuse * vFilter + vDestDiffuse * (1.f - vFilter) + vBrush;

	vector		vAmbient = g_vLightAmbient * g_vMtrlAmbient;
	vector		vSpecular = g_vLightSpecular * g_vMtrlSpecular;

	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);


	vector		vLook = In.vWorldPos - g_vCamPosition;

	vector		vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));

	float		fSpecular_Intensity = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	Out.vColor = (vDiffuse + (vSpecular * fSpecular_Intensity)) * (fShade + vAmbient);
	

	return Out;	
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vSourDiffuse = g_vLightDiffuse * tex2D(SourSampler, In.vTexUV * 10.f);
	vector		vDestDiffuse = g_vLightDiffuse * tex2D(DestSampler, In.vTexUV * 10.f);
	vector		vFilter = tex2D(FilterSampler, In.vTexUV);

	vector		vDiffuse = vSourDiffuse * vFilter + vDestDiffuse * (1.f - vFilter);

	vector		vAmbient = g_vLightAmbient * g_vMtrlAmbient;
	vector		vSpecular = g_vLightSpecular * g_vMtrlSpecular;

	vector		vLightDir = In.vWorldPos - g_vLightPosition;
	float		fDistance = length(vLightDir);

	float		fAtt = max((g_fRange - fDistance) / g_fRange, 0.f);

	float		fShade = max(dot(normalize(vLightDir) * -1.f, normalize(In.vNormal)), 0.f) * fAtt;


	vector		vLook = In.vWorldPos - g_vCamPosition;

	vector		vReflect = reflect(normalize(vLightDir), normalize(In.vNormal));

	float		fSpecular_Intensity = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower) * fAtt;

	Out.vColor = (vDiffuse + (vSpecular * fSpecular_Intensity)) * (fShade + vAmbient);


	return Out;

}

technique Default_Technique
{
	pass Directional 
	{
		fillmode = wireframe;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}

	pass Directional
	{
		fillmode = solid;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}

	pass Point
	{
		fillmode = solid;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}
}