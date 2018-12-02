#include "Shader.h"

std::string renderVertexSource =
"attribute vec3 vertex; \n"
"varying vec2 texCoord; \n"
"void main() { \n"
"	texCoord = vertex.xy * 0.5 + 0.5; \n"
"	gl_Position = vec4(vertex, 1.0); \n"
"} ";

std::string renderFragmentSource =
"precision highp float; \n"
"varying vec2 texCoord; \n"
"uniform sampler2D texture; \n"
"void main(){ \n"
"	gl_FragColor = texture2D(texture, texCoord); \n"
"} ";

std::string lineVertexSource =
"attribute vec3 vertex; \n"
"uniform vec3 cubeMin; \n"
"uniform vec3 cubeMax; \n"
"uniform mat4 modelviewProjection; \n"
"void main() { \n"
"	gl_Position = modelviewProjection * vec4(mix(cubeMin, cubeMax, vertex), 1.0); \n"
"} ";

std::string lineFragmentSource =
"precision highp float; \n"
"void main() { "
"	gl_FragColor = vec4(1.0);"
"} ";

std::string bounces = "5";
std::string epsilon = "0.0001";
std::string infinity = "10000.0";
float lightSize = 0.1;
float lightVal = 0.5;

std::string tracerVertexSource =
"attribute vec3 vertex; \n"
"uniform vec3 eye, ray00, ray01, ray10, ray11; \n"
"varying vec3 initialRay; \n"
"varying vec3 testRay00; \n"
"varying vec3 testRay01; \n"
"varying vec3 testRay10; \n"
"varying vec3 testRay11; \n"
"varying vec2 testPercent; \n"
"void main() { \n"
"	vec2 percent = vertex.xy * 0.5 + 0.5; \n"
"	testPercent = percent; \n"
"	testRay00 = ray00; \n"
"	testRay01 = ray01; \n"
"	testRay10 = ray10; \n"
"	testRay11 = ray11; \n"
"	initialRay = mix(mix(ray00, ray01, percent.y), mix(ray10, ray11, percent.y), percent.x); \n"
"	gl_Position = vec4(vertex, 1.0); \n"
"} ";

std::string tracerFragmentSourceHeader =
"precision highp float; \n"
"uniform vec3 eye; \n"
"varying vec3 initialRay; \n"
"varying vec3 testRay00; \n"
"varying vec3 testRay01; \n"
"varying vec3 testRay10; \n"
"varying vec3 testRay11; \n"
"varying vec2 testPercent; \n"
"uniform float textureWeight; \n"
"uniform float timeSinceStart; \n"
"uniform sampler2D texture; \n"
"uniform float glossiness; \n"
"vec3 roomCubeMin = vec3(-1.0, -1.0, -1.0); \n"
"vec3 roomCubeMax = vec3(1.0, 1.0, 1.0); \n";

std::string intersectCubeSource =
"vec2 intersectCube(vec3 origin, vec3 ray, vec3 cubeMin, vec3 cubeMax){ \n"
"	vec3 tMin = (cubeMin - origin) / ray; \n"
"	vec3 tMax = (cubeMax - origin) / ray; \n"
"	vec3 t1 = min(tMin, tMax); \n"
"	vec3 t2 = max(tMin, tMax); \n"
"	float tNear = max(max(t1.x, t1.y), t1.z); \n"
"	float tFar = min(min(t2.x, t2.y), t2.z); \n"
"	return vec2(tNear, tFar); \n"
"} \n";

