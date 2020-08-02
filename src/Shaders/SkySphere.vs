cbuffer MatrixBuffer {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInput {
    float4 position : POSITION;
};

struct PixelInput {
    float4 position : SV_POSITION;
    float4 spherePosition : TEXCOORD0;
};

PixelInput SkySphereVertexShader(VertexInput input) {
    PixelInput output;

    input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.spherePosition = input.position;

    return output;
}