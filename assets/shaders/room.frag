uniform sampler2D texture;
uniform vec2 playerPos;
uniform vec2 screenSize;
uniform float radius;

void main()
{
    float aspectRatio = screenSize.x / screenSize.y;

    vec2 centeredCoord = (gl_FragCoord.xy / screenSize.xy) - 0.5;
    centeredCoord.x *= aspectRatio;

    vec2 normalizedPlayerPos = (playerPos / screenSize.xy) - 0.5;
    normalizedPlayerPos.x *= aspectRatio;
    normalizedPlayerPos.y *= -1;

    float dist = distance(centeredCoord, normalizedPlayerPos);
    float vignette = 1.0 - smoothstep(radius - 0.25, radius, dist);

    vec4 baseColor = texture2D(texture, gl_TexCoord[0].xy);

    gl_FragColor = vec4(baseColor.rgb * vignette, 1.0);
}