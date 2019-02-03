#version 140

// A quad is uploaded extending from -1 to 1 on both axes
// The uv variable interpolates between the quad vertices
smooth in vec2 uv;
out vec4 outColor;

// Camera
uniform vec3 camera_up;
uniform vec3 camera_right;
uniform vec3 camera_forward;
uniform vec3 camera_eye;
uniform float camera_focal_length;
uniform float camera_near;
uniform float camera_far;

// Scene
uniform vec2  scene_resolution;
uniform int   scene_steps;
uniform float scene_epsilon;
uniform float scene_aspect_ratio;
uniform vec4  scene_sky_color;
uniform vec4  scene_ambient;
uniform vec3  scene_light0pos;
uniform vec4  scene_light0col;

float mapTo(float x, float minX, float maxX, float minY, float maxY) {
    float a = (maxY - minY) / (maxX - minX);
    float b = minY - a * minX;
    return a * x + b;
}

float vmax(vec3 v) {
    return max(max(v.x, v.y), v.z);
}

float fSphere(vec3 position, float radius) {
    return length(position) - radius;
}

float fBoxCheap(vec3 position, vec3 size) {
    return vmax(abs(position) - size);
}

float DistScene(vec3 position) {
    float a = fSphere(position, 0.5);
    float b = fBoxCheap(position, vec3(0.25, 0.33, 1.0));
    return max(b, -a);
}

// Approximate the normalized gradient of the distance function at point p.
// If p is near a surface, the gradient will approximate the surface normal.
vec3 GetNormal(vec3 p) {
    float h = 0.0001f;
    return normalize(vec3(
        DistScene(p + vec3(h, 0, 0)) - DistScene(p - vec3(h, 0, 0)),
        DistScene(p + vec3(0, h, 0)) - DistScene(p - vec3(0, h, 0)),
        DistScene(p + vec3(0, 0, h)) - DistScene(p - vec3(0, 0, h))));
}

// Return a value [0,1] depending on how visible p1 is from p0.
// k is the soft shadow factor (larger -> harder shadows).
// See http://www.iquilezles.org/www/articles/rmshadows/rmshadows.htm
float GetVisibility(vec3 p0, vec3 p1, float k) {
    vec3 rd = normalize(p1 - p0);
    float maxt = length(p1 - p0);
    float t = scene_epsilon * 10.0f;
    float f = 1.0f;

    while (t < maxt) {
        float d = DistScene(p0 + rd*t);

        // If we hit a surface before reaching p1, not visible.
        if (d < scene_epsilon) {
            return 0.0f;
        }
        // Compute penumbra factor
        f = min(f, k * d/t);
        t += d;
    }
    return f;
}

vec4 GetShading(
        vec3 pos, vec3 normal,
        vec3 light_pos, vec4 light_col) {
    float intensity = 0.0f;
    float vis = GetVisibility(pos, light_pos, 16.0f);
    if (vis > 0.0f) {
        vec3 light_dir = normalize(light_pos - pos);
        intensity = vis * clamp(dot(normal, light_dir), 0, 1);
    }
    return light_col * intensity + scene_ambient * (1.0f - intensity);
}

void RayMarch(
        vec3 ro, vec3 rd,
        inout int i, inout float distance) {
    distance = 0.0f;
    for(i=0; i<scene_steps; ++i) {
        float d = DistScene(ro + rd * distance);

        // Make epsilon proportional to the distance so that accuracy can
        // drop as we get further into the scene.  We also just drop the
        // ray if it goes outside the far camera bound.
        if (d < scene_epsilon*distance*2.0f || distance >= camera_far) {
            break;
        }
        distance += d;
    }
}

vec4 GetFloorTexture(vec3 pos) {
    // Compute a checkerboard texture
    vec2 m = mod(pos.xz, 2.0f) - vec2(1.0f);
    return m.x * m.y > 0.0f ? vec4(0.333f) : vec4(1.0f);
}

float RaytraceFloor(vec3 ro, vec3 rd, vec3 normal, vec3 pos) {
    return dot(pos - ro, normal) / dot(rd, normal);
}

vec4 DistLines(vec3 p) {
    return vec4(1);
}

vec4 ComputeColor(vec3 ro, vec3 rd) {
    vec3 floor_normal = vec3(0, 1, 0);
    vec3 floor_pos = vec3(0, -0.5f, 0);

    float t;                    // Distance travelled by ray to eye
    vec3 p;                     // Surface point
    vec3 normal;                // Surface normal
    vec4 texture = vec4(1.0f);  // Surface texture

    int i;          // Steps traveled in raymarch
    float t0;     // Distance traveled in raymarch
    RayMarch(ro, rd, i, t0);

    float t1 = RaytraceFloor(ro, rd, floor_normal, floor_pos);

    // Check if floor was closet and in view of the camera.
    if (t1 < t0 && t1 >= camera_near && t1 < camera_far) {
        t = t1;
        p = ro + rd*t;
        normal = floor_normal;
        texture = GetFloorTexture(p); // * DistLines(p);

    } else if (i < scene_steps && t0 >= camera_near && t0 < camera_far) {
        t = t0;
        p = ro + rd*t;
        normal = GetNormal(p);
    } else {
        return scene_sky_color;
    }

    vec4 color;
    // float z = mapTo(t, camera_near, camera_far, 1, 0);
    // Color as a function of distance
    // color = vec4(1.0f) * z * texture;

    // Light source and ambient with shading
    color = texture * GetShading(p, normal, scene_light0pos, scene_light0col);
    return color;
}

void main()
{
    vec3 rayorigin = camera_eye;
    vec4 color;
#if 1
    vec3 raydirection = normalize(camera_forward * camera_focal_length +
                                  camera_right * uv.x * scene_aspect_ratio + 
                                  camera_up * uv.y);
    color = ComputeColor(rayorigin, raydirection);
#else
    // 4x Anti-Aliasing
    vec2 hps = vec2(1.0) / (scene_resolution * 2.0);
    vec3 rd0 = normalize(camera_forward * camera_focal_length +
                         camera_right * (uv.x-hps.x) * scene_aspect_ratio + 
                         camera_up * (uv.y-hps.y));
    vec3 rd1 = normalize(camera_forward * camera_focal_length +
                         camera_right * (uv.x+hps.x) * scene_aspect_ratio + 
                         camera_up * (uv.y-hps.y));
    vec3 rd2 = normalize(camera_forward * camera_focal_length +
                         camera_right * (uv.x-hps.x) * scene_aspect_ratio + 
                         camera_up * (uv.y+hps.y));
    vec3 rd3 = normalize(camera_forward * camera_focal_length +
                         camera_right * (uv.x+hps.x) * scene_aspect_ratio + 
                         camera_up * (uv.y+hps.y));

    color = (ComputeColor(rayorigin, rd0) +
             ComputeColor(rayorigin, rd1) +
             ComputeColor(rayorigin, rd2) +
             ComputeColor(rayorigin, rd3)) / 4.0;

#endif

	outColor = vec4(color.xyz, 1.0f);
}
