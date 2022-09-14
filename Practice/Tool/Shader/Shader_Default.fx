
// ���������� : ������Ʈ���̺�.
matrix		g_matWorld, g_matView, g_matProj;

texture		g_DiffuseTexture;

sampler	DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
};

// hlsl�� �ۼ��ϱ����� ����.

struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
	
};

// ������ ��İ��ؼ� �����̽��� ��ȯ�����ִ� �۾�.(�Լ�)
// VS_MAIN(float3 vPosition : POSITION, float3 vNormal : NORMAL, long Color : COLOR)
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;	

	// ���庯ȯ 

	vector		vPosition;

	vPosition = mul(vector(In.vPosition, 1.f), g_matWorld);
	vPosition = mul(vPosition, g_matView);
	vPosition = mul(vPosition, g_matProj);

	Out.vPosition = vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;	
}

// Out.vPosition.w�� Out.vPosition.xyzw ������.(��������)
// ����Ʈ ��ȯ.
// �����Ͷ�����(�ȼ��� ����)


struct PS_IN
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector  vColor : COLOR0;	
};


// �ȼ��� ���� �����ϴ� �۾�.(�Լ�)
// vector PS_MAIN(PS_IN In) : COLOR
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);	

	return Out;	
}

technique Default_Technique
{
	pass Default_Rendering
	{
		AlphaBlendEnable = false;
		CullMode = none;
		FillMode = Solid;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass AlphaBlending
	{
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		FillMode = Solid;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}













