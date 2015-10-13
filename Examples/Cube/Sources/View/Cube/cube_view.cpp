
#include "precomp.h"
#include "cube_view.h"

using namespace uicore;

CubeView::CubeView()
{
	style()->set("flex: auto");
}

void CubeView::render_content(const CanvasPtr &canvas)
{
	time.update();
	rotate_angle = std::fmod(rotate_angle + time.get_time_elapsed() * 150.0f, 360.0f);

	render_scene(canvas);

	auto image = Image::create(scene_texture, scene_texture->size());
	image->draw(canvas, geometry().content_size());

	set_needs_render(); // To do: we need a request_animation_frame() function on View
}

void CubeView::render_scene(const uicore::CanvasPtr &canvas)
{
	// Tell canvas we are about to do our own GraphicContext rendering.
	// This flushes any pending rendering commands and restores the graphic context states to their defaults.
	canvas->end();

	auto gc = canvas->gc();

	if (!scene_texture)
	{
		auto vertex_filename = PathHelp::combine(UIThread::resource_path(), "cube_vertex.glsl");
		auto fragment_filename = PathHelp::combine(UIThread::resource_path(), "cube_fragment.glsl");
		program = ProgramObject::load(gc, vertex_filename, fragment_filename);
		program->bind_attribute_location(0, "Vertex");
		program->bind_frag_data_location(0, "FragColor");
		program->link();
		program->set_uniform_buffer_index("UniformBlock", 0);

		vertices = VertexArrayVector<Vec4f>(gc, cube_positions());
		uniform_buffer = UniformVector<UniformBlock>(gc, 1);

		prim_array = PrimitivesArray::create(gc);
		prim_array->set_attributes(0, vertices);

		int texture_width = (int)std::round(geometry().content_width * canvas->pixel_ratio());
		int texture_height = (int)std::round(geometry().content_height * canvas->pixel_ratio());
		scene_texture = Texture2D::create(gc, texture_width, texture_height);

		RasterizerStateDescription raster_desc;
		raster_desc.set_culled(true);
		rasterizer_state = gc->create_rasterizer_state(raster_desc);
		blend_state = gc->create_blend_state(BlendStateDescription::opaque());
		depth_stencil_state = gc->create_depth_stencil_state(DepthStencilStateDescription());
	}

	if (!frame_buffer || gc->is_frame_buffer_owner(frame_buffer))
	{
		frame_buffer = FrameBuffer::create(gc);
		frame_buffer->attach_color(0, scene_texture);
	}

	float fov = 60.0f;
	float aspect = geometry().content_width / geometry().content_height;
	Mat4f cube_rotate = Mat4f::rotate(Angle::from_degrees(rotate_angle), Vec3f(1.2f, 0.74f, 0.38f));
	Mat4f camera = Mat4f::translate(0.0f, 0.0f, 4.0f);
	Mat4f perspective = Mat4f::perspective(fov, aspect, 0.5f, 1000.0f, handed_left, gc->clip_z_range());

	UniformBlock uniform_block;
	uniform_block.transform = perspective * camera * cube_rotate;
	uniform_buffer.upload_data(gc, &uniform_block, 1);

	gc->set_frame_buffer(frame_buffer);
	gc->set_viewport(scene_texture->size(), gc->texture_image_y_axis());
	gc->clear(Colorf::transparent);

	gc->set_rasterizer_state(rasterizer_state);
	gc->set_blend_state(blend_state);
	gc->set_depth_stencil_state(depth_stencil_state);
	gc->set_program_object(program);
	gc->set_uniform_buffer(0, uniform_buffer);

	gc->draw_primitives(type_triangles, 6 * 6, prim_array);

	gc->set_uniform_buffer(0, nullptr);
	gc->set_program_object(nullptr);
	gc->set_rasterizer_state(nullptr);
	gc->set_blend_state(nullptr);
	gc->set_depth_stencil_state(nullptr);

	gc->set_frame_buffer(nullptr);
	gc->set_viewport(gc->size(), gc->texture_image_y_axis());

	// Resume rendering 2D.
	// Make sure you restored all graphic context states you changed back to their defaults before calling this.
	canvas->begin();
}

std::vector<uicore::Vec4f> CubeView::cube_positions()
{
	return
	{
		Vec4f { -1.0f, 1.0f, 1.0f, 0.0f },
		Vec4f { 1.0f, 1.0f, 1.0f, 0.0f },
		Vec4f { 1.0f, -1.0f, 1.0f, 0.0f },
		Vec4f { 1.0f, -1.0f, 1.0f, 0.0f },
		Vec4f { -1.0f, -1.0f, 1.0f, 0.0f },
		Vec4f { -1.0f, 1.0f, 1.0f, 0.0f },

		Vec4f { 1.0f, -1.0f, -1.0f, 1.0f },
		Vec4f { 1.0f, 1.0f, -1.0f, 1.0f },
		Vec4f { -1.0f, 1.0f, -1.0f, 1.0f },
		Vec4f { -1.0f, 1.0f, -1.0f, 1.0f },
		Vec4f { -1.0f, -1.0f, -1.0f, 1.0f },
		Vec4f { 1.0f, -1.0f, -1.0f, 1.0f },

		Vec4f { 1.0f, 1.0f, -1.0f, 2.0f },
		Vec4f { 1.0f, 1.0f, 1.0f, 2.0f },
		Vec4f { -1.0f, 1.0f, 1.0f, 2.0f },
		Vec4f { -1.0f, 1.0f, 1.0f, 2.0f },
		Vec4f { -1.0f, 1.0f, -1.0f, 2.0f },
		Vec4f { 1.0f, 1.0f, -1.0f, 2.0f },

		Vec4f { -1.0f, -1.0f, 1.0f, 3.0f },
		Vec4f { 1.0f, -1.0f, 1.0f, 3.0f },
		Vec4f { 1.0f, -1.0f, -1.0f, 3.0f },
		Vec4f { 1.0f, -1.0f, -1.0f, 3.0f },
		Vec4f { -1.0f, -1.0f, -1.0f, 3.0f },
		Vec4f { -1.0f, -1.0f, 1.0f, 3.0f },

		Vec4f { 1.0f, -1.0f, 1.0f, 4.0f },
		Vec4f { 1.0f, 1.0f, 1.0f, 4.0f },
		Vec4f { 1.0f, 1.0f, -1.0f, 4.0f },
		Vec4f { 1.0f, 1.0f, -1.0f, 4.0f },
		Vec4f { 1.0f, -1.0f, -1.0f, 4.0f },
		Vec4f { 1.0f, -1.0f, 1.0f, 4.0f },

		Vec4f { -1.0f, 1.0f, -1.0f, 5.0f },
		Vec4f { -1.0f, 1.0f, 1.0f, 5.0f },
		Vec4f { -1.0f, -1.0f, 1.0f, 5.0f },
		Vec4f { -1.0f, -1.0f, 1.0f, 5.0f },
		Vec4f { -1.0f, -1.0f, -1.0f, 5.0f },
		Vec4f { -1.0f, 1.0f, -1.0f, 5.0f }
	};
}
