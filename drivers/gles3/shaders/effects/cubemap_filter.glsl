/* clang-format off */
#[modes]

mode_default =
mode_copy = #define MODE_DIRECT_WRITE

#[specializations]

#[vertex]

layout(location = 0) in highp vec2 vertex_attrib;
/* clang-format on */

out highp vec2 uv_interp;

void main() {
	uv_interp = vertex_attrib;
	gl_Position = vec4(uv_interp, 0.0, 1.0);
}

/* clang-format off */
#[fragment]


#define M_PI 3.14159265359

uniform samplerCube source_cube; //texunit:0

/* clang-format on */

uniform int face_id;

#ifndef MODE_DIRECT_WRITE
uniform uint sample_count;
uniform vec4 sample_directions_mip[MAX_SAMPLE_COUNT];
uniform float weight;
#endif

in highp vec2 uv_interp;

layout(location = 0) out vec4 frag_color;

#define M_PI 3.14159265359

// Don't include tonemap_inc.glsl because all we want is these functions, we don't want the uniforms
vec3 linear_to_srgb(vec3 color) {
	return max(vec3(1.055) * pow(color, vec3(0.416666667)) - vec3(0.055), vec3(0.0));
}

vec3 srgb_to_linear(vec3 color) {
	return color * (color * (color * 0.305306011 + 0.682171111) + 0.012522878);
}

vec3 texelCoordToVec(vec2 uv, int faceID) {
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

void main() {
	vec3 color = vec3(0.0);
	vec2 uv = uv_interp;
	vec3 N = texelCoordToVec(uv, face_id);

#ifdef MODE_DIRECT_WRITE
	frag_color = vec4(textureLod(source_cube, N, 0.0).rgb, 1.0);
#else

	vec4 sum = vec4(0.0);
	vec3 UpHector = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	mat3 T;
	T[0] = normalize(cross(UpHector, N));
	T[1] = cross(N, T[0]);
	T[2] = N;

	for (uint sample_num = 0u; sample_num < sample_count; sample_num++) {
		vec4 sample_direction_mip = sample_directions_mip[sample_num];
		vec3 L = T * sample_direction_mip.xyz;
		vec3 val = textureLod(source_cube, L, sample_direction_mip.w).rgb;
		// Mix using linear
		val = srgb_to_linear(val);
		sum.rgb += val * sample_direction_mip.z;
	}

	sum /= weight;

	sum.rgb = linear_to_srgb(sum.rgb);
	frag_color = vec4(sum.rgb, 1.0);
#endif
}
