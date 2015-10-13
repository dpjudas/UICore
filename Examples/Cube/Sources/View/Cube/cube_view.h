
#pragma once

class CubeView : public uicore::View
{
public:
	CubeView();

protected:
	void render_content(const uicore::CanvasPtr &canvas) override;

private:
	struct UniformBlock
	{
		uicore::Mat4f transform;
	};

	void render_scene(const uicore::CanvasPtr &canvas);

	static std::vector<uicore::Vec4f> cube_positions();

	uicore::ProgramObjectPtr program;
	uicore::VertexArrayVector<uicore::Vec4f> vertices;
	uicore::UniformVector<UniformBlock> uniform_buffer;
	uicore::PrimitivesArrayPtr prim_array;
	uicore::Texture2DPtr scene_texture;
	uicore::FrameBufferPtr frame_buffer;
	uicore::RasterizerStatePtr rasterizer_state;
	uicore::BlendStatePtr blend_state;
	uicore::DepthStencilStatePtr depth_stencil_state;

	float rotate_angle = 0.0f;
	uicore::GameTime time;
};
