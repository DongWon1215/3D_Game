
// 전역변수들 : 컨스턴트테이블.
matrix		g_matWorld, g_matView, g_matProj;

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = (vector)1.f;
vector		g_vMtrlSpecular = (vector)1.f;
float		g_fPower = 30.f;

vector		g_vCamPosition;


texture		g_DiffuseTexture;

sampler	DiffuseSampler = sampler_state
{
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;

	texture = g_DiffuseTexture;
};

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float4	vShade : COLOR0;
	float4	vSpecular : COLOR1;
	float2	vTexUV : TEXCOORD0;
	
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	// 월드변환 

	vector		vPosition;

	vPosition = mul(vector(In.vPosition, 1.f), g_matWorld);
	vPosition = mul(vPosition, g_matView);
	vPosition = mul(vPosition, g_matProj);

	Out.vPosition = vPosition;

	vector	vWorldNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));

	Out.vShade = saturate(dot(normalize(g_vLightDir) * -1.f, vWorldNormal));

	vector	vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);

	vector	vLook = vWorldPos - g_vCamPosition;
	vector	vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	Out.vSpecular = pow(saturate(dot(normalize(vLook) * -1.f, vReflect)), g_fPower);

	Out.vTexUV = In.vTexUV;

	return Out;	
}



struct PS_IN
{
	float4	vPosition : POSITION;
	float4	vShade : COLOR0;
	float4	vSpecular : COLOR1;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector  vColor : COLOR0;	
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector	vDiffuse = tex2D(DiffuseSampler, In.vTexUV);	

	Out.vColor = ((g_vLightDiffuse * vDiffuse) + (g_vLightSpecular * g_vMtrlSpecular) * In.vSpecular) * (In.vShade + (g_vLightAmbient * g_vMtrlAmbient));
	

	return Out;	
}

technique Default_Technique
{
	pass GroudShading // 명암연산을 버텍스 단계에서 수행한다.
	{
		AlphaTestEnable = true;
		AlphaRef = 0;
		AlphaFunc = Greater;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	//pass PhongShading // 명암연산을 픽셀 단계에서 수행한다.
	//{
	//	AlphaTestEnable = true;
	//	AlphaRef = 0;
	//	AlphaFunc = Greater;

	//	VertexShader = compile vs_3_0 VS_MAIN();
	//	PixelShader = compile ps_3_0 PS_MAIN();
	//}
}