std::string normalForCubeFunction = "vec3 normalForCube(vec3 hit, vec3 cubeMin, vec3 cubeMax)";
std::string normalForCubeSource = normalForCubeFunction +
"{ \n"
"	if(hit.x < cubeMin.x + " + epsilon + ") return vec3(-1.0, 0.0, 0.0); \n"
"	else if(hit.x > cubeMax.x - " + epsilon + ") return vec3(1.0, 0.0, 0.0); \n"
"	else if(hit.y < cubeMin.y + " + epsilon + ") return vec3(0.0, -1.0, 0.0); \n"
"	else if(hit.y > cubeMax.y - " + epsilon + ") return vec3(0.0, 1.0, 0.0); \n"
"	else if(hit.z < cubeMin.z + " + epsilon + ") return vec3(0.0, 0.0, -1.0); \n"
"	else return vec3(0.0, 0.0, 1.0); \n"
"} \n";

std::string intersectSphereSource =
"float intersectSphere(vec3 origin, vec3 ray, vec3 sphereCenter, float sphereRadius) { \n"
"	vec3 toSphere = origin - sphereCenter; \n"
"	float a = dot(ray, ray); \n"
"	float b = 2.0 * dot(toSphere, ray); \n"
"	float c = dot(toSphere, toSphere) - sphereRadius * sphereRadius; \n"
"	float discriminant = b*b - 4.0*a*c; \n"
"	if(discriminant > 0.0) { \n"
"		float t = (-b - sqrt(discriminant)) / (2.0 * a); \n"
"		if(t > 0.0) return t; \n"
"	} \n"
"	return 10000.0; \n"
"} \n";

std::string normalForSphereSource =
"vec3 normalForSphere(vec3 hit, vec3 sphereCenter, float sphereRadius) { \n"
"	return (hit - sphereCenter) / sphereRadius; \n"
"} \n";

std::string randomSource =
"float random(vec3 scale, float seed) { \n"
"	return fract(sin(dot(gl_FragCoord.xyz + seed, scale)) * 43758.5453 + seed); \n"
"} \n";

std::string cosineWeightDirectionSource =
"vec3 cosineWeightedDirection(float seed, vec3 normal){ \n"
"	float u = random(vec3(12.9898, 78.233, 151.7182), seed); \n"
"	float v = random(vec3(63.7264, 10.873, 623.6736), seed); \n"
"	float r = sqrt(u); \n"
"	float angle = 6.283185307179586 * v; \n"
"	vec3 sdir, tdir; \n"
"	if(abs(normal.x) < .5) { \n"
"		sdir = cross(normal, vec3(1, 0, 0)); \n"
"	} \n"
"	else { \n"
"		sdir = cross(normal, vec3(0, 1, 0)); \n"
"	} \n"
"	tdir = cross(normal, sdir); \n"
"	return r*cos(angle)*sdir + r*sin(angle)*tdir + sqrt(1.-u)*normal; \n"
"} \n";

std::string uniformlyRandomDirectionSource =
"vec3 uniformlyRandomDirection(float seed) { \n"
"	float u = random(vec3(12.9898, 78.233, 151.7182), seed); \n"
"	float v = random(vec3(63.7264, 10.873, 623.6736), seed); \n"
"	float z = 1.0 - 2.0 * u; \n"
"	float r = sqrt(1.0 - z * z); \n"
"	float angle = 6.283185307179586 * v; \n"
"	return vec3(r * cos(angle), r * sin(angle), z); \n"
"} \n";

std::string uniformlyRandomVectorSource =
"vec3 uniformlyRandomVector(float seed) { \n"
"	return uniformlyRandomDirection(seed) * sqrt(random(vec3(36.7539, 50.3658, 306.2759), seed)); \n"
"} \n";

std::string specularReflection =
"vec3 reflectedLight = normalize(reflect(light - hit, normal)); \n"
"specularHighlight = max(0.0, dot(reflectedLight, normalize(hit - origin))); \n";

std::string newDiffuseRay =
"ray = cosineWeightedDirection(timeSinceStart + float(bounce), normal); \n";

std::string newReflectiveRay =
"ray = reflect(ray, normal); \n" +
specularReflection +
"specularHighlight = 2.0 * pow(specularHighlight, 20.0); \n";

