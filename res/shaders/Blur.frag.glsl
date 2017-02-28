precision mediump float;
uniform sampler2D texture;  
varying highp vec2 v_texCoord;
uniform vec2 textureSize;
uniform mat4 kernel_data[30];
uniform float kernel_radius;

float clampedSample(vec2 coord)
{
	if(any(lessThan(coord, vec2(0.0))))
	{
		return 0.0;
	}
	else if(any(greaterThan(coord, vec2(1.0))))
	{
		return 0.0;
	}
	else
	{
		return texture2D(texture, coord).r;
	}
}

void main() 
{
    highp float xv = 1.0/textureSize.x;
    highp float yv = 1.0/textureSize.y;

    float fontcolor = clampedSample(v_texCoord);
    highp float glowvalue = 0.0;
    float intensity = 1.3;
	
    {
        float loopx =0.0;
        float loopy =0.0;
        float fullsize = kernel_radius*2.0-1.0;
        float offset = float(int(fullsize/2.0));
        highp float maxlength = length(vec2(fullsize*xv,fullsize*yv));
        float left_top_size = kernel_radius;
        for(loopx = 0.0; loopx < fullsize; loopx+=1.0)
        {
            for(loopy = 0.0; loopy < fullsize; loopy+=1.0)
            {
                float x = (left_top_size-1.0)-abs(loopx-(left_top_size-1.0));
                float y = (left_top_size-1.0)-abs(loopy-(left_top_size-1.0));
                highp vec2 samplecoord = vec2((loopx-offset)*xv, (loopy-offset)*yv);
                float maxv = max(x,y);
                float minv = min(x,y);
                int index = int( (maxv * (maxv+1.0) / 2.0) + minv );
				int matrix_index = index/16;
				int element_row_index = int(mod(float(index),16.0)/4.0);
				int element_column_index = int(mod(mod(float(index),16.0),4.0));
				mat4 matrix;
				matrix = kernel_data[matrix_index];
				float weight = 1.0;
				weight = matrix[element_row_index][element_column_index];

                glowvalue = glowvalue + weight * clampedSample(v_texCoord+samplecoord);
            }
        }
    }
    
	gl_FragColor = vec4(fontcolor + (1.0-fontcolor) * glowvalue * intensity );
//	gl_FragColor = vec4(fontcolor);
}