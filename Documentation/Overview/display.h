
/*! \page Display Display Module Reference

    <h2>Window and Input</h2>

    <ul>
        <li>uicore::DisplayWindow - Top-level window class</li>
        <li>uicore::DisplayWindowDescription - Allows you to setup a more advanced description when creating a display window</li>
        <li>uicore::Cursor - Mouse cursor class</li>
        <li>uicore::CursorDescription - Allows you to setup a more advanced description when creating a mouse cursor</li>
        <li>uicore::InputDevice - Interface for keyboards, mice and game controllers</li>
        <li>uicore::InputEvent - Describes an input event, such as key down, key up, mouse movement, etc</li>
        <li>uicore::InputCode - This enumeration contains all the input IDs for well-known buttons</li>
    </ul>

    <h2>2D Graphics</h2>

    <ul>
        <li>uicore::Canvas - Main 2D drawing class</li>
        <li>uicore::Image - 2D image</li>
        <li>uicore::Path - Describes a 2D path</li>
        <li>uicore::Color, uicore::Colorf - Specifies a color using red, green, blue and alpha components</li>
        <li>uicore::ColorHSLi, uicore::ColorHSLf - Specifies a color using hue, saturation, lightness and alpha components</li>
        <li>uicore::ColorHSVi, uicore::ColorHSVf - Specifies a color using hue, saturation, value and alpha components</li>
        <li>uicore::TextBlock - Draw rich text with word wrapping, images, fonts and colors</li>
        <li>uicore::TextureGroup - Calibrates which images end up in which atlas textures</li>
    </ul>

    <h2>Fonts</h2>

    <ul>
        <li>uicore::Font - Font class for System / TrueType / OpenType fonts and fonts using sprite image frames as glyphs</li>
        <li>uicore::FontDescription - Allows you to setup a more advanced description when creating a font</li>
        <li>uicore::FontMetrics - The metrics of a font</li>
    </ul>

    <h2>Image Pixels</h2>

    <ul>
        <li>uicore::PixelBuffer - Buffer containing image pixels (in CPU or GPU memory)</li>
        <li>uicore::PixelBufferSet - Set of images that combined form a complete texture</li>
        <li>uicore::PixelConverter - Pixel format converter class</li>
        <li>uicore::TextureFormat - Enumeration for all the pixel formats supported</li>
        <li>uicore::IconSet - Set of images for rendering an icon at different sizes</li>
        <li>uicore::PerlinNoise - Perlin Noise Generator</li>
    </ul>

    <h2>3D Graphics</h2>

    <ul>
        <li>uicore::GraphicContext - Main immediate drawing class</li>
        <li>uicore::Texture - Texture object base class</li>
        <li>uicore::Texture1D, uicore::Texture1DArray - One dimensional textures</li>
        <li>uicore::Texture2D, uicore::Texture2DArray - Two dimensional textures</li>
        <li>uicore::Texture3D - Three dimensional textures</li>
        <li>uicore::TextureCube, uicore::TextureCubeArray - Cube textures</li>
        <li>uicore::UniformBuffer, uicore::UniformVector&lt;T&gt; - GPU buffer for uniform variables</li>
        <li>uicore::VertexArrayBuffer, uicore::VertexArrayVector&lt;T&gt; - GPU buffer for vertex attributes</li>
        <li>uicore::ElementArrayBuffer, uicore::ElementArrayVector&lt;T&gt; - GPU buffer for element indices</li>
        <li>uicore::StorageBuffer, uicore::StorageVector&lt;T&gt; - GPU buffer for shader storage (aka. Unordered Access View)</li>
        <li>uicore::StagingBuffer, uicore::StagingTexture, uicore::StagingVector - CPU/GPU memory transfer buffers</li>
        <li>uicore::BlendState, uicore::BlendStateDescription - Blending state setup</li>
        <li>uicore::DepthStencilState, uicore::DepthStencilStateDescription - Depth and stencil state setup</li>
        <li>uicore::RasterizerState, uicore::RasterizerStateDescription - Rasterizer state setup</li>
        <li>uicore::FrameBuffer, uicore::RenderBuffer - Frame buffer setup</li>
        <li>uicore::ProgramObject, uicore::ShaderObject - Shader programs</li>
        <li>uicore::PrimitivesArray - Vertex buffer objects</li>
        <li>uicore::OcclusionQuery - Occlusion queries</li>
    </ul>

	<h2>Thread run-loop</h2>

	<ul>
	<li>uicore::RunLoop - Application loop helper</li>
	<li>uicore::Timer - Timer class that invokes a callback on a specified interval</li>
	</ul>

*/