std::string newGlossyRay =
"ray = normalize(reflect(ray, normal)) + uniformlyRandomVector(timeSinceStart + float(bounce)) * glossiness; \n" +
specularReflection +
"specularHighlight = pow(specularHighlight, 3.0); \n";

std::string yellowBlueCornellBox =
"if(hit.x < -0.9999) surfaceColor = vec3(0.1, 0.5, 1.0); \n"
"else if(hit.x > 0.9999) surfaceColor = vec3(1.0, 0.9, 0.1); \n";

std::string redGreenCornellBox =
"if(hit.x < -0.9999) surfaceColor = vec3(1.0, 0.3, 0.1); \n"
"else if(hit.x > 0.9999) surfaceColor = vec3(0.3, 1.0, 0.1); \n";

std::string makeShadow(vector<Hitable*> objects) {
	std::string shadowFunction = "float shadow(vec3 origin, vec3 ray) { \n";
	for (int i = 0; i < objects.size(); i++) {
		shadowFunction += objects[i]->getShadowTestCode();
	}
	shadowFunction += "	return 1.0; \n";
	shadowFunction += "} \n";
	return shadowFunction;
}

std::string makeCalculateColor(vector<Hitable*> objects, int material) {
	std::string calculateColor = "";
	calculateColor = calculateColor +
		"vec3 calculateColor(vec3 origin, vec3 ray, vec3 light) { \n" +
		"	vec3 colorMask = vec3(1.0); \n" +
		"	vec3 accumulatedColor = vec3(0.0); \n" +
		"	float testValue = 0.0; \n"+
		"	vec3 testVec; \n"+
		"	for(int bounce = 0; bounce < 5; bounce++) { \n" +
		"		vec2 tRoom = intersectCube(origin, ray, roomCubeMin, roomCubeMax); \n";
	for (int i = 0; i < objects.size(); i++) {
		calculateColor += objects[i]->getIntersectCode();
	}
	calculateColor += "	float t = 10000.0; \n";
	calculateColor += "	if(tRoom.x < tRoom.y) t = tRoom.y; \n";

	for (int i = 0; i < objects.size(); i++) {
		calculateColor += objects[i]->getMinimumIntersectCode();
	}
	calculateColor += "	vec3 hit = origin + ray * t; \n";
	calculateColor += "	vec3 surfaceColor = vec3(0.75); \n";
	calculateColor += "	float specularHighlight = 0.0; \n";
	calculateColor += "	vec3 normal; \n";
	calculateColor += " if(t == tRoom.y) { \n";
	calculateColor += "		normal = -normalForCube(hit, roomCubeMin, roomCubeMax); \n";
	calculateColor += yellowBlueCornellBox;
	calculateColor += newDiffuseRay;
	calculateColor += "	} else if(t == 10000.0) { \n";
	calculateColor += "	break; \n";
	calculateColor += "	} else { \n";
	calculateColor += "		if(false) ; \n";
	for (int i = 0; i < objects.size(); i++) {
		calculateColor += objects[i]->getNormalCalculationCode();
	}

	if (2 == material) {
		calculateColor += newReflectiveRay;
	}
	else if (3 == material) {
		calculateColor += newGlossyRay;
	}
	else {
		calculateColor += newDiffuseRay;
	}
	calculateColor += "	} \n";

	calculateColor += "	vec3 toLight = light - hit; \n";
	calculateColor += "	float diffuse = max(0.0, dot(normalize(toLight), normal)); \n";
	calculateColor += "	float shadowIntensity = shadow(hit + normal * 0.0001, toLight); \n";
	calculateColor += "	colorMask *= surfaceColor; \n";
	calculateColor += "	accumulatedColor += colorMask * (0.5 * diffuse * shadowIntensity); \n";
	calculateColor += "	accumulatedColor += colorMask * specularHighlight * shadowIntensity; \n";
	calculateColor += "	origin = hit; \n";
	//calculateColor += "	testVec = hit; \n";
	//calculateColor += "	testValue = tSphere0; \n";
	calculateColor += "	} \n";
	calculateColor += "	return accumulatedColor; \n";
	calculateColor += "} \n";
	return calculateColor;
}

