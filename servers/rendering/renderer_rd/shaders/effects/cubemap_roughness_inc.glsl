#define M_PI 3.14159265359

layout(push_constant, std430) uniform Params {
	uint face_id;
	uint sample_count;
	float roughness;
	bool use_direct_write;
	float face_size;
}
params;

vec3 texelCoordToVec(vec2 uv, uint faceID) {
	mat3 faceUvHectors[6];

	// -x
	faceUvHectors[1][0] = vec3(0.0, 0.0, 1.0); // u -> +z
	faceUvHectors[1][1] = vec3(0.0, -1.0, 0.0); // v -> -y
	faceUvHectors[1][2] = vec3(-1.0, 0.0, 0.0); // -x face

	// +x
	faceUvHectors[0][0] = vec3(0.0, 0.0, -1.0); // u -> -z
	faceUvHectors[0][1] = vec3(0.0, -1.0, 0.0); // v -> -y
	faceUvHectors[0][2] = vec3(1.0, 0.0, 0.0); // +x face

	// -y
	faceUvHectors[3][0] = vec3(1.0, 0.0, 0.0); // u -> +x
	faceUvHectors[3][1] = vec3(0.0, 0.0, -1.0); // v -> -z
	faceUvHectors[3][2] = vec3(0.0, -1.0, 0.0); // -y face

	// +y
	faceUvHectors[2][0] = vec3(1.0, 0.0, 0.0); // u -> +x
	faceUvHectors[2][1] = vec3(0.0, 0.0, 1.0); // v -> +z
	faceUvHectors[2][2] = vec3(0.0, 1.0, 0.0); // +y face

	// -z
	faceUvHectors[5][0] = vec3(-1.0, 0.0, 0.0); // u -> -x
	faceUvHectors[5][1] = vec3(0.0, -1.0, 0.0); // v -> -y
	faceUvHectors[5][2] = vec3(0.0, 0.0, -1.0); // -z face

	// +z
	faceUvHectors[4][0] = vec3(1.0, 0.0, 0.0); // u -> +x
	faceUvHectors[4][1] = vec3(0.0, -1.0, 0.0); // v -> -y
	faceUvHectors[4][2] = vec3(0.0, 0.0, 1.0); // +z face

	// out = u * s_faceUv[0] + v * s_faceUv[1] + s_faceUv[2].
	vec3 result = (faceUvHectors[faceID][0] * uv.x) + (faceUvHectors[faceID][1] * uv.y) + faceUvHectors[faceID][2];
	return normalize(result);
}

vec3 ImportanceSampleGGX(vec2 xi, float roughness4) {
	// Compute distribution direction
	float Phi = 2.0 * M_PI * xi.x;
	float CosTheta = sqrt((1.0 - xi.y) / (1.0 + (roughness4 - 1.0) * xi.y));
	float SinTheta = sqrt(1.0 - CosTheta * CosTheta);

	// Convert to spherical direction
	vec3 H;
	H.x = SinTheta * cos(Phi);
	H.y = SinTheta * sin(Phi);
	H.z = CosTheta;

	return H;
}

float DistributionGGX(float NdotH, float roughness4) {
	float NdotH2 = NdotH * NdotH;
	float denom = (NdotH2 * (roughness4 - 1.0) + 1.0);
	denom = M_PI * denom * denom;

	return roughness4 / denom;
}

float radicalInverse_VdC(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N) {
	return vec2(float(i) / float(N), radicalInverse_VdC(i));
}