std::string makeMain() {
	return ""
		"void main() { \n"
		"	vec3 newLight = light + uniformlyRandomVector(timeSinceStart - 53.0) * 0.1; \n"
		"	vec3 textureColor = texture2D(texture, gl_FragCoord.xy / 512.0).rgb; \n"
		"	gl_FragColor = vec4(mix(calculateColor(eye, initialRay, newLight), textureColor, textureWeight), 1.0); \n"
		"}";
}

//check the error in glsl
std::string testMakeMain() {
	return ""
		"void main() { \n"
		"	vec3 newLight = light + uniformlyRandomVector(timeSinceStart - 53.0) * 0.1; \n"
		"	vec3 textureColor = texture2D(texture, gl_FragCoord.xy / 512.0).rgb; \n"
		"	vec3 accumulateColor = calculateColor(eye, initialRay, newLight); \n"
		"	vec2 testRoom = intersectCube(eye, initialRay, roomCubeMin, roomCubeMax); \n"
		"	vec3 colorMask = vec3(1.0, 0.0, 0.0); \n"
		"	vec3 testRoomColor = vec3(testRoom, 0.0); \n"
		"	vec3 testPercentColor = vec3(testPercent, 0.0); \n"
		"	vec3 testInitRay = initialRay.xyz * 10.0 - 0.5; \n"
		"	vec3 testRay00Color = testRay00.xyz * 10.0 - 0.5; \n"
		"	vec3 testRay01Color = testRay01.xyz * 10.0 - 0.5; \n"
		"	vec3 testRay10Color = testRay10.xyz * 10.0 - 0.5; \n"
		"	vec3 testRay11Color = testRay11.xyz * 10.0 - 0.5; \n"
		"	gl_FragColor = vec4(accumulateColor, 1.0); \n"
		"}";
}

std::string makeTracerFragmentSource(vector<Hitable*> objects, int material) {
	std::string tmp = tracerFragmentSourceHeader;
	for (int i = 0; i < objects.size(); i++) {
		tmp += objects[i]->getGlobalCode();
	}
	tmp += intersectCubeSource;
	tmp += normalForCubeSource;
	tmp += intersectSphereSource;
	tmp += normalForSphereSource;
	tmp += randomSource;
	tmp += cosineWeightDirectionSource;
	tmp += uniformlyRandomDirectionSource;
	tmp += uniformlyRandomVectorSource;
	tmp += makeShadow(objects);
	tmp += makeCalculateColor(objects, material);
	tmp += makeMain();
	return tmp;
}

GLuint compileSource(const std::string& source, GLuint type) {
	int srcCodeLength = source.size();
	char *srcCodeC = (char*)source.c_str();

	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &srcCodeC, &srcCodeLength);
	glCompileShader(shader);

	int compilationResult;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compilationResult);
	if (compilationResult == GL_FALSE) {
		char shaderErrorLog[1000];
		glGetShaderInfoLog(shader, 1000, 0, shaderErrorLog);
		if (type == GL_VERTEX_SHADER) {
			cout << "vertex shader" << endl;
		}
		else {
			cout << "fragment shader" << endl;
		}
		cout << shaderErrorLog << endl;
		glDeleteShader(shader);
	}
	return shader;
}

GLuint compileShader(const std::string& vertexSource, const std::string& fragmentSource) {
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, compileSource(vertexSource, GL_VERTEX_SHADER));
	glAttachShader(shaderProgram, compileSource(fragmentSource, GL_FRAGMENT_SHADER));
	glLinkProgram(shaderProgram);
	int infoLogLength = 0, result = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength < 1) infoLogLength = 1;
		char* errormsg = new char[infoLogLength];
		glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, errormsg);
		delete[] errormsg;
	}
	return shaderProgram;
}